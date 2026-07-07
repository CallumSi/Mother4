// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "BakeTileMapCommandlet.generated.h"

/**
 * Command-line entry point for baking a text map into a tile map asset, e.g.:
 *
 *   UnrealEditor-Cmd.exe MyProject.uproject -run=BakeTileMap
 *       -map="Maps/onett.map"
 *       -tilemap="/Game/Sprites/Map/OnettTilemap.OnettTilemap"
 *       -tileset="/Game/Sprites/Map/tilemap_TileSet.tilemap_TileSet"
 *
 * All three arguments are optional and default to the values above. The target
 * tile map package is saved on success.
 */
UCLASS()
class UBakeTileMapCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	virtual int32 Main(const FString& Params) override;
};
