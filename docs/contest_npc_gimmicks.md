# Normal-contest NPC gimmicks

## Scope and data ownership

`src/data/contest_npc_presets.h` overrides the 96 original NPC entries only in
normal contests and adds 112 entrants from `src/data/contest_npc_extra.inc`.
The original entries retain trainer names, overworld graphics, ranks, AI script
flags and pre-/postgame availability from `gContestOpponents`. Extra entries
have independent metadata accessed through `Contest_GetNpcTemplate()`.

Each override explicitly defines species, four base moves, held item, Tera type,
G-Max factor, eligible contest categories, condition and intended gimmick.
Changed legacy species receive individual nicknames instead of the old species'
nickname. Entries of the same species retain their original nicknames. Every
extra entrant has an individual Japanese nickname and trainer name.

Each rank adds 25 distinct species from generations 4-9 and regional forms.
Five entrants specialize in each adjacent category pair (Cool/Beauty,
Beauty/Cute, Cute/Smart, Smart/Tough, Tough/Cool), giving **ten additional species
per category per rank even while restricted species are banned**. Species may
enter either of their two specialties; these are not 200 separate entrants.
Master adds another 12 legendary/mythical guests, for **208 total presets**.

| Rank | Ordinary additions | Bonus guests | Extra entrant conditions |
| --- | ---: | ---: | --- |
| Normal | 25 | 0 | 74-85 |
| Super | 25 | 0 | 122-136 |
| Hyper | 25 | 0 | 176-188 |
| Master | 25 | 12 | 226-235; bonus guests 235-238 |

The bonus guests are Zeraora, Solgaleo, Reshiram, Lunala, Xerneas, Diancie,
Magearna, Meloetta, Dawn Wings Necrozma, Giratina, Dialga and Melmetal.
Their entry follows the existing Frontier classification, not a new blanket
ban of all legendary, Ultra Beast or Paradox species.

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
- Ultra Necrozma's presets start as Dusk Mane or Dawn Wings Necrozma with Ultranecrozium Z. They
  spend appeal 1 on Ultra Burst and can use Photon Geyser's signature Z appeal
  from appeal 3 onward. Sunsteel Strike/Moongeist Beam are legal through the fusion table.
- Mega/Ultra entrants favor matching categories for the persistent bonus. The
  existing rule suppressing that bonus after receiving jam is unchanged.

The original contest AI still supplies combo, repeat, order and effect scores;
normal-only scoring adjusts those scores for active/planned gimmicks. A preset
does not randomly fall back to another gimmick. One gimmick per contest remains
the rule, with only the existing Ultra Burst -> Z appeal exception.

## Isolation

`IsFrontierSpeciesBannedByCurrentRules()` gates normal NPC selection only.
The hub's existing `FLAG_FRONTIER_ALLOW_BANNED_SPECIES` controls NPC guests;
fused/alternate forms use the same base-species classification as Frontier.
Player entry permits these species regardless of that flag, in both normal and
linked registration. Egg, fainted-mon and rank/ribbon restrictions still apply.
Rejected entries do not initialize opponents or first-round points. TIPS explains
that only NPC species selection follows the Frontier BAN setting.

`SetLinkAIContestants()` continues to use the untouched original roster. Linked
NPCs retain their previous initialization and do not select gimmicks. Linked
human participants retain their existing gimmick behavior and unrestricted
species eligibility. Link compatibility versions and fingerprint data
do not change. No saved structures or EWRAM fields are added by the roster expansion.

The battle-view sandbox and its animation state are unchanged. `currMove` and
contest calculations still receive base moves; only the animation path resolves
Z/Max/G-Max move IDs. Ordinary battles are not modified.

## Validation

`make -j4 --output-sync=target check TESTS=Contest` covers:

- All 208 presets, four distinct legal base moves, valid categories and plans.
- Ten distinct allowed new species for all 20 rank/category combinations.
- Every new non-Tera/Stellar strategy has matching-category moves for its specialties.
- New nicknames are nonempty, within the name limit and copied unchanged.
- Banned NPCs are excluded; unlocking permits Necrozma in actual selection.
- Player entry permits restricted species, including fused Necrozma, with either NPC BAN setting.
- All 40 rank/category/pre-postgame pool combinations contain three opponents.
- Actual normal-contest NPC selection loads three distinct presets.
- Item/Tera/G-Max initialization and every preset's intended activation turn.
- Multi-category Tera matching and Mega's jam-sensitive bonus.
- Generic/signature Z bonuses, excitement, base move preservation and one use.
- Ultra Burst's consumed turn and subsequent one-time signature Z appeal.
- Three G-Max appeals, zero jam, shared hearts and no subsequent gimmick.
- Linked NPC roster/selection remaining unchanged.

## Expansion verification (2026-10-22)

