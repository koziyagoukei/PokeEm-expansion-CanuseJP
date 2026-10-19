# Frontier Gen 2-4 Sets

## Scope and Compatibility

`tools/frontier_sets_gen2_4.py` hand-authors 195 species/forms and 577 active
sets, including three new Smeargle sets. Existing entries through
`FRONTIER_MON_SUDOWOODO_3`, all Shedinja entries, and all Gen 5-9 sets are
excluded from the update. Meganium was appended after that prefix but is
before Sudowoodo in species order, so its manual sets are also protected.
Regional forms already curated by the Gen 5-9 tool
remain unchanged. No species is removed from the registered pool.

The updater changes only moves, held item, EVs, nature, IVs, ability and Tera
type. Nicknames, gender, balls, Dynamax level and Gigantamax factors are kept.
Existing numeric set IDs are never renumbered. Smeargle IDs 2037-2039 are
appended. Save structures, Pokemon structures and link packet layouts do not
change. Normal battle mechanics and AI rules are not changed.

## Roles

Each ordinary species has three distinct move sets, not a list of its four
strongest STAB moves. Variants combine immediate offense, setup, recovery,
status, priority, trapping, pivoting and hazard control as appropriate.
Examples include Mega/Choice Band/Occa Scizor, Calm Mind and Rest/Sleep Talk
Suicune, Poison Heal Breloom and Gliscor, and Dragon Dance/Choice Band/Assault
Vest Tyranitar. Low-stat species retain specific roles such as Sticky Web,
Endeavor plus priority, Encore, paralysis or Baton Pass rather than pretending
to be conventional offensive Pokemon.

Weather sweepers generally set their own weather. Assault Vest has no status
moves, Choice items have no setup moves, and Mega stones and Z crystals must
match the species/moves. Hidden Power has explicit IVs for its intended type.
Trick Room and Gyro Ball sets use zero Speed IVs. The current facility creator
uses Tera type zero as an unset sentinel, so these sets avoid Normal Tera;
this update does not alter that shared battle behavior.

Reference roles, adapted rather than copied:
- [Smogon Scizor](https://www.smogon.com/dex/sv/pokemon/scizor/)
- [Smogon Tyranitar](https://www.smogon.com/dex/dp/pokemon/tyranitar/)
- [Smogon Suicune](https://www.smogon.com/dex/sm/pokemon/suicune/)
- [Smogon Smeargle](https://www.smogon.com/dex/sm/pokemon/smeargle/)
- [Smogon Castform](https://www.smogon.com/dex/dp/pokemon/castform/)

## Castform and Smeargle

Each Castform form has one active set: neutral/Tera Blast coverage, Sunny
Day/Weather Ball/Solar Beam, Rain Dance/Weather Ball/Thunder, or Hail/Blizzard
coverage. The weather forms still obey Forecast and may revert at battle
start; their weather moves establish the intended form normally.

Legacy Castform `_2`/`_3` entries contain the same set as `_1` so saved Factory
rentals and Dome records remain readable. New opponent selections, fallback
selections and initial Factory drafts skip these eight retired IDs. They are
not replaced with empty Pokemon or reused for another species.

Smeargle has Spore/web/paralysis support, Shell Smash/Power Trip/Baton Pass,
and Spore/Endeavor/Extreme Speed variants. All moves must pass the same PP and
Sketch-ban restrictions as the Frontier relearner. The creation NPC already
allowed Smeargle and is unchanged.
The Shell Smash variant uses Own Tempo so Moody cannot consume its White Herb
before the intended setup turn.

## Legal Move Sources

Validation combines the checked-in historical learnsets, pre-evolution moves,
event overlay and Z-A overlay. Rotom appliance moves are read from the catalog
definitions in `src/data/party_menu.h`. Smeargle uses Sketch legality rather
than only its ordinary learnset. The existing universal Hidden Power/Tera
Blast policy is retained.

Spinda's historical Dream World Superpower was missing from the overlay.
It is added with a [source](https://www.serebii.net/pokedex-bw/327.shtml), and
the refresh tool retains it. This permits Contrary sets without assigning an
unobtainable move. Giratina-Origin uses Griseous Core for the current Gen 9
form-change setting.

## Maintenance and Verification

```sh
python3 tools/frontier_sets_gen2_4.py --audit
python3 tools/frontier_sets_gen2_4.py --write
python3 tools/frontier_sets_gen2_4.py --check
python3 tools/frontier_sets_gen5_9.py --check
python3 tools/frontier_move/generate_event_learnsets.py --check
python3 tools/test_frontier_sets_gen2_4.py
make -j4 --output-sync=target check 'TESTS=Frontier curated sets:'
make -j4 --output-sync=target
```

Use the incremental updater for this review. The full pool generator now also
knows the new definitions, but a full regeneration can overwrite unrelated
hand-edited entries and is not required here. The updater checks its exact
scope and verifies that protected entries were not rewritten.

Static data validation does not establish gameplay difficulty, optimal AI
move selection, or facility win rates. Those require playtesting with the
project's mixed-generation mechanics and random teammates.

## Changed Files

- `src/data/battle_frontier/battle_frontier_mons.h`
- `include/constants/battle_frontier_mons.h`
- `src/frontier_util.c`
- `include/frontier_util.h`
- `src/battle_factory.c`
- `src/data/pokemon/frontier_event_moves.json`
- `src/data/pokemon/frontier_event_learnsets.h`
- `tools/frontier_sets_gen2_4.py`
- `tools/frontier_sets_gen5_9.py`
- `tools/generate_frontier_pool.py`
- `tools/frontier_move/update_event_moves.py`
- `tools/test_frontier_sets_gen2_4.py`
- `test/frontier_pool_sets.c`
- `docs/frontier_gen2_4_sets.md`

`build/check_frontier_gen2_4_scope.py` is an ignored, QA-only comparison against
the pre-edit Git snapshot. Earlier party-menu warp restriction edits were left
untouched; they are not part of this set review.
