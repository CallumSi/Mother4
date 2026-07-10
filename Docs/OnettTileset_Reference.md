# Onett-style tilemap — Complete tile-matching reference

> Reference for `tilemap_TileSet` (`/Game/Sprites/Map/tilemap_TileSet`, tile sheet `/Game/Sprites/Map/tilemap`).
> Used by the text-map importer (`Content/Maps/*.map`, baked via the `MapBaker` actor / `BakeTileMap` commandlet).
> Legend line format in a `.map` file: `<symbol> = <tileIndex> [ @ <layer> ]`.

**Sheet geometry (pixel-verified):** 16 columns × 53 rows = **848 tiles**, each **32×32 px**, separated by 1 px white gridlines. Tiles are numbered **0–847, left→right, top→bottom**. Tile *n* is at row `n ÷ 16`, column `n mod 16`. Tiles **841–847** are blank white filler (the sheet's last row is only partially used).

---

# 1 · How matching works on this sheet (three mechanisms)

**A. Edge-class matching (terrain autotiles).** Grass, cliffs, water, sand paths and roads connect by *edge compatibility*: the right edge of tile A must show the same material as the left edge of tile B (same vertically: bottom↔top). Every tile's four edges have been classified into material codes (see Appendix A). The rule is simply: **E-code of A = W-code of B → A can sit left of B; S-code of A = N-code of B → A can sit above B.** Pure codes (`G`, `W`, `S`, `P`…) are freely interchangeable across the whole sheet — any pure-grass edge meets any other pure-grass edge. Mixed/transition codes (e.g. `C1G~`, `GS~`) only meet edges of the same mixed code, and these are the tiles that carry a terrain boundary across them.

**B. Fixed multi-tile blocks (stamps).** Buildings, signs, big trees, log walls, ponds and shoreline formations only assemble one way. Every sheet-adjacent tile pair was pixel-compared, yielding **150 verified seamless blocks** — their exact layouts are in Appendix B. Within a block, the adjacency shown in the grid is the *only* correct arrangement.

**C. Overlay props.** Single-tile decorations (stop sign 272/312, mailbox 276/310, manhole 316–318, lone flowers, stumps, bushes) have pure-grass or pure-pavement edges on all four sides, so they drop onto any matching base with no constraints.

---

# 2 · Edge-class legend

| code | material | meets |
|---|---|---|
| `G` | flat grass green | any other `G` edge — the universal connector |
| `G+` | grass with tuft/flower pixels touching the edge | `G` and `G+` (visually seamless) |
| `W` / `W+` | deep ocean purple (plain / with sparkle) | `W`/`W+` |
| `V` | light shallow water / river | `V` |
| `S` / `S+` | sand & dirt path | `S`/`S+` |
| `C1+`, `C2` | cliff-rock face interior | same code (continues a cliff face) |
| `P`,`P+`,`P2+` | pavement / concrete (dark & light) | same family — road continues |
| `K`,`K+` | black/shadow (building interiors, cliff bases, tree shadow) | same code |
| `GK~` | grass↔dark transition (canopy or cliff silhouette crossing edge) | `GK~` only |
| `C1G~` | cliff↔grass boundary crossing the edge | `C1G~` only |
| `GS~` | grass↔sand path boundary | `GS~` only |
| `C1K~`,`C1S~`,`C1P2~`,`KW~`,`GW~`,`GP2~`,`PP2~`,`KP2~` | other boundary edges (cliff/shadow, cliff/sand, water lines, grass/pavement, curb lines) | same code only |
| `X` | white (blank filler tiles) | n/a |

The `~` codes are the important ones: **a transition can only continue through tiles that carry the identical transition on the facing edge.** That is the entire logic of assembling coastlines, cliff rims and road borders from this sheet.

---

# 3 · Zone-by-zone breakdown

The sheet is organized in horizontal terrain bands. Row numbers below are tile-grid rows (16 tiles per row).

## Zone 1 — Grass & meadow (rows 0–2, tiles 0–47)
- **0, 3–10, 16–26:** grass fills — plain (`16`, `22`…) and tufted variants. All four edges `G/G+`; every one of these interchanges freely and borders anything with a `G` edge. This is your base terrain.
- **1–2 + 17:** a small **sand-path fragment** (verified block `[1,2,17]`) — path entering grass; its path edges are `S/GS~` and continue into the Zone 4 path set.
- **11/27, 13/29:** grass with a dark diagonal shading edge (verified vertical pairs) — used to shade the base of cliffs from Zone 2; the dark edge is `GK~` and mates with the cliff-bottom tiles at rows 3–8.
- **27–41:** **flower patches** — red/white flower scatter over grass. Edges all `G+`, so they're pure overlays; put them anywhere grass goes. Verified clusters: `[30,31]`, `[24,25,26,...]` inside the big meadow block `[5–10, 19–26, 35–42]` (a whole ready-made meadow scene).
- **42–45:** **flowerbed with picket edging** (block `[44,45]` plus 42–43) — a garden strip; grass edges all round.
- **45–47:** **stepping-stones/pavement patches** on grass — overlay tiles.

## Zone 2 — Pond & cliff plateau (rows 3–8, tiles 48–143)
The big autotile set of the sheet: a raised rock plateau with grass on top.
- **Pond water:** 48, 52–53, 56–58 are flat purple pond fills (`W` edges); **49/65, 50/66, 51, 54–55/70–71, 59/75** are verified vertical pond-shore blocks — water top, grass-and-rock shore bottom. 62–63 carry the pond's right rim.
- **Cliff faces (interior):** 80–81, 97–98, 101–104, 116–118, 121, 137 — full rock-texture tiles, edges `C1+`; they stack to make a cliff any height. Verified columns: `[102,103,104,120]`, `[89,105]`, `[93,94,109]`, `[140,154,155,156]`.
- **Cliff rims:** everything with a `C1G~` edge is a rim piece — grass on one side, rock on the other. The direction of the mixed edge tells you which side: e.g. **64–79** row carries top rims (grass above, rock below → `S` edge is `C1G~`), **128–136** carry bottom rims onto grass.
- **Inner/outer corners:** 67–68, 72–73, 77–78 (verified pairs) are the plateau's corner turns.
- **140 (+154–156):** the **cave mouth**, a verified 4-tile block — dark arch set into a cliff face; place only into a `C1+` face.
- **84:** rock with a bush overlay; **60–61:** grass ledge with hedge.

## Zone 3 — Dirt paths & pond depressions (rows 9–10, tiles 144–179)
- **144–152, 160–171:** the **sand/dirt path** autotile: wide fills (149–152 verified 4-wide strip `[149,150,151,152]`), diagonal path bends (144–146 block), narrows and forks (163–171). Path interiors have `S+` edges; path borders `GS~`. These paths also mate with Zone 1's fragment 1–2/17 and Zone 10's beach sand.
- **161:** a dark **pit/depression** in a path; **173–175 + 178–179:** verified **pond-depression blocks** — oval dark pits with raised rims, sit on grass or path.
- **176–177:** path corner descending from a cliff (mates `C1S~` with Zone 2).

## Zone 4 — Forest (rows 11–16, tiles 180–263)
- **180, 192/208/224, 203, 211–212, 230, 241–243, 245, 256, 259–261:** **tree trunks/stumps and single trees** — most are 1–2 tile props on grass. Verified vertical tree `[192,208,224]` (3-tall tree), `[243,244,259]`, `[260,261]`.
- **181–191, 197–202, 209–210, 218–223, 226–229, 231–234, 246–253, 262–263:** **canopy clusters** — bushy tree masses that interlock via `GK~`/`DG` edges. Verified blocks: `[181,182]`, `[166,167,183,184,185]` (big canopy with a sand arch above), `[218,219,220,221]` (4-wide canopy), `[234,237,238,248–255]` (an 11-tile forest wall — the largest vegetation block), `[222,223]`, `[199,200]`, `[201,202]`.
- **213–216 and 235–237:** **horizontal log walls** (fallen-tree barriers) — verified runs `[214,215]`, `[216,217]`; ends cap with 213/216 and 235/237.
- **Duplicates:** 248=254, 249=255, 250=257 — the canopy fill repeats; interchangeable.

## Zone 5 — Roads & street props (rows 16–19, tiles 256–319)
- **264–271, 280–309, 313–315, 319:** the **pavement autotile**: light slab fill (`P2+` edges), lane-line tiles, curb-to-grass borders (`GP2~` edges), and **diagonal road** pieces — verified diagonals `[283–286, 300–302]` (7-tile diagonal street section), `[291,304–307]` (5-tile bend), `[292–296]` (5-wide straight), `[272,273,288,289,290]` and `[264,265,281,297–299]` (intersection corners).
- **Props (overlay):** **272 & 312 = STOP signs**, **276 & 310 = blue mailboxes**, **316–318 = manhole/oval covers** (verified 3-run `[316,317,318]`), **278 = tree growing through pavement**.
- Curb tiles carry `GP2~`/`PP2~` edges — a road can only meet grass through these.

## Zone 6 — Fences, hedges & town signage (rows 20–23, tiles 320–383)
- **320–328:** **red/brown picket fence** with corner turns — verified corner `[321,322,337]`. Fence tiles connect end-to-end left/right; corner tiles turn the run 90°.
- **333–339:** **white picket fence** run with gate/end pieces.
- **340–346:** **tall wooden slat wall** (verified pair `[343,344]`), ends at 340/346; **345–346** include a hedge bush.
- **347–351:** **chain-link fence** on a brick base — verified `[331,347]`, `[349,350]`; 351 is the diagonal chain-link end.
- **352–354:** chain-link corner + red bench on a wall.
- **Billboards/signs** (all fixed stamps, grass edges around): **355–357** blank framed signs (`[356,357]`), **358–359** wide low sign, **360–361** "TOWN" sign + hatted man figure, **362–363 = "ONETT" sign** (duplicated exactly at **371–372**), **364–366** small signs (with **364/380–381** verified L-shape), **368–369 = "W…" sign**, **373–374 = "ONETT TOWN" cloth banner**.
- **375–379:** first houses — brown gabled roof (375–377), **378–379 yellow kiosk/shack** with awning.
- **382–383:** blue door + brick shopfront fragments.

## Zone 7 — Town buildings I: houses & shopfronts (rows 24–31, tiles 384–511)
Everything here is stamp-assembled; grids in Appendix B. Highlights with verified blocks:
- **386–391 + 402–407:** long **gray civic building** — roofline `[387,388,389]` (387=396 duplicate), entrance run `[390,391]`, lower floor `[402,403,404,405,406,407]` (6-wide verified) — and its **purple-roof house** neighbor `[405,406,407]`.
- **392–395, 408–410, 417–424:** brick row-houses — verified window pairs `[417,418]`, `[419,420]` (419=439, 420=572 dups), `[421,422]`, `[423,424]`, `[427,428]`, `[429,430]`.
- **412–414 (+416):** tan building strip (412=416 duplicate).
- **432–435:** **hotel entrance** — stone façade, double door, lit windows.
- **437 + 477–479 + 480:** purple-striped awning/roof pieces (Onett's arcade-style roofs).
- **440–442:** verified gray slab building `[440,441,442]`.
- **444–446: "DRUGSTORE"** lettering + green-striped awning; continues onto 461–462 and the shop face at 450–453.
- **447–449: burger sign + "BURGER SHOP"** lettering.
- **456–462:** white pyramid-roof building (verified 6-run `[457–462]`).
- **463 + 464–467: "TOWN HALL"** — sign text at 463/464, columned hall `[465,466,467]`; portico columns 468–470, entrance steps `[471–476]` (verified 6-run) with flower beds 474–475.
- **483–485:** red-striped-roof cottage; **486–488:** orange cottage with red-striped awning.
- **490–495:** stone public building, dark doorway `[492,493]`.
- **496–511:** its lower story: pillared façade `[500,501]`, windows `[503,504,505]`, broad **steps** `[507,508,509,510]` (verified 4-run), hedges 496–497/506/511.

## Zone 8 — Town buildings II: civic strip (rows 32–40, tiles 512–655)
- **513–514 + 529–530: "HOTEL"** brick corner (word starts "HO…"); long white hotel body `[515,516,517]`, `[519,520]`, `[522,523]`, `[524,540]`, `[525,526]`.
- **535–537: "HOSPITAL"** lettering (verified 3-run) on the white hospital block spanning 518–543 — entrance 536 below at 552-area, planters/shrubs 534/538–539.
- **544–546:** yellow-block **MALL-side** wall (verified 3-run); "MALL" text appears at 629 (`629` carries "L") and 464-area sign 448.
- **548–553: "PIZZA"** parlor — orange brick `[549,550,551]`, green-striped awning + PIZZA text 551–553.
- **562–563: "GAME"** (arcade) lettering on yellow brick.
- **577–578: "POLICE"** station front; **579–580: "BAKERY"** with green awning.
- **595–597:** large white house (verified 3-run); **598–599** blue-awning shop; **601–603+618** dark-roof slab (verified 4-block); **605–606: "LIBRARY"** lettering (613=616 duplicate roof tiles), library building 619–622 with purple awning 620.
- **608–611:** dark-red brick slab roofs (verified `[610,611]`); **612–613, 586–587, 592–594:** purple-roof houses (verified `[592,593,594]`, `[595,596,597]`).
- **624–633:** gray institutional building — windows `[625,626]`, `[627,628]`, façade `[630,631]`, steps+door `[632,633]`.
- **634–636:** yellow wall + timber posts; **638–641:** big **red-striped roof** (verified `[638,639]`, `[640,641]`); **642–644:** orange clapboard cottage (verified `[642,643]`).
- **645–649:** transition strip — log fence on sand, sand path pieces (verified `[648,649]`) leading toward the beach zone.
- **650–651:** cliff-with-waterfall pieces; **652–655:** dark dense trees (verified `[654,655]`).

## Zone 9 — Ocean & islands (rows 41–47, tiles 656–767)
- **656–661:** dark forest canopy wall (verified `[656,657]`, `[658,659,660]`) — the treeline behind the coast.
- **662–698, 703–762:** **ocean autotile** — deep purple fill with sparkle variants; all `W/W+` edges, freely tileable. Wave-crest overlay tiles scattered through (e.g. 711 splash).
- **Island/shore formations (verified blocks):** `[665,666,682]` grass islet; the **10-tile island** `[667–669, 684–686, 700–703]` (grass top, cliff sides dropping into water); the **11-tile island** `[680,681,695–697,712–714,728–730]`; column `[688,704,720,736]` (4-tall cliff-into-water on the sheet's left edge); `[689–691, 705–708, 722]` 8-tile island; `[694,709,710]`, `[723,738–740,743,753–760]` — a **13-tile beach-cove formation**; `[725,726]`, `[731,732,747]`, `[733,749]`, `[741,742]`, `[745,746]`, `[750,751]`.
- **701/705:** small rock pools (dark ovals in water).
- **763–770:** tall **sea-cliff faces** (verified `[765,766,767]`, `[769,770]`) — the coastline's rock wall, `C1+` interiors mate with Zone 2 cliff tiles.

## Zone 10 — Beach, coast houses & odds (rows 48–52, tiles 768–840)
- **771–774:** beach **sand strip** (verified `[772,773]`) — `S` edges mate with Zone 3 paths.
- **775–778:** magenta/pink long flat-roof building (verified `[776,777,778]`) with chimney 775.
- **779–782:** purple-roof beach houses (verified `[779... ]`, pair `[781,782]` visible in Appendix B).
- **783–787:** white clapboard house (verified `[785,786]`) with door 786.
- **788–792:** flowers, picket fragment, stepping stones (verified `[791,792,808,809]` — stones leading to the shack at 808–809).
- **793–801:** coastal cliffs with a **second cave mouth** `[798,799]` (verified) and its base 800.
- **802–804:** **night-sky/starfield** tiles (unique — background filler, edges `W`-like purple but with stars; use only with each other).
- **805–807, 818–820, 824–827:** pavement/walkway corner pieces (verified `[806,807]`, `[821,837,838]`).
- **808–809:** wooden shack; **810, 822–823, 835, 839–840:** log-post fence pieces; **811–815:** cliff with **river/waterfall** (verified `[811,812]`, `[813,814,815]`) — the `V` light-water edges are unique to this corner and match each other.
- **816–817, 830–833, 836–838:** dark hedge/tree walls (verified `[816,817]`, `[828,829]`); **834:** flower patch.
- **841–847:** blank white — ignore.

---

# 4 · Cross-zone connection map (which zones plug into which)

```
GRASS (Z1) --G-- everything with a G edge (props, stamps' surroundings)
GRASS --C1G~-- CLIFF rims (Z2) -- C1+ -- cliff faces -- C1S~/C1K~ -- caves, path descents
GRASS --GS~-- PATH (Z3) --S-- BEACH SAND (Z10)
GRASS --GP2~-- ROAD curbs (Z5) --P2+-- road fill / diagonals
GRASS --GK~-- FOREST canopy (Z4) and dark hedges (Z10)
CLIFF --KW~/C1-water rims-- OCEAN (Z9) via the island blocks
OCEAN W --W-- all ocean fills; shorelines only through Z9's verified blocks
POND (Z2) is a closed set: its W edges match ocean W, but its shore tiles are its own
RIVER V (Z10, 811-815) only matches itself
Buildings/signs: stamps sit on G or P2 ground; no edge blending - surround with grass/road fills
```

**The one-sentence version:** grass is the glue; every other terrain reaches grass only through its own `~`-coded rim tiles, buildings are indivisible stamps placed on top, and the exact composition of every stamp is in Appendix B.

---
## Appendix A — Per-tile data (all 848 tiles)

Edge codes: `G` grass · `W` ocean · `V` shallow/river water · `S` sand/dirt · `C1/C2` cliff rock · `P/P2` pavement · `K` dark/black · `X` white/empty. Suffix `+` = mostly that class with pixel detail; `~` between two codes = a transition edge mixing both; those only match edges of the same mixed code. **Match rule: a tile's E edge fits any tile whose W edge has the same code; a tile's S edge fits any tile whose N edge has the same code.** `joins` lists pixel-verified seamless partners inside this tile's block.

| tile | r,c | category | N | E | S | W | joins |
|---|---|---|---|---|---|---|---|
| 0 | 0,0 | GRASS+VEG | GK~ | G | G | GK~ | — |
| 1 | 0,1 | GRASS+VEG | G+ | S+ | G | G | E→2 S→17 |
| 2 | 0,2 | PATH | G+ | G | GS~ | S+ | W→1 |
| 3 | 0,3 | GRASS | G | G | G | G | — |
| 4 | 0,4 | GRASS | G | G | G | G | — |
| 5 | 0,5 | GRASS+VEG | G | G | G | G | E→6 S→21 |
| 6 | 0,6 | GRASS | G | G | G | G | E→7 W→5 S→22 |
| 7 | 0,7 | GRASS | G | G | G | G | E→8 W→6 S→23 |
| 8 | 0,8 | GRASS | G | G | G | G | E→9 W→7 S→24 |
| 9 | 0,9 | GRASS+VEG | G | G | G | G | E→10 W→8 S→25 |
| 10 | 0,10 | GRASS | G | G | G | G | W→9 S→26 |
| 11 | 0,11 | GRASS | G | G | G | G | S→27 |
| 12 | 0,12 | GRASS+VEG | G | G | G | G | — |
| 13 | 0,13 | GRASS+VEG | G | G | G | G | S→29 |
| 14 | 0,14 | GRASS+VEG | G | G | G | G | — |
| 15 | 0,15 | GRASS | G | G | G | G | — |
| 16 | 1,0 | GRASS | G | G | G | G | — |
| 17 | 1,1 | GRASS | G | G | G | G | N→1 |
| 18 | 1,2 | GRASS | G | G | G | G | — |
| 19 | 1,3 | GRASS+VEG | G | G | G | G | E→20 S→35 |
| 20 | 1,4 | GRASS+VEG | G | G | G | G | E→21 W→19 S→36 |
| 21 | 1,5 | GRASS+VEG | G | G | G | G | E→22 W→20 N→5 |
| 22 | 1,6 | GRASS | G | G | G | G | E→23 W→21 S→38 N→6 |
| 23 | 1,7 | GRASS | G | G | G | G | E→24 W→22 S→39 N→7 |
| 24 | 1,8 | GRASS | G | G | G | G | E→25 W→23 N→8 |
| 25 | 1,9 | GRASS+VEG | G | G | G | G | E→26 W→24 N→9 |
| 26 | 1,10 | GRASS | G | G | G | G | W→25 S→42 N→10 |
| 27 | 1,11 | GRASS+VEG | G | G | G | G | N→11 |
| 28 | 1,12 | GRASS+VEG | G | G | G | G | — |
| 29 | 1,13 | GRASS+VEG | G | G | G+ | G+ | N→13 |
| 30 | 1,14 | GRASS+VEG | G | G | G+ | G+ | E→31 |
| 31 | 1,15 | GRASS+VEG | G | G | G+ | G | W→30 |
| 32 | 2,0 | GRASS+VEG | G+ | G | G | G | — |
| 33 | 2,1 | GRASS+VEG | G+ | G | G | G+ | — |
| 34 | 2,2 | GRASS+VEG | G+ | G | G | G | — |
| 35 | 2,3 | GRASS+VEG | G | G | G+ | G+ | E→36 N→19 |
| 36 | 2,4 | GRASS+VEG | G+ | G | G+ | G+ | W→35 N→20 |
| 37 | 2,5 | GRASS+VEG | G+ | G | G | G+ | — |
| 38 | 2,6 | GRASS+VEG | G | G | G | G+ | E→39 N→22 |
| 39 | 2,7 | GRASS+VEG | G | G | G+ | G+ | W→38 N→23 |
| 40 | 2,8 | GRASS+VEG | G | G | G+ | G+ | — |
| 41 | 2,9 | GRASS+VEG | G | G | GK~ | G+ | — |
| 42 | 2,10 | GRASS+VEG | G+ | G | G | G+ | N→26 |
| 43 | 2,11 | GRASS+VEG | G+ | G | G | G+ | — |
| 44 | 2,12 | GRASS+VEG | G+ | G | G | G+ | E→45 |
| 45 | 2,13 | GRASS+VEG | G | G | G | G+ | W→44 |
| 46 | 2,14 | GRASS+VEG | G | G | G | G+ | — |
| 47 | 2,15 | GRASS+VEG | P2+ | G | G+ | G+ | — |
| 48 | 3,0 | WATER | W+ | W+ | W | W | — |
| 49 | 3,1 | WATER | W+ | C1+ | G+ | W+ | S→65 |
| 50 | 3,2 | BLDG/OBJ | GW~ | G+ | G+ | W+ | S→66 |
| 51 | 3,3 | BLDG/OBJ | KW~ | G | G | W+ | — |
| 52 | 3,4 | WATER | W | W+ | GW~ | W | — |
| 53 | 3,5 | WATER | W | W+ | W+ | W | — |
| 54 | 3,6 | BLDG/OBJ | KW~ | W+ | G | G | — |
| 55 | 3,7 | WATER | W+ | W+ | G+ | C1+ | E→56 S→71 |
| 56 | 3,8 | WATER | W | W | W+ | W+ | W→55 |
| 57 | 3,9 | WATER | W | W+ | G | W+ | E→58 |
| 58 | 3,10 | WATER | W | W+ | G | W+ | W→57 |
| 59 | 3,11 | GRASS+VEG | W | G+ | G | G+ | S→75 |
| 60 | 3,12 | TREES | W | G+ | GK~ | KW~ | — |
| 61 | 3,13 | BLDG/OBJ | W | KW~ | C1+ | K+ | — |
| 62 | 3,14 | CLIFF | C1+ | C1+ | C1V~ | K+ | — |
| 63 | 3,15 | GRASS+OBJ | GK~ | G+ | G | G+ | — |
| 64 | 4,0 | GRASS+VEG | G+ | C1+ | G+ | G+ | — |
| 65 | 4,1 | GRASS+VEG | G+ | G | G+ | C1K~ | N→49 |
| 66 | 4,2 | GRASS | G | G+ | G+ | G | N→50 |
| 67 | 4,3 | GRASS+OBJ | G | G+ | C1K~ | GK~ | E→68 |
| 68 | 4,4 | GRASS+OBJ | C1+ | G+ | G | G | W→67 |
| 69 | 4,5 | GRASS+VEG | G | G | G+ | C1G~ | S→85 |
| 70 | 4,6 | GRASS+OBJ | G+ | G+ | G | G | E→71 |
| 71 | 4,7 | GRASS+VEG | G | G+ | G+ | G+ | W→70 N→55 |
| 72 | 4,8 | CLIFF | C1+ | GK~ | G+ | C1+ | E→73 |
| 73 | 4,9 | CLIFF | C1+ | C1+ | G+ | GK~ | W→72 |
| 74 | 4,10 | GRASS+VEG | C1+ | G | G | G+ | — |
| 75 | 4,11 | CLIFF | G | C1G~ | C1+ | GK~ | N→59 |
| 76 | 4,12 | BLDG/OBJ | G | G | G+ | C1+ | — |
| 77 | 4,13 | BLDG/OBJ | G+ | C1G~ | C1+ | K+ | E→78 |
| 78 | 4,14 | CLIFF | G | C1S~ | C1+ | C1G~ | W→77 |
| 79 | 4,15 | GRASS+VEG | G | G | G+ | C1G~ | — |
| 80 | 5,0 | CLIFF | C1+ | C1+ | G+ | G+ | — |
| 81 | 5,1 | BLDG/OBJ | C1+ | G+ | C1K~ | C1K~ | E→82 |
| 82 | 5,2 | GRASS | G+ | G | G | G+ | W→81 |
| 83 | 5,3 | BLDG/OBJ | G | G+ | C1+ | C1K~ | — |
| 84 | 5,4 | CLIFF | C1+ | C1G~ | GK~ | C1G~ | — |
| 85 | 5,5 | GRASS+VEG | G | C1G~ | G+ | G | N→69 |
| 86 | 5,6 | BLDG/OBJ | G | G | G+ | C1S~ | — |
| 87 | 5,7 | GRASS+OBJ | G+ | G | G | G+ | — |
| 88 | 5,8 | BLDG/OBJ | G+ | G+ | G+ | G+ | — |
| 89 | 5,9 | GRASS+VEG | G | G | G+ | C1G~ | S→105 |
| 90 | 5,10 | GRASS+VEG | G | G | G+ | G+ | S→106 |
| 91 | 5,11 | CLIFF | G+ | C1G~ | C1+ | K+ | — |
| 92 | 5,12 | GRASS+VEG | G | C1G~ | G+ | G | — |
| 93 | 5,13 | CLIFF | G | C1G~ | C1+ | C1S~ | E→94 S→109 |
| 94 | 5,14 | CLIFF | G | GK~ | C1+ | C1G~ | W→93 |
| 95 | 5,15 | GRASS+VEG | G+ | G+ | G | G | — |
| 96 | 6,0 | BLDG/OBJ | C1+ | C1K~ | C1K~ | G+ | — |
| 97 | 6,1 | CLIFF | C1+ | C1K~ | C1+ | C1S~ | E→98 |
| 98 | 6,2 | CLIFF | C1+ | C1S~ | C1+ | C1K~ | W→97 |
| 99 | 6,3 | GRASS+VEG | G+ | G | G+ | C1K~ | — |
| 100 | 6,4 | BLDG/OBJ | G+ | K+ | C1K~ | G+ | — |
| 101 | 6,5 | CLIFF | G+ | K+ | C1+ | C1G~ | — |
| 102 | 6,6 | CLIFF | C1+ | C1K~ | C1+ | C1S~ | E→103 |
| 103 | 6,7 | CLIFF | C1+ | C1+ | C1+ | C1K~ | E→104 W→102 |
| 104 | 6,8 | CLIFF | C1+ | GK~ | G+ | C1+ | W→103 S→120 |
| 105 | 6,9 | GRASS+VEG | G | C1G~ | G+ | G | N→89 |
| 106 | 6,10 | GRASS+VEG | G | G+ | G+ | G | E→107 N→90 |
| 107 | 6,11 | GRASS+VEG | G | G+ | G | G+ | E→108 W→106 |
| 108 | 6,12 | GRASS+VEG | G+ | C1K~ | G+ | G | W→107 |
| 109 | 6,13 | CLIFF | C1+ | GK~ | C1G~ | C1+ | N→93 |
| 110 | 6,14 | GRASS+VEG | G | G+ | G+ | G+ | — |
| 111 | 6,15 | GRASS+VEG | C1K~ | G+ | G | G+ | — |
| 112 | 7,0 | BLDG/OBJ | G+ | K+ | C1K~ | G+ | — |
| 113 | 7,1 | BLDG/OBJ | G | G+ | G | C1G~ | — |
| 114 | 7,2 | CLIFF | G | C1G~ | C1+ | C1G~ | — |
| 115 | 7,3 | GRASS+VEG | G | C1G~ | G+ | G | — |
| 116 | 7,4 | GRASS+VEG | G+ | C1K~ | G+ | G | — |
| 117 | 7,5 | CLIFF | C1+ | K+ | C1G~ | C1+ | — |
| 118 | 7,6 | GRASS+VEG | C1G~ | G | G | C1G~ | — |
| 119 | 7,7 | BLDG/OBJ | G | C1G~ | G | G+ | — |
| 120 | 7,8 | BLDG/OBJ | G+ | G+ | C1K~ | K+ | N→104 |
| 121 | 7,9 | CLIFF | C1+ | C1K~ | C1+ | C1K~ | — |
| 122 | 7,10 | BLDG/OBJ | G+ | G+ | C1K~ | K+ | E→123 |
| 123 | 7,11 | GRASS+VEG | C1+ | G | G | G | E→124 W→122 |
| 124 | 7,12 | GRASS+VEG | G | G+ | G | G+ | W→123 |
| 125 | 7,13 | CLIFF | C1+ | C1+ | C1G~ | K+ | — |
| 126 | 7,14 | GRASS+VEG | C1G~ | C1G~ | G | G | — |
| 127 | 7,15 | GRASS+VEG | G+ | G | G+ | C1K~ | — |
| 128 | 8,0 | CLIFF | C1+ | C1+ | C1G~ | GK~ | — |
| 129 | 8,1 | BLDG/OBJ | C1+ | G+ | GK~ | GK~ | E→130 |
| 130 | 8,2 | BLDG/OBJ | G+ | K+ | C1G~ | G | W→129 |
| 131 | 8,3 | BLDG/OBJ | C1G~ | G+ | C1G~ | G | — |
| 132 | 8,4 | GRASS+OBJ | C1G~ | G+ | G | G+ | E→133 |
| 133 | 8,5 | GRASS+VEG | C1G~ | G+ | G | G | W→132 |
| 134 | 8,6 | GRASS+VEG | G+ | G | G | G+ | — |
| 135 | 8,7 | CLIFF | C1+ | C1+ | G+ | GK~ | — |
| 136 | 8,8 | BLDG/OBJ | G | C1+ | G+ | G | — |
| 137 | 8,9 | CLIFF | C1+ | C1G~ | G | C1G~ | — |
| 138 | 8,10 | BLDG/OBJ | G+ | K+ | C1+ | C1G~ | — |
| 139 | 8,11 | BLDG/OBJ | G | C1G~ | C1+ | C1G~ | — |
| 140 | 8,12 | CLIFF | C1+ | G+ | G | C1+ | S→156 |
| 141 | 8,13 | GRASS+VEG | G+ | S+ | G | G | E→142 S→157 |
| 142 | 8,14 | PATH | G+ | G+ | S+ | S+ | E→143 W→141 S→158 |
| 143 | 8,15 | GRASS+VEG | C1S~ | S+ | G | G | W→142 S→159 |
| 144 | 9,0 | PATH | G+ | G+ | S+ | S+ | E→145 |
| 145 | 9,1 | GRASS+VEG | G | S+ | G+ | G | E→146 W→144 |
| 146 | 9,2 | PATH | GS~ | G | G+ | S+ | W→145 |
| 147 | 9,3 | PATH | G | GS~ | S+ | GS~ | S→163 |
| 148 | 9,4 | GRASS+VEG | G | G+ | S+ | G+ | — |
| 149 | 9,5 | GRASS+VEG | G | GS~ | G | G | E→150 |
| 150 | 9,6 | PATH | G | GS~ | G | GS~ | E→151 W→149 |
| 151 | 9,7 | PATH | G+ | GS~ | G | GS~ | E→152 W→150 |
| 152 | 9,8 | PATH | G+ | GS~ | G | GS~ | W→151 |
| 153 | 9,9 | GRASS+OBJ | G+ | G | G | S+ | — |
| 154 | 9,10 | GRASS+OBJ | G+ | S+ | G | G | E→155 |
| 155 | 9,11 | PATH | C1S~ | GS~ | G | GS~ | E→156 W→154 |
| 156 | 9,12 | PATH | G | G+ | S+ | GS~ | W→155 N→140 |
| 157 | 9,13 | PATH | G | GS~ | G+ | GS~ | E→158 N→141 |
| 158 | 9,14 | PATH | G | GS~ | G+ | GS~ | E→159 W→157 N→142 |
| 159 | 9,15 | PATH | G+ | S+ | S+ | G+ | W→158 N→143 |
| 160 | 10,0 | GRASS+VEG | C1S~ | G | G | S+ | — |
| 161 | 10,1 | CLIFF | G | GK~ | C1+ | GK~ | — |
| 162 | 10,2 | PATH | G+ | GS~ | S+ | G+ | E→163 |
| 163 | 10,3 | GRASS+VEG | C1S~ | G | G | GS~ | W→162 N→147 |
| 164 | 10,4 | BLDG/OBJ | G | G | G | GS~ | — |
| 165 | 10,5 | PATH | G | GS~ | S+ | G+ | — |
| 166 | 10,6 | PATH | G+ | S+ | S+ | G+ | E→167 |
| 167 | 10,7 | GRASS+VEG | G+ | G | G | S+ | W→166 S→183 |
| 168 | 10,8 | PATH | G+ | GS~ | G+ | GS~ | — |
| 169 | 10,9 | PATH | G+ | G | GS~ | S+ | — |
| 170 | 10,10 | PATH | GS~ | G | GS~ | S+ | — |
| 171 | 10,11 | PATH | C1S~ | G | G+ | S+ | — |
| 172 | 10,12 | PATH | GS~ | G | G+ | S+ | — |
| 173 | 10,13 | GRASS+OBJ | G | G | G | G | E→174 |
| 174 | 10,14 | GRASS+VEG | G | G+ | G | G | E→175 W→173 |
| 175 | 10,15 | BLDG/OBJ | G+ | C1G~ | G | C1G~ | W→174 |
| 176 | 11,0 | CLIFF | G+ | G | C1G~ | C1G~ | E→177 |
| 177 | 11,1 | GRASS+VEG | C1G~ | G | G | G | E→178 W→176 |
| 178 | 11,2 | GRASS+VEG | G | C1G~ | G | G | E→179 W→177 |
| 179 | 11,3 | GRASS+VEG | G | G | G | C1G~ | W→178 |
| 180 | 11,4 | GRASS+OBJ | GK~ | GK~ | G | G | — |
| 181 | 11,5 | TREES | G | G+ | GK~ | G+ | E→182 |
| 182 | 11,6 | TREES | G | G | GK~ | G+ | W→181 |
| 183 | 11,7 | TREES | G | G+ | G+ | G+ | E→184 N→167 |
| 184 | 11,8 | TREES | G | G+ | G+ | G+ | E→185 W→183 |
| 185 | 11,9 | TREES | G | G+ | G+ | G+ | W→184 |
| 186 | 11,10 | CLIFF | C1+ | GK~ | G+ | C1G~ | — |
| 187 | 11,11 | CLIFF | C1+ | C1S~ | GK~ | GK~ | — |
| 188 | 11,12 | TREES | G+ | G+ | G+ | G+ | E→189 |
| 189 | 11,13 | TREES | GK~ | G+ | GK~ | G+ | E→190 W→188 |
| 190 | 11,14 | TREES | GK~ | G+ | K+ | G+ | E→191 W→189 |
| 191 | 11,15 | TREES | G+ | G+ | G+ | G+ | W→190 |
| 192 | 12,0 | TREES | K+ | C1K~ | GK~ | G+ | S→208 |
| 193 | 12,1 | TREES | GK~ | G+ | G+ | C1K~ | — |
| 194 | 12,2 | TREES | G+ | G+ | G+ | G+ | — |
| 195 | 12,3 | TREES | G+ | G+ | GK~ | G+ | — |
| 196 | 12,4 | TREES | G+ | G+ | G+ | G+ | — |
| 197 | 12,5 | TREES | G | G+ | GK~ | G+ | E→198 |
| 198 | 12,6 | TREES | GK~ | G+ | K+ | G+ | W→197 |
| 199 | 12,7 | TREES | GK~ | G+ | GK~ | G+ | E→200 |
| 200 | 12,8 | GRASS+VEG | G | G | G+ | G+ | W→199 |
| 201 | 12,9 | TREES | GK~ | G+ | G+ | K+ | E→202 |
| 202 | 12,10 | TREES | K+ | C1K~ | K+ | G+ | W→201 |
| 203 | 12,11 | GRASS+VEG | GK~ | G+ | G+ | C1K~ | — |
| 204 | 12,12 | TREES | GK~ | G | GK~ | G+ | — |
| 205 | 12,13 | GRASS+VEG | G+ | GS~ | G+ | G+ | — |
| 206 | 12,14 | TREES | GK~ | GS~ | GK~ | G+ | — |
| 207 | 12,15 | PATH | GS~ | G+ | G+ | GS~ | — |
| 208 | 13,0 | TREES | GK~ | G+ | GK~ | G+ | S→224 N→192 |
| 209 | 13,1 | TREES | GK~ | G+ | K+ | G+ | E→210 |
| 210 | 13,2 | TREES | K+ | GK~ | GK~ | G+ | W→209 |
| 211 | 13,3 | TREES | GK~ | C1K~ | G | GK~ | — |
| 212 | 13,4 | GRASS+OBJ | GK~ | G | G | C1K~ | — |
| 213 | 13,5 | CLIFF | C1+ | C1S~ | GK~ | C1K~ | — |
| 214 | 13,6 | CLIFF | C1+ | C1+ | G+ | C1K~ | E→215 |
| 215 | 13,7 | CLIFF | C1+ | C1K~ | C1K~ | C1S~ | W→214 |
| 216 | 13,8 | TREES | C1K~ | G+ | K+ | G+ | E→217 |
| 217 | 13,9 | TREES | K+ | GK~ | GK~ | G+ | W→216 |
| 218 | 13,10 | GRASS+VEG | G | G+ | G+ | G | E→219 |
| 219 | 13,11 | GRASS+VEG | G | G+ | G+ | G+ | E→220 W→218 |
| 220 | 13,12 | GRASS+VEG | G | G+ | G+ | G+ | E→221 W→219 |
| 221 | 13,13 | GRASS+VEG | G | G | GK~ | G+ | W→220 |
| 222 | 13,14 | TREES | GK~ | G+ | K+ | G+ | E→223 |
| 223 | 13,15 | TREES | G+ | G+ | G+ | G+ | W→222 |
| 224 | 14,0 | TREES | G+ | G+ | GK~ | C1K~ | N→208 |
| 225 | 14,1 | TREES | G+ | C1K~ | G | G | — |
| 226 | 14,2 | GRASS+VEG | G | G+ | G+ | G+ | — |
| 227 | 14,3 | TREES | GK~ | G+ | GK~ | G+ | — |
| 228 | 14,4 | TREES | G+ | G+ | G+ | GK~ | — |
| 229 | 14,5 | TREES | K+ | GK~ | G | C1K~ | — |
| 230 | 14,6 | TREES | GK~ | G+ | G+ | C1K~ | — |
| 231 | 14,7 | TREES | G | G+ | GK~ | G+ | E→232 |
| 232 | 14,8 | TREES | G+ | G+ | GK~ | G+ | W→231 |
| 233 | 14,9 | GRASS+VEG | G | G | G+ | G | — |
| 234 | 14,10 | TREES | G+ | G+ | G+ | G+ | S→250 |
| 235 | 14,11 | CLIFF | C1+ | C1K~ | C1G~ | C1+ | — |
| 236 | 14,12 | CLIFF | C1+ | C1K~ | G+ | C1+ | — |
| 237 | 14,13 | TREES | C1+ | G+ | GK~ | G+ | E→238 S→253 |
| 238 | 14,14 | TREES | GK~ | G+ | G+ | G+ | W→237 S→254 |
| 239 | 14,15 | BLDG/OBJ | GK~ | C1K~ | C1K~ | G+ | — |
| 240 | 15,0 | TREES | C1+ | G+ | GK~ | C1K~ | — |
| 241 | 15,1 | CLIFF | C1+ | C1K~ | G+ | C1+ | — |
| 242 | 15,2 | BLDG/OBJ | GK~ | GK~ | G | C1G~ | — |
| 243 | 15,3 | GRASS+VEG | GK~ | G | G | GK~ | E→244 S→259 |
| 244 | 15,4 | GRASS+VEG | G | G+ | G+ | G | W→243 |
| 245 | 15,5 | TREES | G+ | C1K~ | G+ | GK~ | — |
| 246 | 15,6 | TREES | G | G+ | G+ | G+ | — |
| 247 | 15,7 | GRASS+VEG | G | G | G | G+ | — |
| 248 | 15,8 | GRASS+VEG | G | G+ | G+ | G | E→249 |
| 249 | 15,9 | GRASS+VEG | G | G+ | G+ | G+ | E→250 W→248 |
| 250 | 15,10 | GRASS+VEG | G | G+ | G+ | G+ | E→251 W→249 N→234 |
| 251 | 15,11 | GRASS+VEG | G | G+ | G+ | G+ | E→252 W→250 |
| 252 | 15,12 | GRASS+VEG | G | G+ | G+ | G+ | E→253 W→251 |
| 253 | 15,13 | GRASS+VEG | K+ | G+ | GK~ | G+ | E→254 W→252 N→237 |
| 254 | 15,14 | GRASS+VEG | G | G+ | G+ | G | E→255 W→253 N→238 |
| 255 | 15,15 | GRASS+VEG | G | G+ | G+ | G+ | W→254 |
| 256 | 16,0 | GRASS+OBJ | GK~ | GK~ | G | G | — |
| 257 | 16,1 | GRASS+VEG | G | G+ | G+ | G+ | — |
| 258 | 16,2 | GRASS | G | G | GK~ | G+ | — |
| 259 | 16,3 | TREES | G | G+ | G+ | G+ | N→243 |
| 260 | 16,4 | TREES | G+ | G+ | G+ | G+ | E→261 |
| 261 | 16,5 | GRASS+VEG | G | G+ | G+ | G | W→260 |
| 262 | 16,6 | TREES | G+ | G+ | G+ | G+ | — |
| 263 | 16,7 | TREES | G+ | G+ | G+ | G+ | — |
| 264 | 16,8 | BLDG/OBJ | G | GP2~ | P2+ | GP2~ | E→265 |
| 265 | 16,9 | BLDG/OBJ | G | GP2~ | P2+ | GP2~ | W→264 S→281 |
| 266 | 16,10 | BLDG/OBJ | K+ | GP2~ | G | GP2~ | — |
| 267 | 16,11 | ROAD/BLDG | K | P+ | P | P+ | E→268 |
| 268 | 16,12 | ROAD/BLDG | K+ | P2+ | PP2~ | P+ | W→267 |
| 269 | 16,13 | ROAD/BLDG | P | P+ | P | P+ | — |
| 270 | 16,14 | GRASS+VEG | G | G | G | GP2~ | — |
| 271 | 16,15 | BLDG/OBJ | P2+ | G | G | GP2~ | — |
| 272 | 17,0 | ROAD/BLDG | K | P+ | P+ | P+ | E→273 S→288 |
| 273 | 17,1 | ROAD/BLDG | K | P+ | P | P+ | W→272 S→289 |
| 274 | 17,2 | BLDG/OBJ | G | GP2~ | P2+ | GP2~ | — |
| 275 | 17,3 | BLDG/OBJ | KP2~ | P2+ | P+ | G+ | — |
| 276 | 17,4 | BLDG/OBJ | G | GP2~ | P2+ | GK~ | — |
| 277 | 17,5 | BLDG/OBJ | G+ | P2+ | P+ | G+ | — |
| 278 | 17,6 | TREES | K | GP2~ | G+ | GP2~ | — |
| 279 | 17,7 | BLDG/OBJ | K | GP2~ | G | GP2~ | — |
| 280 | 17,8 | ROAD/BLDG | K | GP2~ | G | P2+ | — |
| 281 | 17,9 | ROAD/BLDG | P2+ | P+ | P+ | P2+ | S→297 N→265 |
| 282 | 17,10 | BLDG/OBJ | G | GP2~ | P2+ | GP2~ | — |
| 283 | 17,11 | ROAD/BLDG | P2+ | P+ | P+ | KP2~ | E→284 |
| 284 | 17,12 | ROAD/BLDG | K | P+ | P+ | P+ | E→285 W→283 S→300 |
| 285 | 17,13 | ROAD/BLDG | K | P+ | P+ | P+ | E→286 W→284 S→301 |
| 286 | 17,14 | ROAD/BLDG | K | P+ | P | P+ | W→285 S→302 |
| 287 | 17,15 | ROAD/BLDG | K | P2+ | P+ | GP2~ | — |
| 288 | 18,0 | ROAD/BLDG | K+ | P+ | P+ | P2+ | E→289 N→272 |
| 289 | 18,1 | ROAD/BLDG | P | P+ | P | P+ | E→290 W→288 N→273 |
| 290 | 18,2 | ROAD/BLDG | P+ | P2+ | PP2~ | P+ | W→289 |
| 291 | 18,3 | ROAD/BLDG | K+ | P+ | P | P | S→307 |
| 292 | 18,4 | BLDG/OBJ | G+ | P2+ | P+ | G+ | E→293 |
| 293 | 18,5 | ROAD/BLDG | P2+ | P+ | P+ | P2+ | E→294 W→292 |
| 294 | 18,6 | ROAD/BLDG | P+ | P | P | P+ | E→295 W→293 |
| 295 | 18,7 | ROAD/BLDG | P+ | P2+ | PP2~ | P+ | E→296 W→294 |
| 296 | 18,8 | BLDG/OBJ | P2+ | G | G | P2+ | W→295 |
| 297 | 18,9 | ROAD/BLDG | P | P+ | P | P+ | E→298 N→281 |
| 298 | 18,10 | ROAD/BLDG | P+ | P2+ | P2+ | P+ | E→299 W→297 |
| 299 | 18,11 | ROAD/BLDG | P2+ | GP2~ | P2+ | P2+ | W→298 |
| 300 | 18,12 | ROAD/BLDG | P+ | P | P | P+ | E→301 N→284 |
| 301 | 18,13 | ROAD/BLDG | P | P | P | P | E→302 W→300 N→285 |
| 302 | 18,14 | ROAD/BLDG | P+ | P+ | P | P+ | W→301 N→286 |
| 303 | 18,15 | BLDG/OBJ | G | GP2~ | P2+ | GP2~ | — |
| 304 | 19,0 | ROAD/BLDG | K+ | P+ | P | P | E→305 |
| 305 | 19,1 | ROAD/BLDG | K | P+ | P+ | P+ | E→306 W→304 |
| 306 | 19,2 | ROAD/BLDG | P+ | P | P | P+ | E→307 W→305 |
| 307 | 19,3 | ROAD/BLDG | P | P | P | P | W→306 N→291 |
| 308 | 19,4 | GRASS+VEG | G+ | GP2~ | G | G | E→309 |
| 309 | 19,5 | BLDG/OBJ | P2 | GP2~ | G | GP2~ | W→308 |
| 310 | 19,6 | BLDG/OBJ | G | GP2~ | P2+ | GK~ | E→311 |
| 311 | 19,7 | BLDG/OBJ | G+ | P2+ | P2+ | GP2~ | W→310 |
| 312 | 19,8 | ROAD/BLDG | K | P+ | P+ | P+ | — |
| 313 | 19,9 | ROAD/BLDG | P2+ | P+ | P+ | P2+ | E→314 |
| 314 | 19,10 | ROAD/BLDG | P+ | P+ | P | P+ | W→313 |
| 315 | 19,11 | BLDG/OBJ | G | GP2~ | P2+ | GP2~ | — |
| 316 | 19,12 | ROAD/BLDG | P2+ | P+ | P+ | KP2~ | E→317 |
| 317 | 19,13 | ROAD/BLDG | K | P+ | P | P+ | E→318 W→316 |
| 318 | 19,14 | ROAD/BLDG | K | P+ | P | P+ | W→317 |
| 319 | 19,15 | GRASS+VEG | K+ | G+ | G | G+ | — |
| 320 | 20,0 | GRASS+VEG | G | K+ | GK~ | G | — |
| 321 | 20,1 | GRASS+VEG | G+ | GK~ | G | G+ | E→322 S→337 |
| 322 | 20,2 | BLDG/OBJ | G | GK~ | GK~ | GK~ | W→321 |
| 323 | 20,3 | BLDG/OBJ | G+ | GK~ | K+ | GK~ | — |
| 324 | 20,4 | GRASS+VEG | G+ | G+ | G+ | G+ | E→325 |
| 325 | 20,5 | GRASS+VEG | G | G+ | GK~ | G | W→324 |
| 326 | 20,6 | GRASS+VEG | G | GK~ | GK~ | G | — |
| 327 | 20,7 | GRASS+VEG | G | G+ | G+ | G+ | — |
| 328 | 20,8 | GRASS+VEG | G+ | G | G | G+ | — |
| 329 | 20,9 | GRASS+VEG | G+ | GK~ | G | G | S→345 |
| 330 | 20,10 | GRASS+OBJ | G+ | GK~ | G | G | — |
| 331 | 20,11 | GRASS+OBJ | G | GK~ | G | G+ | S→347 |
| 332 | 20,12 | GRASS+VEG | G | G+ | C1G~ | G | — |
| 333 | 20,13 | GRASS+VEG | G | K+ | GK~ | G | — |
| 334 | 20,14 | BLDG/OBJ | G+ | GK~ | GK~ | GK~ | — |
| 335 | 20,15 | GRASS+VEG | G+ | G+ | G+ | G+ | — |
| 336 | 21,0 | BLDG/OBJ | G | GK~ | C1G~ | GK~ | — |
| 337 | 21,1 | GRASS+VEG | G | GK~ | GK~ | G | N→321 |
| 338 | 21,2 | GRASS+VEG | G | G+ | G+ | G+ | — |
| 339 | 21,3 | GRASS+VEG | G+ | G | G | G+ | — |
| 340 | 21,4 | CLIFF | G+ | C1G~ | G+ | K+ | — |
| 341 | 21,5 | GRASS+OBJ | G | C1+ | C1G~ | G | — |
| 342 | 21,6 | CLIFF | C1K~ | G+ | G+ | K+ | — |
| 343 | 21,7 | CLIFF | GK~ | K+ | C1+ | K+ | E→344 |
| 344 | 21,8 | CLIFF | C1K~ | G+ | G+ | K+ | W→343 |
| 345 | 21,9 | BLDG/OBJ | G+ | C1+ | G+ | G+ | N→329 |
| 346 | 21,10 | BLDG/OBJ | G+ | G | G+ | K+ | — |
| 347 | 21,11 | CLIFF | G+ | C1+ | GK~ | K+ | N→331 |
| 348 | 21,12 | BLDG/OBJ | K | K+ | C1+ | C1K~ | — |
| 349 | 21,13 | BLDG/OBJ | GK~ | G | C1G~ | C1K~ | E→350 |
| 350 | 21,14 | GRASS+OBJ | G | K+ | GK~ | G+ | W→349 |
| 351 | 21,15 | BLDG/OBJ | GK~ | G+ | G+ | GK~ | — |
| 352 | 22,0 | ROAD/BLDG | P2+ | GK~ | C1G~ | P2+ | — |
| 353 | 22,1 | BLDG/OBJ | K+ | K+ | C1+ | G+ | — |
| 354 | 22,2 | GRASS+VEG | P2+ | G+ | G | G+ | — |
| 355 | 22,3 | BLDG/OBJ | K+ | GP2~ | G | G | — |
| 356 | 22,4 | BLDG/OBJ | K+ | GP2~ | G | G | E→357 |
| 357 | 22,5 | GRASS+OBJ | GK~ | G | G | GP2~ | W→356 |
| 358 | 22,6 | GRASS+VEG | G | G+ | P2+ | G+ | E→359 |
| 359 | 22,7 | GRASS+VEG | G | G+ | P2+ | G+ | W→358 |
| 360 | 22,8 | BLDG/OBJ | P2+ | C1P2~ | G | K+ | — |
| 361 | 22,9 | BLDG/OBJ | KW~ | K+ | G | P2W~ | — |
| 362 | 22,10 | ROAD/BLDG | K+ | P2+ | P2+ | K+ | E→363 |
| 363 | 22,11 | ROAD/BLDG | K+ | K+ | KP2~ | P2+ | W→362 |
| 364 | 22,12 | BLDG/OBJ | P2+ | GP2~ | G | GK~ | S→380 |
| 365 | 22,13 | BLDG/OBJ | K+ | K+ | G | K+ | — |
| 366 | 22,14 | GRASS+OBJ | GK~ | G | G | GP2~ | — |
| 367 | 22,15 | BLDG/OBJ | K+ | GP2~ | G | G | — |
| 368 | 23,0 | BLDG/OBJ | K+ | KP2~ | G | K+ | E→369 |
| 369 | 23,1 | BLDG/OBJ | K+ | K+ | G | KP2~ | W→368 |
| 370 | 23,2 | BLDG/OBJ | K+ | GP2~ | G | G | — |
| 371 | 23,3 | ROAD/BLDG | K+ | P2+ | P2+ | K+ | E→372 |
| 372 | 23,4 | ROAD/BLDG | K+ | K+ | KP2~ | P2+ | W→371 |
| 373 | 23,5 | BLDG/OBJ | G | GP2~ | G | G+ | — |
| 374 | 23,6 | BLDG/OBJ | G | G+ | G | GP2~ | — |
| 375 | 23,7 | BLDG/OBJ | G+ | K+ | C1K~ | G+ | — |
| 376 | 23,8 | BLDG/OBJ | G+ | G | GK~ | K+ | — |
| 377 | 23,9 | GRASS+VEG | G+ | C1C2~ | G+ | G | — |
| 378 | 23,10 | BLDG/OBJ | C1C2~ | C1C2~ | C1K~ | C2+ | — |
| 379 | 23,11 | BLDG/OBJ | GK~ | G | G+ | C1C2~ | — |
| 380 | 23,12 | BLDG/OBJ | G+ | K+ | C1+ | G+ | E→381 N→364 |
| 381 | 23,13 | TREES | G+ | G | GK~ | K+ | W→380 |
| 382 | 23,14 | CLIFF | C1+ | C1K~ | GP2~ | K+ | — |
| 383 | 23,15 | BLDG/OBJ | K+ | K+ | P2+ | KW~ | — |
| 384 | 24,0 | GRASS+VEG | G | G+ | K+ | G+ | — |
| 385 | 24,1 | TREES | KS~ | GK~ | G+ | G+ | — |
| 386 | 24,2 | ROAD/BLDG | GK~ | C1P2~ | P+ | GK~ | — |
| 387 | 24,3 | ROAD/BLDG | K | C1P2~ | P+ | P2+ | E→388 |
| 388 | 24,4 | ROAD/BLDG | K | C1P2~ | P+ | P2+ | E→389 W→387 |
| 389 | 24,5 | ROAD/BLDG | K | C1P2~ | P+ | P2+ | W→388 |
| 390 | 24,6 | CLIFF | K | K | C1P~ | P2+ | E→391 |
| 391 | 24,7 | ROAD/BLDG | C1+ | C1+ | P+ | K | W→390 |
| 392 | 24,8 | ROAD/BLDG | C1+ | C1+ | P+ | PP2~ | — |
| 393 | 24,9 | CLIFF | C1+ | K | C1P~ | PP2~ | — |
| 394 | 24,10 | ROAD/BLDG | C1+ | C1P~ | GK~ | PP2~ | — |
| 395 | 24,11 | ROAD/BLDG | C1K~ | C1P2~ | C1P~ | C1K~ | — |
| 396 | 24,12 | ROAD/BLDG | K | C1P2~ | P+ | P2+ | — |
| 397 | 24,13 | GRASS+VEG | P2+ | G+ | G | G+ | E→398 |
| 398 | 24,14 | TREES | G+ | G+ | G | G+ | W→397 |
| 399 | 24,15 | ROAD/BLDG | C1 | KP~ | KP~ | PP2~ | — |
| 400 | 25,0 | ROAD/BLDG | C1+ | K+ | P+ | K+ | — |
| 401 | 25,1 | CLIFF | C1+ | K+ | C1P~ | PP2~ | — |
| 402 | 25,2 | GRASS+VEG | KP2~ | G+ | G | G+ | E→403 |
| 403 | 25,3 | GRASS+VEG | P2+ | G+ | G | G+ | E→404 W→402 |
| 404 | 25,4 | GRASS+VEG | GP2~ | G | G | G+ | E→405 W→403 |
| 405 | 25,5 | WATER | K+ | W+ | K | G+ | E→406 W→404 |
| 406 | 25,6 | WATER | K | W+ | K | W+ | E→407 W→405 |
| 407 | 25,7 | BLDG/OBJ | GK~ | G | GK~ | W+ | W→406 |
| 408 | 25,8 | BLDG/OBJ | GK~ | C1P2~ | C1+ | GK~ | — |
| 409 | 25,9 | CLIFF | C1+ | C1+ | C1+ | K | — |
| 410 | 25,10 | CLIFF | C1+ | C1K~ | C1K~ | K | — |
| 411 | 25,11 | BLDG/OBJ | GK~ | C1P2~ | S+ | GK~ | — |
| 412 | 25,12 | BLDG/OBJ | K | C1P2~ | S+ | C2P2~ | E→413 |
| 413 | 25,13 | CLIFF | K | K | C1S~ | C2P2~ | E→414 W→412 |
| 414 | 25,14 | BLDG/OBJ | C1+ | C1+ | C1S~ | K | W→413 |
| 415 | 25,15 | BLDG/OBJ | C1K~ | C1P2~ | C1S~ | C1K~ | — |
| 416 | 26,0 | BLDG/OBJ | K | C1P2~ | S+ | C2P2~ | — |
| 417 | 26,1 | BLDG/OBJ | C1+ | PP2~ | C1S~ | K+ | E→418 |
| 418 | 26,2 | CLIFF | C1+ | K | C1+ | PP2~ | W→417 |
| 419 | 26,3 | BLDG/OBJ | K | C1P2~ | C1+ | P2S~ | E→420 |
| 420 | 26,4 | CLIFF | K | K | C1+ | P2S~ | W→419 |
| 421 | 26,5 | CLIFF | C1+ | C1P2~ | C1+ | K+ | E→422 |
| 422 | 26,6 | CLIFF | C1+ | K | C1+ | C1P2~ | W→421 |
| 423 | 26,7 | BLDG/OBJ | C1 | PP2~ | K+ | K+ | E→424 |
| 424 | 26,8 | CLIFF | C1+ | K | C1K~ | PP2~ | W→423 |
| 425 | 26,9 | BLDG/OBJ | C1+ | G+ | G | GK~ | E→426 |
| 426 | 26,10 | TREES | C1+ | G+ | G | G+ | W→425 |
| 427 | 26,11 | BLDG/OBJ | K+ | W+ | K | G+ | E→428 |
| 428 | 26,12 | BLDG/OBJ | K | W+ | K | W+ | W→427 |
| 429 | 26,13 | CLIFF | C1 | C1P2~ | C1K~ | C1K~ | E→430 |
| 430 | 26,14 | CLIFF | C1+ | K | C1+ | PP2~ | W→429 |
| 431 | 26,15 | ROAD/BLDG | P+ | K+ | P+ | G | — |
| 432 | 27,0 | ROAD/BLDG | P | C1K~ | KP~ | PP2~ | — |
| 433 | 27,1 | ROAD/BLDG | P+ | K+ | P+ | K+ | — |
| 434 | 27,2 | ROAD/BLDG | C1+ | C1+ | C1P~ | G | — |
| 435 | 27,3 | BLDG/OBJ | C1G~ | G | C1G~ | P2+ | — |
| 436 | 27,4 | GRASS+VEG | C1G~ | G | G | G+ | — |
| 437 | 27,5 | BLDG/OBJ | GK~ | G | KP~ | W+ | — |
| 438 | 27,6 | ROAD/BLDG | GP~ | G | GP~ | P2+ | — |
| 439 | 27,7 | BLDG/OBJ | K | C1P2~ | C1+ | P2S~ | — |
| 440 | 27,8 | GRASS+OBJ | G | GP2~ | GK~ | G | E→441 |
| 441 | 27,9 | BLDG/OBJ | G | GP2~ | K | GP2~ | E→442 W→440 |
| 442 | 27,10 | BLDG/OBJ | G | C1P2~ | K+ | GP2~ | W→441 |
| 443 | 27,11 | ROAD/BLDG | K+ | C1P2~ | C1+ | P2+ | — |
| 444 | 27,12 | CLIFF | C1P2~ | C1P2~ | C1+ | K | — |
| 445 | 27,13 | BLDG/OBJ | P2+ | K+ | C1+ | KP2~ | — |
| 446 | 27,14 | CLIFF | C1P2~ | K | C1+ | C1+ | — |
| 447 | 27,15 | ROAD/BLDG | GK~ | C1K~ | C1K~ | P2+ | — |
| 448 | 28,0 | ROAD/BLDG | C1P2~ | P2+ | C1K~ | KP2~ | — |
| 449 | 28,1 | ROAD/BLDG | P2W~ | P2+ | C1+ | P2+ | — |
| 450 | 28,2 | ROAD/BLDG | P2 | C1+ | C1K~ | P2+ | — |
| 451 | 28,3 | BLDG/OBJ | C1+ | KP2~ | P2+ | KP2~ | — |
| 452 | 28,4 | ROAD/BLDG | C1K~ | C1G~ | P2+ | KP~ | — |
| 453 | 28,5 | BLDG/OBJ | C1+ | GP2~ | P2 | C1G~ | — |
| 454 | 28,6 | ROAD/BLDG | KP2~ | P2+ | P2 | P2+ | — |
| 455 | 28,7 | ROAD/BLDG | KP2~ | C1+ | P2+ | P2+ | — |
| 456 | 28,8 | ROAD/BLDG | KP2~ | C1+ | P2+ | P+ | — |
| 457 | 28,9 | GRASS+VEG | G | GP2~ | P2+ | G+ | E→458 |
| 458 | 28,10 | BLDG/OBJ | G | GP2~ | P2+ | GP2~ | E→459 W→457 |
| 459 | 28,11 | GRASS+OBJ | G | G+ | P2+ | GP2~ | E→460 W→458 |
| 460 | 28,12 | GRASS+VEG | G | PP2~ | G+ | G | E→461 W→459 |
| 461 | 28,13 | ROAD/BLDG | K+ | P2+ | P2+ | PP2~ | E→462 W→460 |
| 462 | 28,14 | ROAD/BLDG | K | C1P2~ | P2+ | P2+ | W→461 |
| 463 | 28,15 | ROAD/BLDG | P2+ | PP2~ | P2+ | C1P2~ | — |
| 464 | 29,0 | ROAD/BLDG | P2+ | PP2~ | P2+ | C1P~ | — |
| 465 | 29,1 | ROAD/BLDG | P2+ | P2+ | P2+ | C1P2~ | E→466 |
| 466 | 29,2 | ROAD/BLDG | K | P2+ | P2 | P2+ | E→467 W→465 |
| 467 | 29,3 | BLDG/OBJ | K+ | G | C1G~ | PP2~ | W→466 |
| 468 | 29,4 | BLDG/OBJ | GK~ | GK~ | C1P~ | PP2~ | — |
| 469 | 29,5 | CLIFF | C1P2~ | K | C1+ | P2+ | — |
| 470 | 29,6 | ROAD/BLDG | C1+ | PP2~ | P2+ | P2+ | — |
| 471 | 29,7 | BLDG/OBJ | C1+ | GK~ | G | GK~ | E→472 |
| 472 | 29,8 | BLDG/OBJ | C1+ | G+ | G | GK~ | E→473 W→471 |
| 473 | 29,9 | TREES | C1+ | G+ | G | G+ | E→474 W→472 |
| 474 | 29,10 | GRASS+VEG | P2+ | G+ | G | G+ | E→475 W→473 |
| 475 | 29,11 | GRASS+VEG | KP2~ | G+ | G | G+ | E→476 W→474 |
| 476 | 29,12 | GRASS+VEG | P2+ | G+ | G | G+ | W→475 |
| 477 | 29,13 | BLDG/OBJ | K+ | W+ | C1K~ | W+ | — |
| 478 | 29,14 | WATER | K | W+ | KW~ | W+ | — |
| 479 | 29,15 | ROAD/BLDG | P2+ | C1P2~ | GK~ | G+ | — |
| 480 | 30,0 | ROAD/BLDG | P2+ | C1P2~ | C1K~ | P2+ | — |
| 481 | 30,1 | BLDG/OBJ | P2W~ | C1G~ | G+ | C1P2~ | — |
| 482 | 30,2 | BLDG/OBJ | C1G~ | G | G+ | GP2~ | E→483 |
| 483 | 30,3 | BLDG/OBJ | G+ | C1+ | C1K~ | G+ | W→482 |
| 484 | 30,4 | BLDG/OBJ | G+ | G | C1G~ | C1+ | — |
| 485 | 30,5 | GRASS+VEG | G+ | C1S~ | G+ | G | — |
| 486 | 30,6 | BLDG/OBJ | S+ | C1S~ | C1K~ | S+ | — |
| 487 | 30,7 | BLDG/OBJ | C1G~ | G | G+ | C1S~ | — |
| 488 | 30,8 | GRASS+VEG | GP~ | G | G | G+ | — |
| 489 | 30,9 | BLDG/OBJ | C1P2~ | GK~ | G | GP2~ | — |
| 490 | 30,10 | ROAD/BLDG | C1P~ | P | P+ | K | — |
| 491 | 30,11 | ROAD/BLDG | C1+ | P+ | P+ | K+ | — |
| 492 | 30,12 | BLDG/OBJ | C1K~ | K | KP~ | K | E→493 |
| 493 | 30,13 | BLDG/OBJ | C1K~ | K | KP~ | K | W→492 |
| 494 | 30,14 | ROAD/BLDG | C1+ | K+ | P+ | P+ | — |
| 495 | 30,15 | ROAD/BLDG | C1P2~ | P | P+ | P2 | — |
| 496 | 31,0 | CLIFF | C1G~ | G+ | C1G~ | P | E→497 |
| 497 | 31,1 | GRASS+VEG | G | G+ | G+ | G | W→496 |
| 498 | 31,2 | ROAD/BLDG | C1P~ | PP2~ | C1 | K+ | — |
| 499 | 31,3 | ROAD/BLDG | C1K~ | PP2~ | C1P2~ | K+ | — |
| 500 | 31,4 | ROAD/BLDG | C1K~ | K+ | P2+ | C1K~ | E→501 |
| 501 | 31,5 | ROAD/BLDG | C1K~ | C1K~ | P2+ | K+ | W→500 |
| 502 | 31,6 | ROAD/BLDG | C1K~ | K+ | C1K~ | PP2~ | — |
| 503 | 31,7 | ROAD/BLDG | C1P2~ | C1P~ | C1+ | P2+ | E→504 |
| 504 | 31,8 | CLIFF | C1G~ | G | GP~ | P+ | E→505 W→503 |
| 505 | 31,9 | GRASS+OBJ | G+ | GP2~ | G | G | W→504 |
| 506 | 31,10 | TREES | P2 | GP2~ | G+ | GP2~ | — |
| 507 | 31,11 | BLDG/OBJ | K+ | GP~ | G | KP2~ | E→508 |
| 508 | 31,12 | ROAD/BLDG | K | GP~ | G | GP~ | E→509 W→507 |
| 509 | 31,13 | ROAD/BLDG | K | C1P2~ | G | GP~ | E→510 W→508 |
| 510 | 31,14 | BLDG/OBJ | KP2~ | GK~ | G+ | GP2~ | W→509 |
| 511 | 31,15 | TREES | P2+ | GP2~ | G+ | GK~ | — |
| 512 | 32,0 | GRASS+OBJ | GP~ | G | G+ | KP~ | — |
| 513 | 32,1 | ROAD/BLDG | KP2~ | C1P2~ | C1+ | KP2~ | — |
| 514 | 32,2 | CLIFF | C1P2~ | KP2~ | C1K~ | K | — |
| 515 | 32,3 | GRASS+OBJ | G | GP2~ | C1G~ | G | E→516 |
| 516 | 32,4 | BLDG/OBJ | G | GP2~ | C1 | GP2~ | E→517 W→515 |
| 517 | 32,5 | GRASS+OBJ | G | G | C1G~ | GP2~ | W→516 |
| 518 | 32,6 | GRASS+OBJ | G | KP2~ | GP2~ | G | — |
| 519 | 32,7 | ROAD/BLDG | K+ | P2+ | C1P2~ | P2+ | E→520 |
| 520 | 32,8 | ROAD/BLDG | K | P2+ | P2W~ | P2+ | W→519 |
| 521 | 32,9 | ROAD/BLDG | K | P2+ | P2+ | P2+ | — |
| 522 | 32,10 | ROAD/BLDG | KP2~ | P2+ | P2 | P2+ | E→523 |
| 523 | 32,11 | ROAD/BLDG | P2+ | KP2~ | P2 | P2+ | W→522 |
| 524 | 32,12 | CLIFF | C1K~ | K | C1+ | P+ | S→540 |
| 525 | 32,13 | BLDG/OBJ | GP2~ | P2+ | GP2~ | G | E→526 |
| 526 | 32,14 | ROAD/BLDG | P2 | KP2~ | P2 | P2+ | W→525 |
| 527 | 32,15 | ROAD/BLDG | P2 | P2+ | P2 | P2+ | — |
| 528 | 33,0 | CLIFF | C1+ | K | C1+ | P | — |
| 529 | 33,1 | ROAD/BLDG | GP2~ | P2+ | GP2~ | G+ | E→530 |
| 530 | 33,2 | ROAD/BLDG | KP2~ | KP2~ | KP2~ | P2+ | W→529 |
| 531 | 33,3 | ROAD/BLDG | KP2~ | KP2~ | K | P2+ | — |
| 532 | 33,4 | ROAD/BLDG | KP2~ | K+ | KP2~ | P2+ | — |
| 533 | 33,5 | ROAD/BLDG | P2+ | P2+ | P2+ | P2+ | — |
| 534 | 33,6 | ROAD/BLDG | KP2~ | P2+ | P2 | P2+ | — |
| 535 | 33,7 | ROAD/BLDG | P2+ | P2+ | P2 | P2+ | E→536 |
| 536 | 33,8 | ROAD/BLDG | P2 | KP2~ | C1P2~ | P2+ | E→537 W→535 |
| 537 | 33,9 | ROAD/BLDG | P2 | C1P2~ | C1P2~ | KP2~ | W→536 |
| 538 | 33,10 | ROAD/BLDG | KP2~ | GP2~ | P2+ | KP2~ | — |
| 539 | 33,11 | ROAD/BLDG | P2+ | P2+ | P2 | GP2~ | — |
| 540 | 33,12 | ROAD/BLDG | C1+ | PP2~ | P2+ | P+ | N→524 |
| 541 | 33,13 | ROAD/BLDG | KP2~ | C1P2~ | C1P~ | KP2~ | — |
| 542 | 33,14 | CLIFF | K | K | C1P~ | P2+ | E→543 |
| 543 | 33,15 | ROAD/BLDG | C1+ | C1+ | C1P~ | K | W→542 |
| 544 | 34,0 | ROAD/BLDG | KP2~ | C1P2~ | S+ | KP2~ | E→545 |
| 545 | 34,1 | BLDG/OBJ | K | C1P2~ | S+ | P2S~ | E→546 W→544 |
| 546 | 34,2 | CLIFF | K | K | C1S~ | P2S~ | W→545 |
| 547 | 34,3 | TREES | C1+ | G+ | G | G+ | — |
| 548 | 34,4 | BLDG/OBJ | C1+ | C1+ | S+ | K | — |
| 549 | 34,5 | BLDG/OBJ | C1+ | PP2~ | S+ | K+ | E→550 |
| 550 | 34,6 | CLIFF | C1+ | K | C1+ | PP2~ | E→551 W→549 |
| 551 | 34,7 | PATH | C1S~ | C1P2~ | P2S~ | K | W→550 |
| 552 | 34,8 | BLDG/OBJ | GP2~ | K+ | C1+ | KP2~ | — |
| 553 | 34,9 | CLIFF | C1+ | K | C1S~ | S+ | — |
| 554 | 34,10 | BLDG/OBJ | C1+ | KP2~ | P2+ | KP2~ | — |
| 555 | 34,11 | ROAD/BLDG | C1K~ | GK~ | P2+ | KP~ | — |
| 556 | 34,12 | BLDG/OBJ | C1+ | GP2~ | P2 | GS~ | — |
| 557 | 34,13 | CLIFF | C1 | C1P2~ | K+ | K+ | — |
| 558 | 34,14 | BLDG/OBJ | C1+ | G+ | G | GK~ | — |
| 559 | 34,15 | BLDG/OBJ | GK~ | C1P2~ | C1+ | GP2~ | — |
| 560 | 35,0 | BLDG/OBJ | K | C1P2~ | C1+ | P2+ | — |
| 561 | 35,1 | CLIFF | K | K | C1+ | P2+ | — |
| 562 | 35,2 | PATH | P2 | C1+ | C1+ | P2 | — |
| 563 | 35,3 | BLDG/OBJ | P2 | C1+ | C1+ | P2+ | — |
| 564 | 35,4 | CLIFF | C1P2~ | K | C1+ | P2 | — |
| 565 | 35,5 | BLDG/OBJ | P2 | C1P2~ | P2 | P2+ | — |
| 566 | 35,6 | ROAD/BLDG | C1P2~ | C1G~ | P2+ | KP2~ | — |
| 567 | 35,7 | BLDG/OBJ | C1P2~ | GP2~ | P2 | GP2~ | — |
| 568 | 35,8 | ROAD/BLDG | GK~ | C1P2~ | P+ | GK~ | — |
| 569 | 35,9 | CLIFF | K | K | C1P~ | P2+ | — |
| 570 | 35,10 | BLDG/OBJ | G | C2+ | C1K~ | G+ | E→571 |
| 571 | 35,11 | BLDG/OBJ | G | C2+ | C1K~ | C2G~ | W→570 |
| 572 | 35,12 | CLIFF | K | K | C1+ | P2S~ | — |
| 573 | 35,13 | ROAD/BLDG | C1+ | PP2~ | C1P~ | K+ | E→574 |
| 574 | 35,14 | CLIFF | C1+ | K | C1+ | PP2~ | W→573 |
| 575 | 35,15 | BLDG/OBJ | C2+ | K+ | GK~ | G+ | — |
| 576 | 36,0 | BLDG/OBJ | C2 | K+ | KS~ | C2S~ | — |
| 577 | 36,1 | BLDG/OBJ | GP2~ | KP2~ | GS~ | G+ | — |
| 578 | 36,2 | BLDG/OBJ | KP2~ | C1P2~ | KS~ | C1K~ | — |
| 579 | 36,3 | CLIFF | C1P2~ | C1P2~ | C1+ | K | — |
| 580 | 36,4 | BLDG/OBJ | P2W~ | K+ | C1+ | C1K~ | — |
| 581 | 36,5 | CLIFF | C1+ | K | C1+ | C1+ | — |
| 582 | 36,6 | ROAD/BLDG | C1 | KP2~ | P2+ | KP2~ | — |
| 583 | 36,7 | BLDG/OBJ | C1+ | GP2~ | P2 | PP2~ | E→584 |
| 584 | 36,8 | ROAD/BLDG | C1G~ | KP2~ | P2+ | GP2~ | W→583 |
| 585 | 36,9 | ROAD/BLDG | C1K~ | C1G~ | P2+ | KP2~ | — |
| 586 | 36,10 | BLDG/OBJ | G+ | W+ | P+ | G+ | — |
| 587 | 36,11 | BLDG/OBJ | G+ | G | GW~ | W+ | — |
| 588 | 36,12 | GRASS+VEG | G+ | PP2~ | G+ | G | — |
| 589 | 36,13 | ROAD/BLDG | P2+ | PP2~ | GK~ | P2+ | — |
| 590 | 36,14 | BLDG/OBJ | GW~ | G | G+ | PP2~ | — |
| 591 | 36,15 | BLDG/OBJ | S+ | C1S~ | GK~ | P2S~ | — |
| 592 | 37,0 | GRASS+OBJ | G | GW~ | GW~ | G | E→593 |
| 593 | 37,1 | BLDG/OBJ | G | GW~ | W | GW~ | E→594 W→592 |
| 594 | 37,2 | GRASS+OBJ | G | G | GW~ | GW~ | W→593 |
| 595 | 37,3 | BLDG/OBJ | G+ | C1W~ | C1+ | G+ | E→596 |
| 596 | 37,4 | BLDG/OBJ | G | C1W~ | C1 | P2W~ | E→597 W→595 |
| 597 | 37,5 | BLDG/OBJ | G+ | G | C1K~ | P2W~ | W→596 |
| 598 | 37,6 | ROAD/BLDG | KP2~ | PP2~ | G+ | G+ | E→599 |
| 599 | 37,7 | BLDG/OBJ | P2+ | C1G~ | G+ | C1P2~ | W→598 |
| 600 | 37,8 | BLDG/OBJ | C1P2~ | G | G+ | GP2~ | — |
| 601 | 37,9 | TREES | GK~ | GP2~ | K | G+ | E→602 |
| 602 | 37,10 | TREES | G+ | GP2~ | K | GK~ | E→603 W→601 S→618 |
| 603 | 37,11 | TREES | GK~ | GK~ | C1G~ | GP2~ | W→602 |
| 604 | 37,12 | ROAD/BLDG | K+ | C1P2~ | P+ | K+ | — |
| 605 | 37,13 | ROAD/BLDG | C1+ | C1P2~ | C1P2~ | C1K~ | — |
| 606 | 37,14 | ROAD/BLDG | C1+ | C1+ | P2W~ | PP2~ | — |
| 607 | 37,15 | ROAD/BLDG | C1K~ | C1P2~ | P+ | PP2~ | — |
| 608 | 38,0 | GRASS+VEG | G+ | C1P2~ | G+ | G | — |
| 609 | 38,1 | BLDG/OBJ | GK~ | C1P2~ | C1+ | GK~ | — |
| 610 | 38,2 | BLDG/OBJ | C1K~ | C1P2~ | C1+ | C1K~ | E→611 |
| 611 | 38,3 | CLIFF | K | K | C1+ | P2S~ | W→610 |
| 612 | 38,4 | BLDG/OBJ | G+ | W+ | C1K~ | G+ | — |
| 613 | 38,5 | BLDG/OBJ | G+ | G | GW~ | W+ | — |
| 614 | 38,6 | BLDG/OBJ | GW~ | G | G+ | C1P2~ | — |
| 615 | 38,7 | ROAD/BLDG | K | C1P2~ | C1+ | KP2~ | — |
| 616 | 38,8 | CLIFF | K | K | C1S~ | C2P2~ | — |
| 617 | 38,9 | BLDG/OBJ | K+ | W+ | P+ | W+ | — |
| 618 | 38,10 | BLDG/OBJ | K | W+ | KW~ | W+ | N→602 |
| 619 | 38,11 | ROAD/BLDG | P2+ | PP2~ | GK~ | G+ | — |
| 620 | 38,12 | ROAD/BLDG | P2+ | PP2~ | KP~ | P2+ | — |
| 621 | 38,13 | ROAD/BLDG | P2W~ | GP~ | G+ | PP2~ | — |
| 622 | 38,14 | BLDG/OBJ | GP~ | G | G+ | GP2~ | — |
| 623 | 38,15 | ROAD/BLDG | C1K~ | P+ | PP2~ | C1K~ | — |
| 624 | 39,0 | ROAD/BLDG | K | P+ | PP2~ | P2+ | — |
| 625 | 39,1 | ROAD/BLDG | K | K | C1P~ | P2+ | E→626 |
| 626 | 39,2 | ROAD/BLDG | C1P~ | P | PP2~ | K | W→625 |
| 627 | 39,3 | ROAD/BLDG | C1K~ | PP2~ | PP2~ | K+ | E→628 |
| 628 | 39,4 | CLIFF | C1+ | K | C1P~ | PP2~ | W→627 |
| 629 | 39,5 | BLDG/OBJ | C1P2~ | C1P2~ | C1K~ | KP2~ | — |
| 630 | 39,6 | ROAD/BLDG | PP2~ | KP~ | PP2~ | K | E→631 |
| 631 | 39,7 | ROAD/BLDG | PP2~ | PP2~ | PP2~ | KP2~ | W→630 |
| 632 | 39,8 | BLDG/OBJ | C1+ | GK~ | G | GK~ | E→633 |
| 633 | 39,9 | BLDG/OBJ | C1+ | G+ | G | GK~ | W→632 |
| 634 | 39,10 | BLDG/OBJ | G | G | C1K~ | C2G~ | — |
| 635 | 39,11 | CLIFF | C2G~ | G | C1G~ | C2+ | — |
| 636 | 39,12 | CLIFF | C1G~ | G | C1G~ | C2 | — |
| 637 | 39,13 | BLDG/OBJ | C1K~ | P2+ | P2 | C2G~ | — |
| 638 | 39,14 | BLDG/OBJ | K+ | C1+ | K | G+ | E→639 |
| 639 | 39,15 | BLDG/OBJ | K+ | C1+ | C1K~ | C1+ | W→638 |
| 640 | 40,0 | BLDG/OBJ | K | C1+ | C1K~ | C1+ | E→641 |
| 641 | 40,1 | BLDG/OBJ | GK~ | G | GK~ | C1+ | W→640 |
| 642 | 40,2 | BLDG/OBJ | S+ | C1S~ | GK~ | G+ | E→643 |
| 643 | 40,3 | BLDG/OBJ | C1S~ | C1G~ | G+ | C1S~ | W→642 |
| 644 | 40,4 | BLDG/OBJ | C1S~ | G | G+ | GS~ | — |
| 645 | 40,5 | CLIFF | G+ | C1G~ | GK~ | K+ | — |
| 646 | 40,6 | PATH | G | G | S+ | GS~ | — |
| 647 | 40,7 | GRASS+VEG | C1G~ | C1G~ | G | G | — |
| 648 | 40,8 | GRASS+VEG | G | G | G | G | E→649 |
| 649 | 40,9 | GRASS+OBJ | C1S~ | S+ | G | G | W→648 |
| 650 | 40,10 | CLIFF | C1G~ | G+ | C1+ | C1K~ | — |
| 651 | 40,11 | CLIFF | C1K~ | C1G~ | C1+ | C1K~ | — |
| 652 | 40,12 | TREES | K+ | GK~ | GK~ | G+ | — |
| 653 | 40,13 | TREES | GK~ | G+ | K+ | G+ | — |
| 654 | 40,14 | TREES | G | G+ | G+ | G+ | E→655 |
| 655 | 40,15 | TREES | G | G+ | K+ | G+ | W→654 |
| 656 | 41,0 | TREES | G+ | G+ | G+ | G+ | E→657 |
| 657 | 41,1 | TREES | GK~ | G+ | GK~ | G+ | W→656 |
| 658 | 41,2 | TREES | GK~ | G+ | GK~ | G+ | E→659 |
| 659 | 41,3 | TREES | GK~ | G+ | K+ | G+ | E→660 W→658 |
| 660 | 41,4 | TREES | G+ | G+ | G+ | G+ | W→659 |
| 661 | 41,5 | TREES | G+ | G+ | G+ | G+ | — |
| 662 | 41,6 | TREES | G+ | G+ | G+ | G+ | — |
| 663 | 41,7 | WATER | W | W | W | W | — |
| 664 | 41,8 | WATER | W | W | W | W | — |
| 665 | 41,9 | WATER | W | W+ | W | W | E→666 |
| 666 | 41,10 | WATER | W | W | W | W | W→665 S→682 |
| 667 | 41,11 | WATER | W | W+ | W | W | E→668 |
| 668 | 41,12 | WATER | W | W+ | W | W | E→669 W→667 S→684 |
| 669 | 41,13 | WATER | W | W | W | W+ | W→668 S→685 |
| 670 | 41,14 | WATER | W | W+ | W+ | W | E→671 |
| 671 | 41,15 | WATER | W | W+ | G | W+ | W→670 |
| 672 | 42,0 | WATER | W | W+ | W+ | W | E→673 |
| 673 | 42,1 | WATER | W+ | C1+ | G+ | W+ | W→672 |
| 674 | 42,2 | WATER | W | W | W | W | — |
| 675 | 42,3 | WATER | W | W+ | GW~ | W | E→676 |
| 676 | 42,4 | BLDG/OBJ | KW~ | G | G | W+ | W→675 |
| 677 | 42,5 | WATER | W | W | W | W | — |
| 678 | 42,6 | WATER | W | W | W | W | — |
| 679 | 42,7 | WATER | W | W | W | W | — |
| 680 | 42,8 | WATER | W | W+ | W+ | W | E→681 S→696 |
| 681 | 42,9 | WATER | W | W | W | W | W→680 S→697 |
| 682 | 42,10 | WATER | W | W | W | W | N→666 |
| 683 | 42,11 | WATER | W | W | W | W | — |
| 684 | 42,12 | WATER | W | W | W | W | E→685 S→700 N→668 |
| 685 | 42,13 | WATER | W | W+ | W | W | E→686 W→684 S→701 N→669 |
| 686 | 42,14 | WATER | W | W | W | W+ | W→685 S→702 |
| 687 | 42,15 | WATER | W | W | W | W | — |
| 688 | 43,0 | WATER | W | W | W | W | S→704 |
| 689 | 43,1 | WATER | W | W+ | W | W | E→690 S→705 |
| 690 | 43,2 | WATER | W | W+ | W | W+ | E→691 W→689 S→706 |
| 691 | 43,3 | WATER | W | W | W | W+ | W→690 S→707 |
| 692 | 43,4 | WATER | W | W | W | W | — |
| 693 | 43,5 | WATER | W | W | W | W | — |
| 694 | 43,6 | WATER | W | W | W | W | S→710 |
| 695 | 43,7 | WATER | W | W+ | W+ | W | E→696 |
| 696 | 43,8 | WATER | W | W | W+ | W+ | E→697 W→695 S→712 N→680 |
| 697 | 43,9 | WATER | W | W | W | W | W→696 S→713 N→681 |
| 698 | 43,10 | WATER | W | W | W | W | — |
| 699 | 43,11 | WATER | W | W | W | W | — |
| 700 | 43,12 | WATER | W+ | W+ | W | W | E→701 N→684 |
| 701 | 43,13 | WATER | C1+ | W+ | W | W+ | E→702 W→700 N→685 |
| 702 | 43,14 | WATER | W+ | W+ | W+ | W+ | E→703 W→701 N→686 |
| 703 | 43,15 | WATER | W | W+ | G | W+ | W→702 |
| 704 | 44,0 | WATER | W | W | W+ | W+ | S→720 N→688 |
| 705 | 44,1 | WATER | C1+ | W+ | W | W+ | E→706 N→689 |
| 706 | 44,2 | WATER | W+ | W+ | W | W+ | E→707 W→705 S→722 N→690 |
| 707 | 44,3 | WATER | W | W+ | W+ | W+ | E→708 W→706 N→691 |
| 708 | 44,4 | BLDG/OBJ | W+ | C1+ | G+ | W+ | W→707 |
| 709 | 44,5 | WATER | W | W+ | W | W | E→710 |
| 710 | 44,6 | WATER | W | W+ | GW~ | W+ | W→709 N→694 |
| 711 | 44,7 | BLDG/OBJ | GW~ | G+ | G+ | W+ | — |
| 712 | 44,8 | WATER | W | W | W | W+ | E→713 S→728 N→696 |
| 713 | 44,9 | WATER | W | W+ | W | W | E→714 W→712 S→729 N→697 |
| 714 | 44,10 | WATER | W | W | W | W+ | W→713 S→730 |
| 715 | 44,11 | WATER | W | W | W | W | — |
| 716 | 44,12 | WATER | W | W | W | W | — |
| 717 | 44,13 | BLDG/OBJ | W | G+ | G | W+ | — |
| 718 | 44,14 | WATER | W | W | W | W | — |
| 719 | 44,15 | WATER | W | W+ | W | W | — |
| 720 | 45,0 | WATER | W | W | W | W+ | S→736 N→704 |
| 721 | 45,1 | WATER | W | W+ | W+ | W+ | — |
| 722 | 45,2 | BLDG/OBJ | W | KW~ | C1+ | K+ | N→706 |
| 723 | 45,3 | WATER | W | W | W+ | W+ | S→739 |
| 724 | 45,4 | WATER | W | W | W | W | — |
| 725 | 45,5 | WATER | W+ | W+ | W | W | E→726 |
| 726 | 45,6 | WATER | C1+ | W+ | W+ | W+ | W→725 |
| 727 | 45,7 | BLDG/OBJ | W+ | KW~ | C1+ | K+ | — |
| 728 | 45,8 | WATER | W+ | W+ | W | W+ | E→729 N→712 |
| 729 | 45,9 | WATER | C1+ | W+ | W | W+ | E→730 W→728 N→713 |
| 730 | 45,10 | WATER | W | W | W | W+ | W→729 N→714 |
| 731 | 45,11 | WATER | W | W+ | W | W | E→732 S→747 |
| 732 | 45,12 | WATER | W | W | W | W | W→731 |
| 733 | 45,13 | WATER | W | W | W | W | S→749 |
| 734 | 45,14 | WATER | W | W | W | W | — |
| 735 | 45,15 | BLDG/OBJ | GW~ | G+ | G+ | W | — |
| 736 | 46,0 | WATER | W | W+ | W | W | N→720 |
| 737 | 46,1 | BLDG/OBJ | G+ | G | G+ | KW~ | — |
| 738 | 46,2 | WATER | W | W+ | W | W | E→739 S→754 |
| 739 | 46,3 | WATER | W | W | W+ | W+ | E→740 W→738 S→755 N→723 |
| 740 | 46,4 | WATER | W+ | W+ | W | W+ | W→739 S→756 |
| 741 | 46,5 | CLIFF | C1+ | C1+ | VW~ | KW~ | E→742 |
| 742 | 46,6 | CLIFF | C1+ | KW~ | VW~ | C1+ | W→741 |
| 743 | 46,7 | WATER | W | W+ | W | W+ | S→759 |
| 744 | 46,8 | CLIFF | C1+ | C1+ | VW~ | KW~ | — |
| 745 | 46,9 | WATER | W+ | W+ | W | W+ | E→746 |
| 746 | 46,10 | WATER | C1+ | W+ | W+ | W+ | W→745 |
| 747 | 46,11 | BLDG/OBJ | W+ | KW~ | C1+ | K+ | N→731 |
| 748 | 46,12 | WATER | C1V~ | C1W~ | W+ | W+ | — |
| 749 | 46,13 | WATER | W | W | W | W | N→733 |
| 750 | 46,14 | WATER | W+ | W+ | W | W | E→751 |
| 751 | 46,15 | WATER | W+ | W+ | W | W+ | W→750 |
| 752 | 47,0 | BLDG/OBJ | W+ | KW~ | C1+ | K+ | — |
| 753 | 47,1 | WATER | C1+ | W+ | W+ | W+ | E→754 |
| 754 | 47,2 | WATER | W+ | W+ | W | W+ | E→755 W→753 N→738 |
| 755 | 47,3 | WATER | W | W+ | W | W | E→756 W→754 N→739 |
| 756 | 47,4 | WATER | W | W+ | W | W+ | E→757 W→755 N→740 |
| 757 | 47,5 | WATER | W+ | W+ | W | W+ | E→758 W→756 |
| 758 | 47,6 | WATER | C1+ | W+ | W | W+ | E→759 W→757 |
| 759 | 47,7 | WATER | W | W+ | W | W+ | E→760 W→758 N→743 |
| 760 | 47,8 | WATER | W | W | W | W+ | W→759 |
| 761 | 47,9 | WATER | W+ | W+ | W+ | W+ | — |
| 762 | 47,10 | WATER | W+ | C1G~ | GW~ | W | — |
| 763 | 47,11 | CLIFF | C1+ | C1K~ | C1+ | C1+ | — |
| 764 | 47,12 | CLIFF | C1+ | C1+ | G+ | G+ | — |
| 765 | 47,13 | CLIFF | C1+ | C1+ | C1+ | C1K~ | E→766 |
| 766 | 47,14 | CLIFF | C1+ | K+ | C1G~ | C1+ | E→767 W→765 |
| 767 | 47,15 | CLIFF | C1+ | C1+ | C1G~ | GK~ | W→766 |
| 768 | 48,0 | CLIFF | C1+ | C1+ | C1G~ | K+ | — |
| 769 | 48,1 | CLIFF | C1+ | C1+ | G+ | GK~ | E→770 |
| 770 | 48,2 | CLIFF | C1+ | GK~ | G+ | C1+ | W→769 |
| 771 | 48,3 | PATH | G | GS~ | G | GS~ | — |
| 772 | 48,4 | PATH | G | GS~ | G+ | GS~ | E→773 |
| 773 | 48,5 | PATH | G | GS~ | G | GS~ | W→772 |
| 774 | 48,6 | PATH | G+ | GS~ | G | GS~ | — |
| 775 | 48,7 | BLDG/OBJ | G | GW~ | W+ | G | — |
| 776 | 48,8 | BLDG/OBJ | G | GW~ | W+ | GK~ | E→777 |
| 777 | 48,9 | BLDG/OBJ | G | GW~ | W | GW~ | E→778 W→776 |
| 778 | 48,10 | GRASS+VEG | G | G | GW~ | GW~ | W→777 |
| 779 | 48,11 | ROAD/BLDG | G+ | P+ | P+ | G+ | — |
| 780 | 48,12 | ROAD/BLDG | GW~ | PW~ | P+ | P2+ | — |
| 781 | 48,13 | ROAD/BLDG | G+ | P+ | P+ | P2W~ | — |
| 782 | 48,14 | ROAD/BLDG | G+ | G | GP~ | P2W~ | — |
| 783 | 48,15 | ROAD/BLDG | P2+ | PP2~ | G+ | G+ | — |
| 784 | 49,0 | ROAD/BLDG | P2+ | GP~ | G+ | P2+ | — |
| 785 | 49,1 | ROAD/BLDG | P2+ | P2+ | GK~ | GP2~ | E→786 |
| 786 | 49,2 | ROAD/BLDG | P2+ | GP~ | GP2~ | P2+ | W→785 |
| 787 | 49,3 | BLDG/OBJ | GP~ | G+ | G+ | GP2~ | — |
| 788 | 49,4 | GRASS+VEG | G+ | G | G+ | G+ | — |
| 789 | 49,5 | GRASS+VEG | G | G+ | G | G+ | S→805 |
| 790 | 49,6 | GRASS+VEG | GP2~ | G+ | G | G+ | — |
| 791 | 49,7 | GRASS+VEG | GP2~ | G+ | G | G+ | E→792 |
| 792 | 49,8 | BLDG/OBJ | C1+ | C1G~ | G | G | W→791 S→808 |
| 793 | 49,9 | CLIFF | C1+ | C1G~ | GK~ | GK~ | — |
| 794 | 49,10 | CLIFF | C1+ | C1+ | G+ | G | — |
| 795 | 49,11 | PATH | G | GS~ | G+ | GS~ | — |
| 796 | 49,12 | GRASS+OBJ | G | GK~ | C1K~ | G+ | — |
| 797 | 49,13 | CLIFF | G+ | K+ | C1K~ | C1G~ | — |
| 798 | 49,14 | BLDG/OBJ | C1+ | C1G~ | G | G+ | E→799 |
| 799 | 49,15 | CLIFF | C1+ | G+ | G | C1G~ | W→798 |
| 800 | 50,0 | BLDG/OBJ | C1K~ | G+ | G | C1G~ | — |
| 801 | 50,1 | GRASS+VEG | G | G | G | G | — |
| 802 | 50,2 | WATER | W | W | W | W | — |
| 803 | 50,3 | WATER | W | W | W | W | — |
| 804 | 50,4 | WATER | W | W | W+ | W | — |
| 805 | 50,5 | GRASS+OBJ | G | GP2~ | P2+ | G+ | N→789 |
| 806 | 50,6 | BLDG/OBJ | K | GP2~ | G | GP2~ | E→807 |
| 807 | 50,7 | ROAD/BLDG | K | GP2~ | GS~ | GP2~ | W→806 |
| 808 | 50,8 | BLDG/OBJ | G+ | C1W~ | C1+ | G+ | E→809 N→792 |
| 809 | 50,9 | BLDG/OBJ | G+ | G+ | C1K~ | P2W~ | W→808 |
| 810 | 50,10 | GRASS+VEG | G | G+ | G+ | G+ | — |
| 811 | 50,11 | CLIFF | C1+ | W+ | C1W~ | C1+ | E→812 |
| 812 | 50,12 | CLIFF | C1+ | C1+ | C1W~ | W+ | W→811 |
| 813 | 50,13 | GRASS | G | G | G | G | E→814 |
| 814 | 50,14 | BLDG/OBJ | C1K~ | C1K~ | G+ | G+ | E→815 W→813 |
| 815 | 50,15 | BLDG/OBJ | C1K~ | G+ | G+ | C1K~ | W→814 |
| 816 | 51,0 | BLDG/OBJ | KW~ | G+ | G+ | KW~ | E→817 |
| 817 | 51,1 | BLDG/OBJ | KW~ | KW~ | G+ | G+ | W→816 |
| 818 | 51,2 | GRASS+VEG | G+ | C1K~ | G+ | G | — |
| 819 | 51,3 | ROAD/BLDG | KP~ | P+ | P+ | PP2~ | — |
| 820 | 51,4 | ROAD/BLDG | K+ | GP2~ | P2+ | P2+ | — |
| 821 | 51,5 | GRASS+VEG | G+ | G+ | G | G | S→837 |
| 822 | 51,6 | CLIFF | G+ | C1G~ | G+ | K+ | — |
| 823 | 51,7 | CLIFF | G+ | C1G~ | GK~ | K+ | — |
| 824 | 51,8 | ROAD/BLDG | P2+ | C1G~ | C1G~ | P2+ | — |
| 825 | 51,9 | TREES | C1+ | G+ | G | GK~ | — |
| 826 | 51,10 | ROAD/BLDG | PP2~ | P+ | P+ | PP2~ | — |
| 827 | 51,11 | BLDG/OBJ | G+ | G | G+ | K+ | — |
| 828 | 51,12 | TREES | C1 | GK~ | G+ | GK~ | E→829 |
| 829 | 51,13 | TREES | C1+ | G+ | G | GK~ | W→828 |
| 830 | 51,14 | BLDG/OBJ | GK~ | GK~ | C1K~ | G+ | — |
| 831 | 51,15 | CLIFF | C1+ | C1G~ | C1+ | C1G~ | — |
| 832 | 52,0 | GRASS+VEG | G+ | G | GK~ | C1K~ | — |
| 833 | 52,1 | TREES | G+ | G | GK~ | G+ | — |
| 834 | 52,2 | GRASS+VEG | C1G~ | G | G | G+ | — |
| 835 | 52,3 | BLDG/OBJ | G | GK~ | GK~ | G+ | — |
| 836 | 52,4 | TREES | G+ | G+ | G+ | G+ | — |
| 837 | 52,5 | TREES | G | G+ | GK~ | G+ | E→838 N→821 |
| 838 | 52,6 | TREES | G+ | G+ | G+ | G+ | W→837 |
| 839 | 52,7 | GRASS+OBJ | GK~ | K+ | G | G | — |
| 840 | 52,8 | BLDG/OBJ | G+ | C1+ | G+ | G | — |
| 841 | 52,9 | EMPTY | X | X | X | X | — |
| 842 | 52,10 | EMPTY | X | X | X | X | — |
| 843 | 52,11 | EMPTY | X | X | X | X | — |
| 844 | 52,12 | EMPTY | X | X | X | X | — |
| 845 | 52,13 | EMPTY | X | X | X | X | — |
| 846 | 52,14 | EMPTY | X | X | X | X | — |
| 847 | 52,15 | EMPTY | X | X | X | X | — |

## Appendix B — Verified multi-tile blocks (pixel-seam matched)

Each grid shows tile numbers in their exact relative positions; `.` = not part of the block. Assemble these blocks exactly as shown.

**Block 1** (tiles 1–17): `[[1,2],[17,.]]`
**Block 2** (tiles 5–42): `[[.,.,5,6,7,8,9,10],[19,20,21,22,23,24,25,26],[35,36,.,38,39,.,.,42]]`
**Block 3** (11–27): `[[11],[27]]`
**Block 4** (13–29): `[[13],[29]]`
**Block 5** (30–31): `[30,31]`
**Block 6** (44–45): `[44,45]`
**Block 7** (49–65): `[[49],[65]]`
**Block 8** (50–66): `[[50],[66]]`
**Block 9** (55–71): `[[.,55,56],[70,71,.]]`
**Block 10** (57–58): `[57,58]`
**Block 11** (59–75): `[[59],[75]]`
**Block 12** (67–68): `[67,68]`
**Block 13** (69–85): `[[69],[85]]`
**Block 14** (72–73): `[72,73]`
**Block 15** (77–78): `[77,78]`
**Block 16** (81–82): `[81,82]`
**Block 17** (89–105): `[[89],[105]]`
**Block 18** (90–108): `[[90,.,.],[106,107,108]]`
**Block 19** (93–109): `[[93,94],[109,.]]`
**Block 20** (97–98): `[97,98]`
**Block 21** (102–120): `[[102,103,104],[.,.,120]]`
**Block 22** (122–124): `[122,123,124]`
**Block 23** (129–130): `[129,130]`
**Block 24** (132–133): `[132,133]`
**Block 25** (140–156): `[[.,.,140],[154,155,156]]`
**Block 26** (141–159): `[[141,142,143],[157,158,159]]`
**Block 27** (144–146): `[144,145,146]`
**Block 28** (147–163): `[[.,147],[162,163]]`
**Block 29** (149–152): `[149,150,151,152]`
**Block 30** (166–185): `[[166,167,.,.],[.,183,184,185]]`
**Block 31** (173–175): `[173,174,175]`
**Block 32** (176–179): `[176,177,178,179]`
**Block 33** (181–182): `[181,182]`
**Block 34** (188–191): `[188,189,190,191]`
**Block 35** (192–224): `[[192],[208],[224]]` (3-tall tree)
**Block 36** (197–198): `[197,198]`
**Block 37** (199–200): `[199,200]`
**Block 38** (201–202): `[201,202]`
**Block 39** (209–210): `[209,210]`
**Block 40** (214–215): `[214,215]`
**Block 41** (216–217): `[216,217]`
**Block 42** (218–221): `[218,219,220,221]`
**Block 43** (222–223): `[222,223]`
**Block 44** (231–232): `[231,232]`
**Block 45** (234–255): `[[.,.,234,.,.,237,238,.],[248,249,250,251,252,253,254,255]]`
**Block 46** (243–259): `[[243,244],[259,.]]`
**Block 47** (260–261): `[260,261]`
**Block 48** (264–299): `[[264,265,.,.],[.,281,.,.],[.,297,298,299]]`
**Block 49** (267–268): `[267,268]`
**Block 50** (272–290): `[[272,273,.],[288,289,290]]`
**Block 51** (283–302): `[[283,284,285,286],[.,300,301,302]]`
**Block 52** (291–307): `[[.,.,.,291],[304,305,306,307]]`
**Block 53** (292–296): `[292,293,294,295,296]`
**Block 54** (308–309): `[308,309]`
**Block 55** (310–311): `[310,311]`
**Block 56** (313–314): `[313,314]`
**Block 57** (316–318): `[316,317,318]`
**Block 58** (321–337): `[[321,322],[337,.]]`
**Block 59** (324–325): `[324,325]`
**Block 60** (329–345): `[[329],[345]]`
**Block 61** (331–347): `[[331],[347]]`
**Block 62** (343–344): `[343,344]`
**Block 63** (349–350): `[349,350]`
**Block 64** (356–357): `[356,357]`
**Block 65** (358–359): `[358,359]`
**Block 66** (362–363): `[362,363]`
**Block 67** (364–381): `[[364,.],[380,381]]`
**Block 68** (368–369): `[368,369]`
**Block 69** (371–372): `[371,372]`
**Block 70** (387–389): `[387,388,389]`
**Block 71** (390–391): `[390,391]`
**Block 72** (397–398): `[397,398]`
**Block 73** (402–407): `[402,403,404,405,406,407]`
**Block 74** (412–414): `[412,413,414]`
**Block 75** (417–418): `[417,418]`
**Block 76** (419–420): `[419,420]`
**Block 77** (421–422): `[421,422]`
**Block 78** (423–424): `[423,424]`
**Block 79** (425–426): `[425,426]`
**Block 80** (427–428): `[427,428]`
**Block 81** (429–430): `[429,430]`
**Block 82** (440–442): `[440,441,442]`
**Block 83** (457–462): `[457,458,459,460,461,462]`
**Block 84** (465–467): `[465,466,467]`
**Block 85** (471–476): `[471,472,473,474,475,476]`
**Block 86** (482–483): `[482,483]`
**Block 87** (492–493): `[492,493]`
**Block 88** (496–497): `[496,497]`
**Block 89** (500–501): `[500,501]`
**Block 90** (503–505): `[503,504,505]`
**Block 91** (507–510): `[507,508,509,510]`
**Block 92** (515–517): `[515,516,517]`
**Block 93** (519–520): `[519,520]`
**Block 94** (522–523): `[522,523]`
**Block 95** (524–540): `[[524],[540]]`
**Block 96** (525–526): `[525,526]`
**Block 97** (529–530): `[529,530]`
**Block 98** (535–537): `[535,536,537]`
**Block 99** (542–543): `[542,543]`
**Block 100** (544–546): `[544,545,546]`
**Block 101** (549–551): `[549,550,551]`
**Block 102** (570–571): `[570,571]`
**Block 103** (573–574): `[573,574]`
**Block 104** (583–584): `[583,584]`
**Block 105** (592–594): `[592,593,594]`
**Block 106** (595–597): `[595,596,597]`
**Block 107** (598–599): `[598,599]`
**Block 108** (601–618): `[[601,602,603],[.,618,.]]`
**Block 109** (610–611): `[610,611]`
**Block 110** (625–626): `[625,626]`
**Block 111** (627–628): `[627,628]`
**Block 112** (630–631): `[630,631]`
**Block 113** (632–633): `[632,633]`
**Block 114** (638–639): `[638,639]`
**Block 115** (640–641): `[640,641]`
**Block 116** (642–643): `[642,643]`
**Block 117** (648–649): `[648,649]`
**Block 118** (654–655): `[654,655]`
**Block 119** (656–657): `[656,657]`
**Block 120** (658–660): `[658,659,660]`
**Block 121** (665–682): `[[665,666],[.,682]]`
**Block 122** (667–703): `[[667,668,669,.,.],[.,684,685,686,.],[.,700,701,702,703]]` (10-tile island)
**Block 123** (670–671): `[670,671]`
**Block 124** (672–673): `[672,673]`
**Block 125** (675–676): `[675,676]`
**Block 126** (680–730): `[[.,680,681,.],[695,696,697,.],[.,712,713,714],[.,728,729,730]]` (11-tile island)
**Block 127** (688–736): `[[688],[704],[720],[736]]` (4-tall cliff-into-water)
**Block 128** (689–722): `[[689,690,691,.],[705,706,707,708],[.,722,.,.]]` (8-tile island)
**Block 129** (694–710): `[[.,694],[709,710]]`
**Block 130** (723–760): `[[.,.,723,.,.,.,.,.],[.,738,739,740,.,.,743,.],[753,754,755,756,757,758,759,760]]` (13-tile beach-cove)
**Block 131** (725–726): `[725,726]`
**Block 132** (731–747): `[[731,732],[747,.]]`
**Block 133** (733–749): `[[733],[749]]`
**Block 134** (741–742): `[741,742]`
**Block 135** (745–746): `[745,746]`
**Block 136** (750–751): `[750,751]`
**Block 137** (765–767): `[765,766,767]`
**Block 138** (769–770): `[769,770]`
**Block 139** (772–773): `[772,773]`
**Block 140** (776–778): `[776,777,778]`
**Block 141** (785–786): `[785,786]`
**Block 142** (789–805): `[[789],[805]]`
**Block 143** (791–809): `[[791,792,.],[.,808,809]]`
**Block 144** (798–799): `[798,799]`
**Block 145** (806–807): `[806,807]`
**Block 146** (811–812): `[811,812]`
**Block 147** (813–815): `[813,814,815]`
**Block 148** (816–817): `[816,817]`
**Block 149** (821–838): `[[821,.],[837,838]]`
**Block 150** (828–829): `[828,829]`

## Appendix C — Exact duplicate tiles (interchangeable)

- 248 = 254
- 249 = 255
- 250 = 257
- 362 = 371
- 363 = 372
- 387 = 396
- 390 = 569
- 412 = 416
- 413 = 616
- 419 = 439
- 420 = 572
- 841 = 842 = 843 = 844 = 845 = 846 = 847 (blank white filler)

---

## Notes for the importer

- **Orientation:** the level's `PaperTileMapActor` renders "north-up" only when viewed with the debug camera yawed +90° (pitch −90). Text-map `[map]` rows still map top→bottom, cols left→right in tile space; verify final look via **Play** (game camera) or a yaw-90 top-down capture.
- Multi-tile stamps: place each block's tiles at the exact relative offsets in Appendix B. `.` cells in a block are *not* part of it — fill with surrounding terrain.
- Overlay props (signs, mailbox, manhole, lone flowers/bushes) can go on any grass/pavement cell.
