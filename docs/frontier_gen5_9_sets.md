# Frontier Gen 5-9 Sets

The registered pool contains 397 targeted species/forms and 1,191 sets. This
includes new evolutions defined in earlier-generation family headers and the
Alolan, Galarian, Hisuian, and Paldean regional forms. The registered species,
set IDs, nicknames, gender policy, Dynamax flags, and Gigantamax factors are
preserved. This update changes opponent set data rather than battle rules/AI.

## Design

Each species has three hand-authored sets in `tools/frontier_sets_gen5_9.py`.
Coverage, setup, priority, recovery, status, and pivoting replace redundant
high-power attacks. Sets use the species' appropriate offensive stat and
ability. Weather users normally supply their own weather rather than require
a specific randomly selected teammate. Choice items are used for immediate
offense; Assault Vest sets have no status moves; Z/Mega items match the user.

Double STAB remains when the moves serve different purposes: a priority move,
Freeze-Dry coverage, a repeatable alternative to Blood Moon/Overheat, or a
high-HP attack such as Eruption/Dragon Energy. Cosmetic forms share sets;
forms with different stats, abilities, or move pools have separate definitions.

Examples include Contrary Leaf Storm/Glare Serperior, three-attack or Swords
Dance Samurott, Nasty Plot/Recover and Choice Scarf Gholdengo, and bulky/setup
Kingambit with Sucker Punch. Hidden Power types are encoded in the actual IVs.
Special attackers use zero Attack IVs unless a physical move requires Attack;
Trick Room/Gyro Ball sets use zero Speed IVs.

Strategy references: [Smogon Serperior](https://www.smogon.com/dex/sm/pokemon/serperior/),
[Smogon Gholdengo National Dex](https://www.smogon.com/dex/sv/pokemon/gholdengo/national-dex/),
[Smogon Kingambit Battle Stadium Singles](https://www.smogon.com/dex/sv/pokemon/kingambit/battle-stadium-singles/),
and [Pokemon Tettei Kouryaku Serperior](https://yakkun.com/sv/theory/p497).
These inform roles; the sets are adapted to random Frontier teams and the
project's combined-generation mechanics rather than copied as team templates.

## Maintenance

```sh
python3 tools/frontier_sets_gen5_9.py --audit
python3 tools/frontier_sets_gen5_9.py --write
python3 tools/frontier_sets_gen5_9.py --check
```

`--write` updates only the seven curated fields in existing entries. It does
not regenerate the species pool or renumber constants. The full pool generator
also uses these definitions, so later regeneration retains the curated sets.

Validation checks all three variants, move/ability/item existence, current and
historical learnsets including pre-evolution moves, form-specific fusion moves,
Z/Mega compatibility, item restrictions, EV totals, and offensive EV alignment.
The Frontier tutor's universal Hidden Power/Tera Blast policy is included.
Adding a new registered Gen 5-9 species requires adding its three sets here.

Build validation does not establish optimal AI play or win rates. Those depend
on facility rules, randomly selected teammates, opponents, and gimmick choice.
