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
	if (TileMapPath.IsEmpty()) { TileMapPath = TEXT("/Game/Sprites/Map/OnettTilemap.OnettTilemap"); }

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

	FString Error;
	if (!UTileMapImporterLibrary::BakeTileMapFromFile(TileMap, TileSet, MapPath, Error))
	{
		UE_LOG(LogBakeTileMap, Error, TEXT("Bake failed: %s"), *Error);
		return 1;
	}

	UPackage* Package = TileMap->GetOutermost();
	Package->SetDirtyFlag(true);

	const FString FileName = FPackageName::LongPackageNameToFilename(
		Package->GetName(), FPackageName::GetAssetPackageExtension());

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.SaveFlags = SAVE_NoError;

	const bool bSaved = UPackage::SavePackage(Package, TileMap, *FileName, SaveArgs);
	if (!bSaved)
	{
		UE_LOG(LogBakeTileMap, Error, TEXT("Bake succeeded but saving the package failed: %s"), *FileName);
		return 1;
	}

	UE_LOG(LogBakeTileMap, Display, TEXT("Baked '%s' into '%s' and saved %s"), *MapPath, *TileMapPath, *FileName);
	return 0;
}
