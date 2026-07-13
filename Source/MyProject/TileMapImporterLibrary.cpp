// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMapImporterLibrary.h"

#include "PaperTileMap.h"
#include "PaperTileLayer.h"
#include "PaperTileSet.h"
#include "SpriteEditorOnlyTypes.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogTileMapImporter, Log, All);

namespace
{
	struct FParsedMap
	{
		TMap<TCHAR, int32> SymbolToIndex;
		TMap<TCHAR, int32> SymbolToLayer;
		TSet<TCHAR>        SolidSymbols;   // symbols whose tiles get full-tile collision
		TArray<FString>    Rows;
	};

	void ParseMapText(const FString& Text, FParsedMap& Out)
	{
		TArray<FString> Lines;
		Text.ParseIntoArrayLines(Lines, /*InCullEmpty=*/false);

		enum class ESection { None, Legend, Map } Section = ESection::None;

		for (FString Line : Lines)
		{
			Line.RemoveFromEnd(TEXT("\r"));

			FString Trimmed = Line;
			Trimmed.TrimStartAndEndInline();

			if (Trimmed.Equals(TEXT("[legend]"), ESearchCase::IgnoreCase)) { Section = ESection::Legend; continue; }
			if (Trimmed.Equals(TEXT("[map]"), ESearchCase::IgnoreCase))    { Section = ESection::Map;    continue; }

			if (Section == ESection::Legend)
			{
				if (Trimmed.IsEmpty() || Trimmed.StartsWith(TEXT("#")))
				{
					continue;
				}

				FString Left, Right;
				if (Trimmed.Split(TEXT("="), &Left, &Right))
				{
					Left.TrimStartAndEndInline();
					Right.TrimStartAndEndInline();
					if (Left.Len() == 0)
					{
						continue;
					}

					// Allow a trailing "# comment" after the value.
					int32 Hash = INDEX_NONE;
					if (Right.FindChar(TEXT('#'), Hash))
					{
						Right = Right.Left(Hash).TrimEnd();
					}

					// Detect (and strip) an optional "solid" flag anywhere in the value,
					// e.g. "183 solid" or "12 @ 1 solid". Tiles marked solid get full-tile
					// collision written into the tile set during the bake.
					bool bSolid = false;
					{
						TArray<FString> Tokens;
						Right.ParseIntoArrayWS(Tokens);
						FString Rebuilt;
						for (const FString& Tok : Tokens)
						{
							if (Tok.Equals(TEXT("solid"), ESearchCase::IgnoreCase))
							{
								bSolid = true;
								continue;
							}
							if (!Rebuilt.IsEmpty()) { Rebuilt += TEXT(" "); }
							Rebuilt += Tok;
						}
						Right = Rebuilt;
					}

					int32 Layer = 0;
					FString IndexStr = Right;
					FString LayerStr;
					if (Right.Split(TEXT("@"), &IndexStr, &LayerStr))
					{
						IndexStr.TrimStartAndEndInline();
						LayerStr.TrimStartAndEndInline();
						Layer = FCString::Atoi(*LayerStr);
					}

					const TCHAR Symbol = Left[0];
					Out.SymbolToIndex.Add(Symbol, FCString::Atoi(*IndexStr));
					Out.SymbolToLayer.Add(Symbol, Layer);
					if (bSolid)
					{
						Out.SolidSymbols.Add(Symbol);
					}
				}
			}
			else if (Section == ESection::Map)
			{
				// Skip only leading blank lines; keep everything else verbatim so
				// column positions line up exactly with the source text.
				if (Out.Rows.Num() == 0 && Line.IsEmpty())
				{
					continue;
				}
				Out.Rows.Add(Line);
			}
		}

		// Drop trailing blank rows introduced by the file's final newline / formatting.
		while (Out.Rows.Num() > 0 && Out.Rows.Last().TrimEnd().IsEmpty())
		{
			Out.Rows.Pop();
		}
	}

