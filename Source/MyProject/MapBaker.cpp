// Fill out your copyright notice in the Description page of Project Settings.

#include "MapBaker.h"

#include "TileMapImporterLibrary.h"
#include "PaperTileMap.h"

DEFINE_LOG_CATEGORY_STATIC(LogMapBaker, Log, All);

AMapBaker::AMapBaker()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMapBaker::GenerateFromFile()
{
	if (!TargetTileMap)
	{
		UE_LOG(LogMapBaker, Error, TEXT("Map Baker: 'Target Tile Map' is not set."));
		return;
	}

	FString Error;
	const bool bOk = UTileMapImporterLibrary::BakeTileMapFromFile(TargetTileMap, TileSet, MapFilePath, Error);
	if (bOk)
	{
		UE_LOG(LogMapBaker, Display,
			TEXT("Map Baker: baked '%s' into '%s'. Save the tile map asset (Ctrl+S) to keep the change."),
			*MapFilePath, *TargetTileMap->GetName());
	}
	else
	{
		UE_LOG(LogMapBaker, Error, TEXT("Map Baker failed: %s"), *Error);
	}
}
