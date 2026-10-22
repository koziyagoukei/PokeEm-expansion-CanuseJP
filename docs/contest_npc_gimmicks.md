# Normal-contest NPC gimmicks

## Scope and data ownership

`src/data/contest_npc_presets.h` overrides the 96 original NPC entries only in
normal contests. Trainer names, overworld graphics, ranks, AI script flags and
pre-/postgame availability still come from `gContestOpponents`.

Each override explicitly defines species, four base moves, held item, Tera type,
G-Max factor, eligible contest categories, condition and intended gimmick.
Changed species receive their own species name instead of the old species'
nickname. Entries of the same species retain their original nicknames.

`SetContestants()` applies the preset before the contest starts.
`InitNpcContestGimmickStatus()` reads item, Tera type, Dynamax level and G-Max
factor from the selected contestant after resource initialization clears state.
Four temporary preset IDs identify the selected strategies; no save layout or
link packet is extended.

## Strategy

- Normal rank introduces Tera, Z and Dynamax with lower conditions.
- Super rank introduces Mega Evolution and more setup/combo chains.
- Hyper and Master add stronger conditions, G-Max, signature Z appeals and Ultra
  Burst. Conditions are not universally maximized.
- Normal-rank Tera activates on appeal 2; other Tera and Mega presets use appeal
  1. Non-Stellar Tera types are matched to the entered contest category, including
  entrants eligible for multiple categories.
- Z presets use appeal 3, or the next available turn if unable to act then. The
  AI selects a compatible base move and favors appeal value, matching category
  and signature Z bonuses.
- Dynamax starts on appeal 2 and lasts for three appeals. Move selection accounts
  for its fixed appeal, removed jam and G-Max bonus rather than trusting the old
  move's appeal/jam alone. Machamp and Meowth explicitly have the G-Max factor.
- Ultra Necrozma's preset starts as Dusk Mane Necrozma with Ultranecrozium Z. It
  spends appeal 1 on Ultra Burst and can use Photon Geyser's signature Z appeal
  from appeal 3 onward. Sunsteel Strike is legal through the fusion table.
- Mega/Ultra entrants favor matching categories for the persistent bonus. The
  existing rule suppressing that bonus after receiving jam is unchanged.

The original contest AI still supplies combo, repeat, order and effect scores;
normal-only scoring adjusts those scores for active/planned gimmicks. A preset
does not randomly fall back to another gimmick. One gimmick per contest remains
the rule, with only the existing Ultra Burst -> Z appeal exception.

## Isolation

`SetLinkAIContestants()` continues to use the untouched original roster. Linked
NPCs retain their previous initialization and do not select gimmicks. Linked
human participants retain their existing gimmick behavior. Link compatibility
versions and fingerprint data do not change.

The battle-view sandbox and its animation state are unchanged. `currMove` and
contest calculations still receive base moves; only the animation path resolves
Z/Max/G-Max move IDs. Ordinary battles are not modified.

## Validation

`make -j4 --output-sync=target check TESTS="Contest NPC:"` covers:

- All 96 presets, four distinct legal base moves, valid categories and plans.
- All 40 rank/category/pre-postgame pool combinations contain three opponents.
- Actual normal-contest NPC selection loads three distinct presets.
- Item/Tera/G-Max initialization and every preset's intended activation turn.
- Multi-category Tera matching and Mega's jam-sensitive bonus.
- Generic/signature Z bonuses, excitement, base move preservation and one use.
- Ultra Burst's consumed turn and subsequent one-time signature Z appeal.
- Three G-Max appeals, zero jam, shared hearts and no subsequent gimmick.
- Linked NPC roster/selection remaining unchanged.

Animation rendering and complete contests still require gameplay checks; logic
tests are not a substitute for visual or difficulty playtesting.
