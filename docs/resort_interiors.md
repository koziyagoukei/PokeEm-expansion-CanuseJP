# Resort interior renovation

The hub, diner, bath and villa shop now have purpose-built static interiors.
NPC identities, positions, scripts, services, dialogue and warp IDs are retained.
The furnished local room is not rebuilt or reset by this change.

## Layouts and art

- Hub: service table, waiting table, guide desk, television corner, windows and
  plants around a clear central entrance aisle.
- Diner: complete kitchen fixtures, serving counter, party-Pokemon floor space,
  two dining tables and an unobstructed entrance.
- Bath: two rock pools, wooden circulation deck, five existing contest paintings
  on a continuous museum wall, and a complete north doorway.
- Villa shop: a private layout with a clear aisle to both existing clerks.
- South district: remove the original museum's clipped roof-adjacent terrain and
  cliff fragments from its new, level plaza.

`gTileset_ResortHouse` retains the original Emerald house art and composes
furniture over either wooden flooring or carpet. It imports a complete plant
into eight unused graphics tiles and uses otherwise-unused palette bank 12.
The original house and shop tilesets are unchanged. Stair side pieces and
isolated furniture fragments are no longer used as repeating room decoration.

The private bath tileset imports museum wall/door tiles into unused graphics
slots. Its pool rims retain the original rock/water foreground; the outside
ground is wooden deck rather than the original grass/sand. Palette banks are:
6 house/deck, 7 and 12 museum frames/walls, 8 original water, 9 museum doorway,
11 solid black boundary. Steam animation tile slots remain reserved.

## Existing saves

`Special_RestoreResortInterior` runs on load only in the four static rooms. It
restores the expected layout and replaces old cached map cells before painting
flags are applied. When a saved player position now falls inside solid furniture,
it moves the player to a safe aisle. Valid doorway arrivals are retained.

No save structures, furniture records, flags, link packet layouts or compatibility
versions were added or changed. The private shop layout is appended to the layout
list; the ordinary Mart layout is not edited. EWRAM remains 227,532 bytes and
IWRAM remains 28,592 bytes.

## Regeneration

These are explicit maintenance commands, not automatic build steps. Running the
map generator replaces manual edits to the four generated room layouts.

```sh
python3 tools/build_bath_gallery.py
python3 tools/resort_interiors/build_maps.py --preview workspace/resort_interiors/after
python3 tools/frontier_district/build_map.py
python3 tools/resort_interiors/build_maps.py --check
python3 tools/resort_interiors/test_maps.py -v
python3 tools/frontier_district/test_maps.py -v
make -j4 --output-sync=target
```

The bath asset generator retains the current map by default. Its optional
`--legacy-layout` switch explicitly reconstructs the old full-pool layout.

## Validation

- Full ROM build passed; ROM usage is 26,725,400 bytes before padding.
- Interior and district layout checks cover dimensions, tile ranges, spawn cells,
  reachable services/doors, private asset allocation, and existing connections.
- Japanese text and link fingerprint checks passed (compat 12, data 6).
- Headless mGBA displayed all four rooms and returned from the START menu.
- Simulated old cached floors and blocked saved positions recovered in all four
  rooms, including the shop's previous shared Mart layout.
- Walking through all four exits reached the expected outdoor map.
- The original diner conversation opened across the counter.
- An empty painting frame and a temporarily populated Rayquaza painting opened
  and returned to the bath successfully.
- Additional camera positions covered both halves of the hub, both pools, and
  the relocated museum's entrance. Screenshots were visually inspected.
- The emulator loaded the save into temporary memory. The source save's SHA-256
  was unchanged after testing.

All meal/purchase variants and long play sessions were not replayed in this
renovation pass. Their event logic is unchanged.

## Changed files

```text
data/layouts/BattleFrontier_OutsideSouth/map.bin
data/layouts/MiniMiniVilla_Shop/border.bin
data/layouts/MiniMiniVilla_Shop/map.bin
data/layouts/Party_eat_time/map.bin
data/layouts/Partybathtime/border.bin
data/layouts/Partybathtime/map.bin
data/layouts/layouts.json
data/layouts/ultra_frontie_hub/map.bin
data/maps/MiniMiniVilla_Shop/map.json
data/maps/MiniMiniVilla_Shop/scripts.inc
data/maps/Party_eat_time/scripts.inc
data/maps/Partybathtime/scripts.inc
data/maps/ultra_frontie_hub/scripts.inc
data/specials.inc
data/tilesets/secondary/party_bath/metatile_attributes.bin
data/tilesets/secondary/party_bath/metatiles.bin
data/tilesets/secondary/party_bath/tiles.png
data/tilesets/secondary/resort_house/metatile_attributes.bin
data/tilesets/secondary/resort_house/metatiles.bin
data/tilesets/secondary/resort_house/tiles.png
include/constants/metatile_labels.h
src/data/tilesets/graphics.h
src/data/tilesets/headers.h
src/data/tilesets/metatiles.h
src/resort_interiors.c
tools/build_bath_gallery.py
tools/frontier_district/build_map.py
tools/frontier_district/test_maps.py
tools/resort_interiors/build_maps.py
tools/resort_interiors/build_tileset.py
tools/resort_interiors/test_maps.py
docs/bath_gallery.md
docs/resort_interiors.md
```

ROMs, previews and the temporary emulator QA harness under `build/` are generated
artifacts, not additional game source changes. Story planning is not included.
