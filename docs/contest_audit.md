# Contest Rules Audit

> Historical notes imported from `frontier-hack` at `1311d01582`.
> Test counts and runtime observations below belong to that branch, not this port.
> See [the Japanese integration record](contest_frontier_integration.md) for this port's scope and verification.

## Fixed Issues

- Registration called `InitPlayerContestGimmickStatus` before appeal resources
  existed. Registration now copies metadata into `gContestMons` only; the existing
  `InitContestResources` initializes the active state when appeals start.
- Dynamax expired while calculating its third appeal, before its animation and
  later entrants' jam attempts. Each successful Max appeal is now recorded in
  `moveGimmick`; the remaining count and G-Max form expire at round end. Failed
  or skipped appeals do not grant shared hearts or consume a remaining appeal.
- Dynamax only reduced jam by one heart. It now uses the existing immunity path,
  including protection against nervousness, without consuming a separate shield.
- Variable-appeal effects overwrote Mega, Z and Stellar appeal bonuses. Heart
  bonuses are now added after the original effect. Z's base appeal/combo value,
  signature Z bonus, category, one-use limit and excitement calculation remain
  separate from animation move IDs.
- Category-sensitive effects and result messages read the original category after
  Tera changed it. They now use the category recorded for the performed appeal;
  the global move data and combo definitions are unchanged.
- Cutaway preparation initialized charging-move data more than once, skipping
  the charging phase; Z/Max moves inherited extra replays from their base move.
  Preparation now occurs only at animation launch, general gimmick animations
  only clear animation variables, and charging phases apply to unconverted moves.
- Cutaway sprite synchronization overwrote a self-target with the invisible
  opponent. It now preserves the target chosen from the original move.

The existing `showGimmickIntro` field distinguishes initial Dynamax activation
from subsequent Max appeals. No save, contestant or communication structure is
extended. Normal battle code, `DoMoveAnim(animMove)` and effect/animation move-ID
separation are preserved. G-Max icons refresh behind the curtain after expiry.

## Automated Checks

The first five added regression tests failed against the old implementation:
third G-Max animation, immunity, variable appeal bonuses, Tera-sensitive jam and
converted charging moves. They pass after the fixes. Additional tests cover
ordinary charging phases, unsuccessful Dynamax appeals and resource-free entry.

```sh
make -j4 --output-sync=target
make -j4 --output-sync=target check TESTS=Contest
make -j4 --output-sync=target check TESTS="Link audit:"
python3 -m unittest discover -s tools/tests -v
python3 tools/check_jp_text.py
python3 tools/link_compat/check_fingerprint.py
git diff --check
```

Results: 32 contest tests, 14 link-audit tests and 98 Python tests pass, as do the
normal ROM build, Japanese text check and link fingerprint check. The test runner
also emits its pre-existing `secret_power.c:7: ASSUME failed` discovery notices;
the selected test suites complete successfully. EWRAM remains 227,532 bytes and
IWRAM remains 28,592 bytes. Link compatibility remains 12 / data layout 6.

## Gameplay Checks

Headless mGBA uses `loadTemporarySave`; the player's actual save is never written.
The ignored `build/contest_audit_qa.*` helpers drive real five-round appeal scenes
and capture selection, gimmick, move-animation and restored-contest frames.
Animation IDs/phases are inspected in memory rather than inferred from text.

Checks include three consecutive G-Max animations and expiry, ordinary charging
and attacking phases, Mega, signature Z, Ultra Burst followed by Z, Tera,
converted Solar Beam, Surf, Trick Room, self-target moves and Stellar.
All eleven configurations completed five appeal rounds and returned to the
field (55 rounds total). Representative captured frames were visually inspected;
the results screen was also opened and its rankings/banner rendering inspected.
This is not a frame-by-frame visual verification of every animation.

Two-console linked gameplay, every move/form animation, and every contest rank
and category in a full playthrough are not exhaustively verified. Logic tests,
asset/layout tests and representative animation runs do not establish that all
possible contests are bug-free.

## Active Gimmick Indicators

Normal contests now reuse the battle Mega, type-specific Tera and Dynamax
indicators at the left edge of each participant panel. They follow turn-order
changes, hide behind the lower move-selection panel, and disappear when Dynamax
expires. Selection alone does not display an active-state indicator.

The existing `tabIconSpriteId` stores the local OBJ ID. Icons are destroyed before
cutaway and recreated with the normal view afterward. Shared tiles and palettes
are released only after all icons are destroyed. No additional persistent state,
save data, communication layout or normal-battle code is introduced.

Validation after this addition: the ROM build, 37 contest tests, 98 Python tests,
Japanese text check and link fingerprint check pass. Headless mGBA completed five
appeal rounds each for Mega, Fire Tera and G-Max (15 rounds total), checking icon
IDs, coordinates and palette tags, including absence during cutaway and expiry.
Captured normal-view and move-selection frames were visually inspected. The
actual save file's SHA-256 is unchanged; EWRAM remains 227,532 bytes.

The repeated full link-audit suite currently passes 13 of 14 tests. The
`move reply and controller completion arrive together` case fails at
`test/link_battle_transport.c:157` for its four-player parameter when run with
the full suite, but both parameters pass when that test is run alone. This
inconsistent result is not counted as a passed link regression check. It does
not exercise the new normal-contest icon lifecycle; linked gameplay remains
outside this addition's verified scope.
