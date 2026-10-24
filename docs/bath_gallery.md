# Resort bath contest gallery

The north wall of `Partybathtime` has five two-metatile frames. From left to
right they display Cool, Beauty, Cute, Smart and Tough paintings. Stand below a
frame and press A. Both halves of every frame can be inspected.

The gallery reads the same `contestWinners[]` museum slots and
`FLAG_*_PAINTING_MADE` flags as Lilycove Museum. An unearned category has an empty
frame; after the artist deposits a painting, re-entering the bath displays it.
The existing museum painting screen, captions, artwork effects and return-to-map
callback are reused. There is one current painting per category, shared by both
locations, without additional save data.

## Artist availability

The existing result ceremony sets the artist-ready state after a player wins
Master Rank with at least 800 total points. The lobby now runs that event even
if the player has never met the Lilycove Museum curator. The original approach,
painting preview, deposit confirmation, Artist Ribbon and painting flags remain
in use for both normal and linked contests.

Choose Yes when the artist asks to take the painting to the museum. The saved
painting then becomes available in both the museum and the bath. Species,
personality, names and shiny status are preserved; shiny status was previously
omitted when copying a winner into a saved museum slot.

## Assets and existing saves

`Partybathtime` has a private layout and secondary tileset, retaining the old
20x20 bath, exit and roaming-party events. Museum frame tiles are appended after
the Lavaridge graphics. Steam animation tile indices are unchanged. Museum
palettes use banks 7 and 12, which are unused by the bath terrain.

The new layout ID is appended to `layouts.json`. The bath's on-load script also
sets that layout and resets all ten frame metatiles before applying saved flags.
This supports saves made inside the bath when its saved layout ID still referred
to Lavaridge. Other maps using Lavaridge keep their original layout and tileset.

Regenerate the committed binary/PNG assets with:

```text
python tools/build_bath_gallery.py
python tools/resort_interiors/build_maps.py
```

The bath generator now updates the private tileset without replacing the map.
The interior generator builds the renovated pools, deck and gallery wall. Use
`--legacy-layout` on the bath generator only to recreate the former full-pool
layout. See [resort_interiors.md](resort_interiors.md) for the renovation's
palette allocation, save-cache handling and validation.

## Validation

- `make -j4 --output-sync=target` passed.
- `make -j4 --output-sync=target check TESTS=Contest`: all 68 tests passed,
  including saving both shiny and non-shiny museum winners.
- Headless mGBA opened all five categories and returned to the bath.
- Empty frames showed the original museum frame message.
- Restoring the legacy Lavaridge layout ID and continuing the game restored the
  bath gallery successfully.
- With the curator flag clear, a qualifying Master Rank result caused the artist
  to approach the player. The preview, deposit, shiny winner save and subsequent
  bath inspection completed successfully.
- The mGBA driver loads `pokeemerald.sav` with `loadTemporarySave`; it never
  flushes the temporary changes to the user's save.

Runtime artifacts are in `build/bath_gallery_qa.log` and
`build/bath_gallery_*.png`. The source save's SHA-256 is checked separately.
EWRAM remains 227,532 bytes.

## Changed files

- `data/layouts/layouts.json`
- `data/layouts/Partybathtime/border.bin`
- `data/layouts/Partybathtime/map.bin`
- `data/maps/Partybathtime/map.json`
- `data/maps/Partybathtime/scripts.inc`
- `data/maps/LilycoveCity_ContestLobby/scripts.inc`
- `data/tilesets/secondary/party_bath/tiles.png`
- `data/tilesets/secondary/party_bath/metatiles.bin`
- `data/tilesets/secondary/party_bath/metatile_attributes.bin`
- `include/constants/metatile_labels.h`
- `src/data/tilesets/graphics.h`
- `src/data/tilesets/headers.h`
- `src/data/tilesets/metatiles.h`
- `src/contest.c` (saved painting shiny flag only in this change)
- `test/contest_npc.c` (saved painting regression test)
- `tools/build_bath_gallery.py`
- `docs/bath_gallery.md`

Earlier uncommitted contest-AI changes are retained separately. Build-generated
headers, ROMs, logs and the temporary QA driver are not source changes.
