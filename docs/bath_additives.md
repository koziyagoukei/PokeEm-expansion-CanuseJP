# Resort bath additives

The berry plots at `(1..4, 7..9)` in PartyRoamerPark are replaced with ordinary
walkable ground. Their two berry-tree objects are removed. The bath attendant
is an old man at `(21, 5)`, immediately west of the bath entrance.

Talk to him to choose an additive for free. B cancels without changing the
selection. All non-Egg party members receive the active bath effect every
300 overworld task updates (five seconds at 60 FPS).

| Menu entry | Effect per interval | Water color |
| --- | --- | --- |
| スコヴィランのかおり | Cool +1, Sheen +1 | Red |
| ゆきみおんせんのもと | Tough +1, Sheen +1 | Yellow |
| カンポーやくゆ | Smart +1, Sheen +1 | Green |
| グラシデアエッセンス | Cute +1, Sheen +1 | Pink |
| ソルティジオヅム | Beauty +1, Sheen +1 | Blue |
| フエンかけながし | Friendship +1 | Original color |

The five additives replace the friendship effect. Each stat independently caps
at 255; full Sheen does not prevent the selected condition from increasing.
Fainted Pokemon are eligible, as in the previous friendship bath. Eggs and empty
party slots are excluded. Time spent in a separate menu does not count toward
an overworld interval. Reopening the field starts a new interval.

## Save and rendering

- `VAR_PARTY_BATH_ADDITIVE` reuses unused variable `0x40F8`; zero is the original
  friendship bath. Invalid values fall back to the original bath. Save block
  sizes and offsets do not change.
- The camp's on-load script removes plots from an old cached map view. Saved
  object templates containing the removed berry objects are refreshed before
  object spawning. Party roamer local IDs 1 through 6 stay unchanged.
- The bath's effect task is started in `MAP_SCRIPT_ON_RESUME`. The old
  on-transition start was lost to `ResetTasks()` during loading. Resume also
  restarts it after the Bag, summary and painting screens; duplicate starts do
  not add another task. The task destroys itself after leaving the bath.
- BG bank 8 colors 9 and 10 supply the water; only those entries are replaced
  during the bath's palette load. Rocks, walls and museum painting frame banks
  are preserved. The shared field-effect palette's water color is also matched
  when a submerged sprite is created in this bath, avoiding a blue rectangle
  below the player or Pokemon. Other maps use the existing rendering path.
- TIPS removes the obsolete berry-growing instructions and explains the bath
  choices and the condition/Sheen effect.

## Verification

- `make -j4 --output-sync=target`: passed; `pokeemerald.gba` rebuilt.
- `make -j4 --output-sync=target check TESTS=Party`: 11 tests passed, including
  six bath tests with parameterized additive cases and existing resort warp
  restrictions.
- Tests cover the first 299 frames vs. the 300th, all party members, Eggs,
  capped stats, duplicate timers, resume, leaving the map, B/invalid choices,
  palette isolation/skipFaded and legacy camp templates.
- Headless mGBA confirmed NPC selection/cancellation, all six water colors and
  corresponding stat changes, Bag and painting returns with exactly one effect
  task, and the cleared plot area.
- `build/bath_additives_colors.png` shows the six bath colors.
  `build/bath_additives_qa.log` records the live stat checks.
- QA uses `loadTemporarySave`; the user's actual save is not modified.
- EWRAM remains 227,532 bytes.

## Changed files

- `data/layouts/PartyRoamerPark/map.bin`
- `data/maps/PartyRoamerPark/map.json`
- `data/maps/PartyRoamerPark/scripts.inc`
- `data/maps/Partybathtime/scripts.inc`
- `data/specials.inc`
- `include/constants/party_roamer_park.h`
- `include/constants/script_menu.h`
- `include/constants/vars.h`
- `include/party_roamer_park.h`
- `src/data/resort_tips.h`
- `src/data/script_menu.h`
- `src/field_effect_helpers.c`
- `src/fieldmap.c`
- `src/party_roamer_park.c`
- `test/party_bath.c`
- `docs/bath_additives.md`
