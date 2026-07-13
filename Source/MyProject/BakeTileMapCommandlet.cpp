// Fill out your copyright notice in the Description page of Project Settings.

#include "BakeTileMapCommandlet.h"

#include "TileMapImporterLibrary.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "UObject/Package.h"
#include "UObject/SavePackage.h"
#include "Misc/PackageName.h"

DEFINE_LOG_CATEGORY_STATIC(LogBakeTileMap, Log, All);

int32 UBakeTileMapCommandlet::Main(const FString& Params)
{
	TArray<FString> Tokens;
	TArray<FString> Switches;
	TMap<FString, FString> ParamsMap;
	UCommandlet::ParseCommandLine(*Params, Tokens, Switches, ParamsMap);

	FString MapPath = ParamsMap.FindRef(TEXT("map"));
	if (MapPath.IsEmpty()) { MapPath = TEXT("Maps/onett.map"); }

	FString TileMapPath = ParamsMap.FindRef(TEXT("tilemap"));
	// Matches the target the in-editor AMapBaker actor writes into (the "_Text" tile map).
	if (TileMapPath.IsEmpty()) { TileMapPath = TEXT("/Game/Sprites/Map/OnettTilemap_Text.OnettTilemap_Text"); }

	FString TileSetPath = ParamsMap.FindRef(TEXT("tileset"));
	if (TileSetPath.IsEmpty()) { TileSetPath = TEXT("/Game/Sprites/Map/tilemap_TileSet.tilemap_TileSet"); }

	UPaperTileMap* TileMap = LoadObject<UPaperTileMap>(nullptr, *TileMapPath);
	if (!TileMap)
	{
		UE_LOG(LogBakeTileMap, Error, TEXT("Could not load tile map: %s"), *TileMapPath);
		return 1;
	}

	UPaperTileSet* TileSet = LoadObject<UPaperTileSet>(nullptr, *TileSetPath);
	if (!TileSet)
	{
		UE_LOG(LogBakeTileMap, Warning, TEXT("Could not load tile set: %s (baking without one)"), *TileSetPath);
	}

	// Default ON to match AMapBaker: the Onett/Mother4 tilemap actor presents its
	// back face to the camera (rotated 180deg), which mirrors everything - so the
	// bake must pre-flip tiles or asymmetric art (signs/lettering) reads backwards.
	// Pass -nomirror only for an actor that shows its front face un-reflected.
	const bool bMirrorX = !Switches.Contains(TEXT("nomirror"));

	FString Error;
	if (!UTileMapImporterLibrary::BakeTileMapFromFile(TileMap, TileSet, MapPath, bMirrorX, Error))
	{
		UE_LOG(LogBakeTileMap, Error, TEXT("Bake failed: %s"), *Error);
		return 1;
	}

	// The bake writes tiles into the tile MAP and (for 'solid' tiles) collision into
	// the tile SET, so both packages may be dirty and must be saved.
	auto SaveAsset = [](UObject* Asset) -> bool
	{
		if (!Asset) { return true; }
		UPackage* Package = Asset->GetOutermost();
		Package->SetDirtyFlag(true);

		const FString FileName = FPackageName::LongPackageNameToFilename(
			Package->GetName(), FPackageName::GetAssetPackageExtension());

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;

		if (!UPackage::SavePackage(Package, Asset, *FileName, SaveArgs))
		{
			UE_LOG(LogBakeTileMap, Error, TEXT("Bake succeeded but saving failed: %s"), *FileName);
			return false;
		}
		UE_LOG(LogBakeTileMap, Display, TEXT("Saved %s"), *FileName);
		return true;
	};

	if (!SaveAsset(TileMap) || !SaveAsset(TileSet))
	{
		return 1;
	}

	UE_LOG(LogBakeTileMap, Display, TEXT("Baked '%s' into '%s'"), *MapPath, *TileMapPath);
	return 0;
}
