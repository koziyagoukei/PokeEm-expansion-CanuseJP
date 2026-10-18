# Battle Pyramid balance rules

These rules apply only to `BATTLE_TYPE_PYRAMID`. They do not replace the shared
Frontier Pokemon pool, change the saved Pyramid bag, or change link layouts.

## Gimmicks

- Both sides may use Z-Moves and Terastallization only. Mega Evolution,
  Primal Reversion, Ultra Burst and Dynamax are blocked.
- Each side has one Z-Move and one Terastallization per battle, including
  encounters with two separate NPC trainers. A Pokemon cannot use both.
- The facility lends the ring/orb capability without adding key items to the
  ten-slot bag. Z-Moves still require the matching held crystal and a usable
  move. Holding a Z-Crystal still prevents Terastallization.
- Terastallization neither requires nor consumes the field orb's charge.

## Opponents

`PreparePyramidTrainerBattle` runs once after the complete opponent party is
created, for both floor encounters and Brandon. It normalizes items and then
applies the existing Frontier IV reduction (30/20/10/0 for rounds 1-2/3-4/5-6/7+).
The old fourth-floor guaranteed Mega/Z selection does not apply to the Pyramid.

Normal NPCs have a 20% gimmick-candidate chance on floors 1-3 and 40% on floors
4-7. A candidate is assigned a matching Z-Crystal or its primary Tera type;
the opposing side gets at most one of each. Actual use still depends on move
eligibility and the existing AI. Other held items are replaced with nothing or,
at 50%, an Oran/Cheri/Pecha/Rawst/Chesto Berry. Brandon gets one Z and
one Tera candidate when its sets support them. The remaining slot has no gimmick.

Rates are centralized in `include/constants/battle_pyramid.h`. The battle-start
Frontier marker refresh deliberately leaves these normalized parties intact.

## Loot

- The first item object on the first floor of each seven-floor challenge is
  a usable crystal, if the saved entry party has a matching move.
- Later floor crystals replace held battle equipment at a 10% rate, not healing
  medicine or berries. The guarantee may replace one ordinary item on floor 1.
- Pickup also has a 10% crystal opportunity. A crystal already in the bag or
  held by the current party falls back to the original Pickup reward.
- All 18 type crystals and matching signature crystals are eligible;
  Ultranecrozium Z is excluded. Crystals are reusable.
- Fixed loot selection uses the existing saved floor seed and saved entry
  party. Opening the bag, reordering the party, failing a pickup or resuming
  the same floor does not reroll it. Full-bag handling and removal of item
  objects continue to use the existing scripts.
- The bag remains ten distinct stacks. Starting medicine is unchanged.

## Wild encounters

Run `python3 tools/generate_pyramid_encounters.py` to regenerate the two wild
headers, or add `--check` to validate them without writing. The curated species
and themes are in that script. It uses the existing species/move parsers but
does not invoke the shared Frontier pool generator's write functions.

There are 20 rounds, eight weighted slots per round and 149 distinct species.
Each round has at least four post-Hoenn species or regional forms. Legendary,
sublegendary, mythical, Ultra Beast, Paradox and battle-only forms are excluded.
The slot weights, encounter rates and floor layouts remain unchanged.

Level-50 base levels range from 35 to 50, with the existing +/-5 variation.
Open Level uses equivalent offsets from the facility level. Actual levels are
clamped to 1-100. Tables select level-up moves legal at the minimum level and
always include an ordinary damaging move; no competitive EVs are added.
Generation-dependent learnset changes fall back to the species' initial moves.
Wild Pokemon are recreated from their real species and healed to full HP,
instead of retaining derived fields from the placeholder encounter.

## Validation commands

```sh
python3 tools/generate_pyramid_encounters.py --check
python3 tools/link_compat/check_fingerprint.py
make -j4 --output-sync=target
make check -j4 --output-sync=target TESTS='Pyramid:'
make check -j4 --output-sync=target TESTS='Frontier IV penalty:'
```

Manual checks still needed: actual floor traversal, giving/taking crystals in
the Pyramid bag UI, full-bag pickup/retry, suspend/save/reload and exit restoration,
paired-NPC encounters, Brandon, and a complete seven-floor difficulty playtest.
Do not use old emulator save states to validate a newly built ROM; resume from
an in-game save. No save migration is required by these changes.
