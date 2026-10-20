# Frontier battle music

## Player behavior

- The bottom-right music fan in `ultra_frontie_hub` manages the collection.
- Each of the 21 compiled battle tracks costs 5 BP to unlock. A confirmation is required.
- Unlocking a track also selects it. Selecting an owned track or changing playback mode is free.
- Random mode draws from owned tracks once per battle; consecutive repeats are possible.
- With no owned tracks, random mode is rejected. Cancelling never charges BP.
- Restoring the default BGM keeps all owned tracks.
- Frontier Brain battles always use `MUS_VS_FRONTIER_BRAIN`, including when an explicit song is passed to a battle-music entry point.
- Normal trainer battles in all seven Frontier facilities, including doubles and NPC multi, use the setting.
- Wild encounters, non-Frontier trainer battles, link battles (including Tower link multi), and recorded playback retain their existing music.

The catalog contains the Hoenn wild/trainer/Gym/rival/Elite Four/champion,
Aqua/Magma and their leader, Regi, Kyogre/Groudon, Rayquaza, Mew, and Frontier
Brain tracks; the Kanto wild/trainer/Gym/champion/legendary, Mewtwo, and Deoxys
tracks; and the compiled unused legendary-beast battle track. Intro,
encounter, victory, and fanfare music are not full battle tracks and are excluded.

## Implementation boundaries

`FrontierMusic_ChooseBattleBGM` is invoked by the three existing battle-music
entry points in `pokemon.c`. The Dome's delayed task stores the chosen song
at creation and does not draw again during execution. Random selection uses
the secondary RNG, not the gameplay/trainer-generation stream `gRngValue`.
No battle calculation, party generation, or general map-music dispatch is changed.

`SaveBlock3.frontierMusic` is appended at offset 152, increasing that block
from 152 to 168 bytes. Existing fields retain their offsets. The new 16-byte
record has a magic value, version, unlock bits, selected song, mode, and CRC.
Absent, incompatible, or corrupt extension data initializes to an empty
collection with default playback. New games already clear SaveBlock3.
Normal saving persists the record; purchases do not force an automatic save.

Catalog indices are saved unlock-bit positions: do not reorder existing entries.
The selected song is stored as its song ID. SaveBlock3 is not a link or
record-mixing payload. No communication structure or compatibility version changes.

The NPC's old Porymap visual-event metadata was removed because it described
the previous dialogue-only script; its existing map-event label is retained.

## Verification

- `make -j4 --output-sync=target`: passed.
- `make -j4 --output-sync=target check 'TESTS=Frontier music'`: 18 passed.
- `make -j4 --output-sync=target check 'TESTS=Local room:'`: 30 passed.
- `make -j4 --output-sync=target check 'TESTS=Special dolls:'`: 14 passed.
- Japanese text and link compatibility fingerprint checks: passed; compat 10/data 6 unchanged.
- mGBA, temporary in-memory copy of an existing save: NPC interaction, list
  rendering, 5-BP purchase, insufficient-BP rejection, random selection, and
  save/soft-reset/continue persistence confirmed. Original `.sav` untouched.
- Actual music IDs at immediate and delayed battle entry are covered by tests.
  Complete facility playthroughs and listening to every track were not performed.

## Changed project files

- `data/event_scripts.s`
- `data/maps/ultra_frontie_hub/scripts.inc`
- `data/specials.inc`
- `include/constants/frontier_music.h` (new)
- `include/frontier_music.h` (new)
- `include/global.h`
- `src/frontier_music.c` (new)
- `src/pokemon.c`
- `test/frontier_music.c` (new)
- `test/local_room.c`
- `test/special_doll.c`
- `docs/frontier_music.md` (this document)

Generated ROM and ignored QA artifacts are under `pokeemerald.gba` and
`build/frontier_music_qa*`; they are not additional game-source changes.
