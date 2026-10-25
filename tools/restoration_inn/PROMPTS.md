# Adopted Asset Generation Specification

Mode: built-in image generation, transparent background. One exterior and one
interior image. The following records the adopted prompt specifications rather
than a byte-for-byte tool transcript. Original generated PNGs are retained in
`graphics/custom/restoration_inn/source/`.

## Shared Style

Create an original tileset for a large, old but cared-for Japanese ryokan in a
Pokemon Emerald / Generation III overworld style. Use a strict orthogonal
top-down, slightly front-facing map perspective, not isometric. Arrange 64
metatiles in an 8x8 grid: logical atlas 128x128, each cell exactly 16x16 logical
pixels, enlarged with hard square pixels for inspection. Simple readable shapes,
dark outlines, flat restricted colors and restrained highlights. No gradients,
anti-aliasing, photorealism, HD texture, perspective rendering, people, Pokemon,
boats, lettering, logos, watermarks or grid labels. Tiles must join cleanly.

## Exterior

Use the existing Lilycove tiles as a style/scale reference, not as content to
copy. Grey-blue tiled roof, weathered timber and plaster, paper windows, faded
red noren, moss, bamboo and a small coastal garden. 15 opaque colors plus
transparency. Eight cells per row:

1. Grass, dark grass, stone path, dirt, wood floor, stone edge, teal water, dark water.
2. Roof caps/middle, ridge left/middle/right, plain and mossy roof.
3. Eaves left/middle/right, gable left/middle/right, roof sides.
4. Plaster, timber post, window halves, lower timber wall, pillar, dark wood, worn plaster.
5. Entrance left/center/right, closed door halves, threshold, step, small sign.
6. Veranda railing left/middle/right, post, lantern top/base, signpost, basin.
7. Four bamboo quadrants, shrub, big rock, small rock, leaves.
8. Pier sides/floor/end, bollard, stone wall left/middle/right.

## Interior

Use the existing Building tiles as a scale/style reference and the adopted
exterior for matching materials. Old timber corridors, green tatami, shoji,
paper walls, low tables, blue/white futons, red/blue cushions, a reception desk,
shoe cabinets and a tokonoma alcove. Eight cells per row:

1. Horizontal/vertical wood floor, tatami variants/borders, threshold, paper wall.
2. Upper/lower wall, post, worn plaster, shoji halves, screen, crossbeam.
3. Door halves, open door halves, threshold, stairs up/down, railing.
4. Reception desk top/front sections, cabinet, tea kettle.
5. Futon and low-table quadrants.
6. Red/blue cushions, chest, tansu halves, blank scroll, flowers, paper lantern.
7. Bookcase quadrants, alcove left/middle/right, basin.
8. Wall corners, standing lamp, noren halves, door-frame halves, shoe cabinet.

The generated atlas is an art source, not a valid GBA binary by itself. The
checked-in asset pipeline normalizes selected cells, limits palettes, composes
opaque floor/wall backgrounds and creates the actual 8x8/16x16 production data.

## Exterior Seam Repair (2026-10-26)

Built-in image editing tool. Edit target: `source/exterior_generated.png`.
Supporting reference: `RestorationInn_Outside.png` before the repair, used only
to identify the unwanted square seams. Adopted output:
`source/exterior_seamless_generated.png`. Exact prompt:

```text
Edit image 1, an existing 8 by 8 tile atlas for a Japanese ryokan in a Pokemon Emerald Generation III overworld. Image 2 is the current assembled map and is ONLY evidence of the unwanted square seams; do not return a map. Return only a replacement atlas with EXACTLY the same 8x8 arrangement and identical cell meanings/positions as image 1. Keep the old timber ryokan, grey-blue tiled roofs, faded red noren, bamboo, lanterns and dock. Keep a restrained GBA pixel-art look: crisp square pixels, flat colors, no antialiasing, no gradients, no realistic texture.

CRITICAL REPAIR: the original atlas contains thin dark or brown separator lines at the edges of the cells. Completely remove ALL atlas separator/grid lines. There must be NO presentation grid, gutter, border, outlines around individual floor tiles, or boxed swatches. The entire canvas is occupied by art to its edges. Every repeating material cell must be a seamless texture, not an outlined square.

First row meanings stay: short grass, darker grass, irregular small stone path with sparse moss, dirt, wood deck planks, stone edge, light teal water, dark teal water. Short grass, stone path, dirt, wood planks and water must be repeatable in BOTH directions with NO dark pixel edge or artificial border on any side. Stone path should have small irregular stones, not a big square slab, with edges that do not form a 16-pixel lattice. Water waves must cross tile boundaries naturally. Wood planks should have horizontal grain with staggered joints, not a square frame or vertical edge at every metatile.

Roof cells must join smoothly into one roof. Keep real small curved roof-tile lines, but remove straight black square frames at the 16-pixel atlas cell edges. The plain repeating roof surface (row 2, column 7, counting from 1) must repeat smoothly across both axes; mossy variant (row 2, column 8) shares the same edge pixels.
Plain plaster wall (row 4, column 1) must not have a dark vertical border or bottom frame: the separate timber-post cell provides the posts. The lower wooden wall / wainscot cells (row 4, column 5 and row 5, columns 4 and 5) must be continuous vertical wood slats without a rectangular frame at every cell. Keep genuine window frames, doorway geometry, noren and deliberate corner/endcaps.

All other 64 cell subjects and their locations must be preserved. Do not introduce new objects, people, Pokemon, letters, grid lines, labels, shadows outside each object, or text. This is production tile art, not a contact sheet. Simplify tiny detail rather than add noise. Match the original colors closely.
```
