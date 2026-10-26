# Frontier Title Still Life

The title background is a quiet plate of four square brown Hokkaido-style
kibidango beside a stylized elderly hand. It deliberately uses flat color
clusters, concise age lines and no dithering or realistic skin/wood texture.
The generated source was redrawn after the user rejected the grainier draft.
Generation used the built-in image tool; the final prompt set is in `PROMPTS.md`.
The manufacturer's [anniversary booklet](https://www.kibidango.co.jp/history/magazine.pdf)
was consulted for the regional confection's shape, not copied as image data.

## Files and Runtime

- `source/generated.png`: final source illustration, not used directly by GBA.
- `still.png` / `still_4x.png`: native 240x160 and integer-enlarged previews.
- `still.tiles.bin`: deduplicated 4bpp tiles, including an empty tile zero.
- `still.map.bin`: 32x32 text tilemap, palette bank 15.
- `still.palette.bin`: fixed 16-color BGR555 palette.
- `validation.json`: sizes, tile count, palette bank and combined SHA-256.

`src/title_screen.c` loads these into BG0, character base 2 / screen block 26.
The tile limit is 0x5000 bytes so tiles cannot overwrite that map. The existing
8bpp logo is checked not to use palette indices 240..255. No visible art pixel
uses transparent index zero. The background does not scroll or shimmer.

The old Rayquaza/cloud declarations remain available to the original extraction
manifest, but this title no longer loads or displays them. The cloud scanline
effect and the Rayquaza marking-color update are not run. Unused original data
is removed by the existing linker garbage collection.

The logo, version artwork and copyright are unchanged. PRESS START is moved
from y=108 to y=88 so it does not obscure the plate or hand. A/START, the existing
special key combinations, and the normal main-menu/replay callbacks remain.
The separately edited opening movie and its score are not changed.

## Regeneration and Tests

Use Node.js with `pngjs` available. Normal builds use the checked-in binary
assets and do not require Node.js:

```sh
node --input-type=module -e "import {PNG} from 'pngjs'; import {prepareAssets} from './tools/frontier_title/assets.mjs'; prepareAssets(PNG, process.cwd());"
python3 tools/frontier_title/music.py
python3 -m unittest discover -s tools/frontier_title -p test_title.py -v
make -j$(nproc) check TESTS=test/frontier_title.c
make -j$(nproc)
```

Binary filenames end in `.bin` so make does not silently reconvert the preview
PNG through the standard indexed-PNG `.4bpp`/`.gbapal` rules. Do not rename them
to those implicit-build targets without changing the build rules as well.

Music details are in `tools/frontier_title/README.md`. Runtime screenshots,
recording and logs use copies under `build/qa_frontier_title_20261026/`, not the
production save file. Hardware display and speaker balance remain untested.