	// Writes a full-tile collision box into every solid tile of the tile set, then
	// switches the tile map to 3D collision and rebuilds its body setup. Collision in
	// Paper2D lives on the tile SET (per-tile geometry), not on the placed cells, so
	// this mutates the tile set asset too - it must be saved alongside the tile map.
	void ApplyTileCollision(UPaperTileMap* TileMap, UPaperTileSet* TileSet, const TSet<int32>& SolidTileIndices)
	{
		const FIntPoint TileSize = TileSet->GetTileSize();
		const FVector2D BoxSize(TileSize.X, TileSize.Y);

		int32 WithCollision = 0;
		for (const int32 TileIndex : SolidTileIndices)
		{
			FPaperTileMetadata* Meta = TileSet->GetMutableTileMetadata(TileIndex);
			if (!Meta)
			{
				continue;
			}
			// Reset first so re-baking replaces (not stacks) the box. Tile geometry is
			// authored around the tile centre, so a centred box of TileSize covers it.
			Meta->CollisionData.Reset();
			Meta->CollisionData.GeometryType = ESpritePolygonMode::FullyCustom;
			Meta->CollisionData.AddRectangleShape(FVector2D::ZeroVector, BoxSize);
			++WithCollision;
		}

		TileSet->MarkPackageDirty();
#if WITH_EDITOR
		TileSet->PostEditChange();
#endif

		// Per-tile geometry is only baked into the body when the map has a collision
		// domain and each layer is flagged to collide - otherwise it is silently ignored.
		TileMap->SetCollisionDomain(ESpriteCollisionMode::Use3DPhysics);
		for (UPaperTileLayer* Layer : TileMap->TileLayers)
		{
			if (Layer)
			{
				Layer->SetLayerCollides(true);
			}
		}
		TileMap->RebuildCollision();

		UE_LOG(LogTileMapImporter, Display,
			TEXT("Wrote full-tile collision to %d tile(s) and rebuilt tile map collision. Save the tile SET asset too."),
			WithCollision);
	}

	bool ApplyToTileMap(UPaperTileMap* TileMap, UPaperTileSet* TileSet, const FParsedMap& Parsed, bool bMirrorX, FString& OutError)
	{
		const int32 Height = Parsed.Rows.Num();
		int32 Width = 0;
		for (const FString& Row : Parsed.Rows)
		{
			Width = FMath::Max(Width, Row.Len());
		}

		if (Width == 0 || Height == 0)
		{
			OutError = TEXT("The [map] section contained no tiles.");
			return false;
		}

		int32 MaxLayer = 0;
		for (const TPair<TCHAR, int32>& Pair : Parsed.SymbolToLayer)
		{
			MaxLayer = FMath::Max(MaxLayer, Pair.Value);
		}

		// Match tile pixel size to the tile set so the map renders at the right scale.
		if (TileSet)
		{
			const FIntPoint TileSize = TileSet->GetTileSize();
			if (TileSize.X > 0) { TileMap->TileWidth = TileSize.X; }
			if (TileSize.Y > 0) { TileMap->TileHeight = TileSize.Y; }
		}

		TileMap->ResizeMap(Width, Height, /*bForceResize=*/true);

		while (TileMap->TileLayers.Num() <= MaxLayer)
		{
			TileMap->AddNewLayer();
		}
		for (UPaperTileLayer* Layer : TileMap->TileLayers)
		{
			if (Layer)
			{
				Layer->ResizeMap(Width, Height);
			}
		}

		// Clear every cell first so a re-bake fully replaces prior content.
		const FPaperTileInfo EmptyCell;
		for (UPaperTileLayer* Layer : TileMap->TileLayers)
		{
			if (!Layer) { continue; }
			for (int32 Y = 0; Y < Height; ++Y)
			{
				for (int32 X = 0; X < Width; ++X)
				{
					Layer->SetCell(X, Y, EmptyCell);
				}
			}
		}

		TArray<TCHAR> UnknownSymbols;
		int32 TilesPlaced = 0;
		for (int32 Y = 0; Y < Height; ++Y)
		{
			const FString& Row = Parsed.Rows[Y];
			for (int32 X = 0; X < Row.Len(); ++X)
			{
				const TCHAR Symbol = Row[X];
				if (Symbol == TEXT(' '))
				{
					continue; // space = intentionally empty
				}

				const int32* IndexPtr = Parsed.SymbolToIndex.Find(Symbol);
				if (!IndexPtr)
				{
					UnknownSymbols.AddUnique(Symbol);
					continue;
				}

				int32 Layer = 0;
				if (const int32* LayerPtr = Parsed.SymbolToLayer.Find(Symbol))
				{
					Layer = *LayerPtr;
				}
				if (!TileMap->TileLayers.IsValidIndex(Layer))
				{
					continue;
				}

				// The level's tile map actor displays mirrored in-game; when bMirrorX is set
				// we place at the mirrored column and horizontally flip the tile art so the
				// two mirrors cancel and the map reads correctly in the running game.
				const int32 DestX = bMirrorX ? (Width - 1 - X) : X;

				FPaperTileInfo Info;
				Info.TileSet = TileSet;
				Info.PackedTileIndex = *IndexPtr;
				if (bMirrorX)
				{
					Info.PackedTileIndex |= static_cast<int32>(EPaperTileFlags::FlipHorizontal);
				}
				TileMap->TileLayers[Layer]->SetCell(DestX, Y, Info);
				++TilesPlaced;
			}
		}

		// Give every solid-flagged tile full-tile collision (once we know the tile set).
		if (TileSet)
		{
			TSet<int32> SolidTileIndices;
			for (const TCHAR Sym : Parsed.SolidSymbols)
			{
				if (const int32* Idx = Parsed.SymbolToIndex.Find(Sym))
				{
					SolidTileIndices.Add(*Idx);
				}
			}
			if (SolidTileIndices.Num() > 0)
			{
				ApplyTileCollision(TileMap, TileSet, SolidTileIndices);
			}
		}

		if (UnknownSymbols.Num() > 0)
		{
			FString Joined;
			for (const TCHAR C : UnknownSymbols)
			{
				Joined += FString::Printf(TEXT("'%c' "), C);
			}
			UE_LOG(LogTileMapImporter, Warning,
				TEXT("Ignored %d symbol(s) with no legend entry: %s"), UnknownSymbols.Num(), *Joined);
		}

		TileMap->MarkPackageDirty();
#if WITH_EDITOR
		TileMap->PostEditChange();
#endif

		UE_LOG(LogTileMapImporter, Display,
			TEXT("Baked %dx%d tile map (%d tiles placed across %d layer(s))."),
			Width, Height, TilesPlaced, TileMap->TileLayers.Num());
		return true;
	}
}