- Normal ROM build: `make -j4 --output-sync=target` passed.
- Contest tests: 42/42 passed, including all 208 presets and rejection preserving
  the current roster/points. TIPS tests: 4/4 passed, including text width/height.
- Python tests: 98/98 passed. Japanese text and `git diff --check` passed.
- Link fingerprint remains compatibility 12 / data layout 6.
- EWRAM remains 227,532 bytes; IWRAM remains 28,592 bytes.
- Headless mGBA completed five appeals at each rank (20 rounds), returning to the
  field. The runs included Shinx/Z, Lucario/Mega, Hatterene/G-Max and Dawn Wings
  Necrozma/Ultra Burst followed by its signature Z appeal. NPC presets are chosen
  through the real selector; a temporary QA helper retries selection until the
  representative guest is present, rather than editing the game's NPC data.
- Checks inspect active-icon sprite IDs, coordinates, palette tags and absence
  during cutaway. Captured normal-view frames confirm the Japanese nicknames,
  participant panels, background and active-state indicators.
- The real `pokeemerald.sav` is unchanged (SHA-256
  `4338f917308426f833cd67fdaa3b0317b7382186e91f3ca25122411ed3ff071a`).

The test runner prints existing `secret_power.c:7: ASSUME failed` discovery
notices, but the selected suites pass. This is representative rendering and
progression coverage, not exhaustive verification of every move/form animation,
every rank/category combination in gameplay, two-console communication, or
competitive difficulty. Long-term balance still needs player feedback.

## Files changed for this expansion

- `src/data/contest_npc_extra.inc`: 112 new identities and strategies.
- `src/data/contest_npc_presets.h`: extra roster integration and legacy nicknames.
- `src/contest.c`: template access, expanded selection, NPC BAN checks and name rendering.
- `include/contest.h`: display-name formatter declaration.
- `include/contest_npc.h`: template accessor declaration.
- `src/contest_util.c`: initialize entrants only after accepted registration.
- `include/contest_util.h`: registration function declaration for testing.
- `src/contest_painting.c`: correct name fonts in painting captions.
- `src/text.c`: AUTO font width calculation reads the following character.
- `src/data/contest_tips.h`: NPC-only BAN rule explanation.
- `test/contest_npc.c`: roster, legality, names, BAN and registration coverage.
- `test/frontier_tips.c`: updated page-count expectation.
- `docs/contest_npc_gimmicks.md`: specification and validation record.

Earlier uncommitted contest audit/icon changes are preserved. Ignored `build/`
QA scripts and screenshots are local verification artifacts, not ROM sources.

## Player names and normal-view placement

Names from the naming screen contain raw Japanese bytes, while NPC names can
carry explicit font controls. Prefixing a raw player name with `/` made the
participant printer choose Latin mode and display symbols instead of kana.
`Contest_CopyDisplayName()` adds rendering-only font controls, preserving explicit
controls and selecting the font for each raw character. This also handles mixed
Latin/kana names and restores automatic selection for text after the name.
Saved, linked and contestant name arrays retain their original size and bytes.

Participant panels, results, winner announcements, reception name buffers and
painting captions use the display formatter. Result-panel trainer names normally
start at X 50; longer names move left according to their measured width, keeping
seven-character Japanese names inside the window. The AUTO branch in `GetStringWidth()` now examines the next character,
matching the renderer instead of treating the AUTO opcode itself as a glyph.

The four party icons are centered at X 22/56/90/124, Y 100. Their 32x32 footprints
remain below the excitement meter (bottom Y 60), above the lower text box and
inside the left stage. The seven-pixel bounce also remains clear of the meter.

Follow-up verification: 44 contest tests, 4 TIPS tests and 98 Python tests pass.
The ROM build, Japanese text check, link fingerprint and whitespace check pass.
Headless mGBA reproduces the old kana-as-symbols rendering, then confirms the fix
with raw katakana, seven-character hiragana and seven-character Latin names.
The maximum-length Japanese-name run completes five appeals and returns to the
field. Separate results-screen captures confirm the final seven-character name
is fully visible. Before/after normal-view captures also show all four icons
below the excitement meter. EWRAM and the actual save file remain unchanged.

Files changed in this follow-up:

- `src/contest.c`, `include/contest.h`: player eligibility, display formatter and icon positions.
- `src/contest_util.c`: result/announcement/script name formatting and long-name placement.
- `src/contest_painting.c`: painting-caption name formatting.
- `src/text.c`: one-character correction to AUTO width measurement.
- `src/data/contest_tips.h`: NPC-only BAN explanation.
- `test/contest_npc.c`: unrestricted player entry and raw/tagged/mixed-name regression tests.
- `include/constants/contest.h`, `src/party_menu.c`: removed the player banned-entry result and label.
- `data/scripts/contest_hall.inc`, `data/maps/LilycoveCity_ContestLobby/scripts.inc`: removed player BAN rejection branches/messages.
- `docs/contest_npc_gimmicks.md`: current specification and verification record.
