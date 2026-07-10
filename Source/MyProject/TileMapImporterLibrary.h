// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TileMapImporterLibrary.generated.h"

class UPaperTileMap;
class UPaperTileSet;

/**
 * Builds a Paper2D tile map from a plain-text description.
 *
 * The text file has two sections:
 *
 *   [legend]
 *   . = 0          # <symbol> = <tile index in the tile set>
 *   T = 12 @ 1     # optional "@ <layer>" puts that symbol on a specific layer (default 0)
 *
 *   [map]
 *   TTTTT          # one character per tile; a space leaves the cell empty
 *   T...T
 *   TTTTT
 *
 * Lines starting with '#' and blank lines are ignored inside [legend].
 * Inside [map] every non-empty line is a row, taken verbatim.
 *
 * The map/legend are data, so re-painting only needs an edit to the text file
 * plus a re-run of the baker - no recompile.
 *
 * bMirrorX: this level's tile map actor renders horizontally mirrored in-game,
 * so pass true to pre-mirror the bake (place each tile at the mirrored column and
 * set Paper2D's horizontal-flip flag) - the two mirrors cancel and the map reads
 * correctly in the running game.
 */
UCLASS()
class MYPROJECT_API UTileMapImporterLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Parses MapText and writes the resulting tiles into TileMap. Marks the tile map's package dirty. */
	UFUNCTION(BlueprintCallable, Category = "Tile Map Importer")
	static bool BakeTileMapFromString(UPaperTileMap* TileMap, UPaperTileSet* TileSet, const FString& MapText, bool bMirrorX, FString& OutError);

	/** Loads a .map text file (path resolved against the project & Content dirs) and bakes it into TileMap. */
	UFUNCTION(BlueprintCallable, Category = "Tile Map Importer")
	static bool BakeTileMapFromFile(UPaperTileMap* TileMap, UPaperTileSet* TileSet, const FString& FilePath, bool bMirrorX, FString& OutError);

	/** Resolves a possibly-relative map path against the project Content dir, then the project dir. */
	static FString ResolveMapPath(const FString& InPath);
};
