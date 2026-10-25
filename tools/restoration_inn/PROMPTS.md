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
