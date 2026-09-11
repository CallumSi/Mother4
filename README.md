# Mother 4

> A personal, EarthBound-inspired 2D RPG built in **Unreal Engine 5** — a project to explore UE5's 2D toolset and build a bespoke, data-driven tilemap pipeline from scratch.

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.8-0E1128?logo=unrealengine&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-gameplay-00599C?logo=cplusplus&logoColor=white)
![PaperZD](https://img.shields.io/badge/2D-PaperZD-FF6F00)
![Status](https://img.shields.io/badge/status-personal%20project-blue)

An overhead, EarthBound/Mother-style world rendered with sprite tiles and
flipbook characters. The point of the project was less "ship a game" and more to
learn UE5's 2D pipeline end to end — sprite animation, Enhanced Input, and
especially **how to author large tile maps as plain text and bake them into the
engine**.

## Highlights

- 🗺️ **Custom text-driven tilemap pipeline** — levels are authored as human-readable `.map` files (`<symbol> = <tileIndex> [@ <layer>]` legends + an ASCII grid) and baked into UE tile maps via a `MapBaker` actor and a `BakeTileMap` commandlet. Edit a map in any text editor, re-bake, done.
- 🧩 **Autotiling reference for the tileset** — every tile's four edges were classified into material codes so terrain (grass, cliffs, water, sand, roads) connects by edge compatibility. The full analysis lives in [`Docs/OnettTileset_Reference.md`](Docs/OnettTileset_Reference.md): 848 tiles, edge-class matching, verified multi-tile "stamp" blocks, and overlay props.
- 🎞️ **2D character animation with PaperZD** — directional idle/walk flipbooks driven by an animation blueprint and a directionality state machine.
- 🎮 **Enhanced Input** — movement mapped through an Input Mapping Context (`IMC_Player`) and Input Actions.
- 🧱 **C++ gameplay layer** — character, HUD, a Blueprint function library, and the tilemap importer/baker are all implemented in C++ (`Source/MyProject`).

## How the tilemap pipeline works

```
  onett.map  ──▶  MapBaker actor / BakeTileMap commandlet  ──▶  UE Tile Map asset  ──▶  Level
  (plain text)     (reads legend + grid, resolves tiles,        (Content/…)
                    places them across layers)
```

A `.map` file declares a legend (`symbol = tile index @ layer`) and then draws
the level as a grid of those symbols. The baker parses it, looks up each tile on
the `tilemap_TileSet` sheet, and places it — so map design happens in text and
version-controls as clean diffs, instead of being trapped in a binary editor.

## Project structure

```
Source/MyProject/     C++ gameplay — MyCharacter, Player1, HudBB,
                      MapBaker, BakeTileMapCommandlet, TileMapImporterLibrary, …
Content/Blueprints/   BP_Player, animation blueprint, input actions
Content/Maps/         onett.map, probe.map  (text source maps)
Content/Sprites/      tileset sheet + character sprites
Docs/                 OnettTileset_Reference.md — the autotiling analysis
```

## Built with

Unreal Engine 5.8 · C++ · [PaperZD](https://github.com/CreativeReasoningStudio/PaperZD) (2D animation) · Enhanced Input

## Getting started

1. Install **Unreal Engine 5.8** and the **PaperZD** plugin.
2. Clone the repo and open `MyProject.uproject` (let it build the C++ modules on first launch).
3. Open `Content/Levels/Mother4.umap` and press Play.

To re-bake a text map, edit a file in `Content/Maps/` and run the `BakeTileMap`
commandlet (or drop a `MapBaker` actor into the level and trigger it).

## Notes

This is a **non-commercial personal/learning project** inspired by the *Mother /
EarthBound* series. Any third-party sprite, tileset, or reference assets remain
the property of their respective owners and are used here only for study.
