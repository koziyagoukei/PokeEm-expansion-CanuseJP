# Restoration Inn

Original Gen III-style coastal ryokan assets and four isolated maps for future
restored-person events. No ferry, NPC, story flag, encounter or existing-map warp
is added. Existing map groups and layout IDs retain their order; the new entries
are appended. There is no new save-data field.

## Maps

| Map | Size | Purpose |
| --- | --- | --- |
| `RestorationInn_Outside` | 40 x 36 | Two-storey inn, garden and future arrival pier |
| `RestorationInn_Lobby` | 32 x 22 | Reception and shared tatami lounge |
| `RestorationInn_EastWing` | 34 x 24 | Four rooms, twelve futons |
| `RestorationInn_WestWing` | 34 x 24 | Four rooms, twelve futons |

Outside connects only to the lobby. The lobby connects to both wings. There is
deliberately no way to enter from the existing world until a later event adds
one. Open these map names in Porymap, or use the existing debug warp facility.

## Assets

The two secondary tilesets use `General` and `Building` as their respective
primary tilesets. Each has 67 metatiles, one 16-color palette in bank 6 and fewer
than 512 8x8 tiles. Palette index 0 is transparent. No unrelated animation
callback is inherited. Walls, furniture, roof and water have explicit collision.

Source artwork and previews live in `graphics/custom/restoration_inn/`.
The production files are in
`data/tilesets/secondary/restoration_inn_exterior/` and
`data/tilesets/secondary/restoration_inn_interior/`.

The original images were created with the built-in image generation tool;
`PROMPTS.md` records their generation specification. `assets.mjs` deterministically
samples those images, quantizes to fixed palettes, deduplicates 8x8 tiles and
builds the metatiles. This is not an extraction of commercial game tiles.

### Local Metatile IDs

Exterior rows of eight: 0-7 ground/water; 8-15 roof; 16-23 eaves/gable;
24-31 walls/windows; 32-39 entrance/steps; 40-47 rail/lantern/sign/basin;
48-55 bamboo/rocks/leaves; 56-63 pier/stone edge.

Interior rows: 0-7 floors/wall; 8-15 walls/windows; 16-23 doors/stairs;
24-31 counter/furnishings; 32-39 futon/table and spare furnishings;
40-47 cushions/cabinets/scroll/lamp; 48-55 bookcase/alcove/basin;
56-63 corners/noren/frame/shoe cabinet. IDs 32 and 33 form a 16x32 futon;
34 is a low table, 24-26 a counter and 52-54 an alcove.

Both tilesets reserve 64 for a non-animated north door, 65 for a south-arrow
exit mat, and 66 for the solid dark surround. Runtime metatile ID = local ID+512.

## Regeneration

With Node.js and `pngjs` available:

```js
import { PNG } from 'pngjs';
import { prepareAssets, renderMap } from './tools/restoration_inn/assets.mjs';
const root = process.cwd();
prepareAssets(PNG, root);
for (const name of ['RestorationInn_Outside', 'RestorationInn_Lobby',
                    'RestorationInn_EastWing', 'RestorationInn_WestWing']) {
    renderMap(PNG, root, name, `${root}/graphics/custom/restoration_inn/${name}.png`);
}
```

```sh
python3 tools/restoration_inn/build_maps.py
python3 tools/restoration_inn/build_maps.py --check
python3 -m unittest discover -s tools/restoration_inn -p test_maps.py -v
make -j$(nproc) check TESTS=test/restoration_inn.c
make -j$(nproc)
```

The map generator writes only its four new map/layout directories. Registration
in `map_groups.json`, `layouts.json` and the tileset/script include files is
maintained separately. The Python tests check connectivity, collision, warp
targets, hardware limits, deterministic output and isolation. The C tests check
the actual compiled map headers, warp metadata and metatile behaviors.
