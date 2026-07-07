// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMapImporterLibrary.h"

#include "PaperTileMap.h"
#include "PaperTileLayer.h"
#include "PaperTileSet.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogTileMapImporter, Log, All);

namespace
{
	struct FParsedMap
	{
		TMap<TCHAR, int32> SymbolToIndex;
		TMap<TCHAR, int32> SymbolToLayer;
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

	bool ApplyToTileMap(UPaperTileMap* TileMap, UPaperTileSet* TileSet, const FParsedMap& Parsed, FString& OutError)
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

				FPaperTileInfo Info;
				Info.TileSet = TileSet;
				Info.PackedTileIndex = *IndexPtr;
				TileMap->TileLayers[Layer]->SetCell(X, Y, Info);
				++TilesPlaced;
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

bool UTileMapImporterLibrary::BakeTileMapFromString(UPaperTileMap* TileMap, UPaperTileSet* TileSet, const FString& MapText, FString& OutError)
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

	return ApplyToTileMap(TileMap, TileSet, Parsed, OutError);
}

bool UTileMapImporterLibrary::BakeTileMapFromFile(UPaperTileMap* TileMap, UPaperTileSet* TileSet, const FString& FilePath, FString& OutError)
{
	const FString Resolved = ResolveMapPath(FilePath);

	FString Text;
	if (!FFileHelper::LoadFileToString(Text, *Resolved))
	{
		OutError = FString::Printf(TEXT("Could not read map file: %s"), *Resolved);
		return false;
	}

	return BakeTileMapFromString(TileMap, TileSet, Text, OutError);
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