bool UTileMapImporterLibrary::BakeTileMapFromString(UPaperTileMap* TileMap, UPaperTileSet* TileSet, const FString& MapText, bool bMirrorX, FString& OutError)
{
	if (!TileMap)
	{
		OutError = TEXT("TileMap is null.");
		return false;
	}

	FParsedMap Parsed;
	ParseMapText(MapText, Parsed);

	if (Parsed.Rows.Num() == 0)
	{
		OutError = TEXT("No [map] rows were found. Did the file include a '[map]' section header?");
		return false;
	}
	if (Parsed.SymbolToIndex.Num() == 0)
	{
		OutError = TEXT("The legend was empty. Add a '[legend]' section with 'symbol = tileIndex' lines.");
		return false;
	}

	return ApplyToTileMap(TileMap, TileSet, Parsed, bMirrorX, OutError);
}

bool UTileMapImporterLibrary::BakeTileMapFromFile(UPaperTileMap* TileMap, UPaperTileSet* TileSet, const FString& FilePath, bool bMirrorX, FString& OutError)
{
	const FString Resolved = ResolveMapPath(FilePath);

	FString Text;
	if (!FFileHelper::LoadFileToString(Text, *Resolved))
	{
		OutError = FString::Printf(TEXT("Could not read map file: %s"), *Resolved);
		return false;
	}

	return BakeTileMapFromString(TileMap, TileSet, Text, bMirrorX, OutError);
}

FString UTileMapImporterLibrary::ResolveMapPath(const FString& InPath)
{
	if (FPaths::FileExists(InPath))
	{
		return InPath;
	}

	const FString UnderContent = FPaths::ProjectContentDir() / InPath;
	if (FPaths::FileExists(UnderContent))
	{
		return UnderContent;
	}

	const FString UnderProject = FPaths::ProjectDir() / InPath;
	if (FPaths::FileExists(UnderProject))
	{
		return UnderProject;
	}

	// Fall back to the Content-relative path so the error message is meaningful.
	return UnderContent;
}
