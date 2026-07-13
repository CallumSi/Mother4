// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapBaker.generated.h"

class UPaperTileMap;
class UPaperTileSet;

/**
 * Drop this actor into a level, point it at a tile map asset + tile set, then
 * press the "Generate From File" button in the Details panel to (re)paint the
 * tile map from a text file. After baking, save the tile map asset (Ctrl+S).
 *
 * The tile map stays a normal Paper2D asset, so it remains fully editable with
 * the paint tool afterwards.
 */
UCLASS()
class MYPROJECT_API AMapBaker : public AActor
{
	GENERATED_BODY()

public:
	AMapBaker();

	/** The tile map asset to write into (e.g. OnettTilemap). */
	UPROPERTY(EditAnywhere, Category = "Map Baker")
	TObjectPtr<UPaperTileMap> TargetTileMap;

	/** The tile set the legend indices refer to (e.g. tilemap_TileSet). */
	UPROPERTY(EditAnywhere, Category = "Map Baker")
	TObjectPtr<UPaperTileSet> TileSet;

	/** Path to the .map text file, relative to the project's Content folder. */
	UPROPERTY(EditAnywhere, Category = "Map Baker")
	FString MapFilePath = TEXT("Maps/onett.map");

	/**
	 * Pre-mirror the bake (reverse columns + set each tile's horizontal-flip flag).
	 * ONLY enable this if the tile map actor itself renders reflected (negative X scale);
	 * otherwise it reflects the whole map and asymmetric tiles (signs, text) read backwards.
	 * The Onett map's orientation is a camera rotation, not a reflection, so leave this OFF.
	 */
	UPROPERTY(EditAnywhere, Category = "Map Baker")
	bool bMirrorX = false;

	/** Reads MapFilePath and bakes it into TargetTileMap. Remember to save the asset afterwards. */
	UFUNCTION(CallInEditor, Category = "Map Baker")
	void GenerateFromFile();
};
