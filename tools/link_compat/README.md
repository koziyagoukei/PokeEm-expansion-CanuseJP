# Link compatibility fingerprint

`check_fingerprint.py` is a build-time guard for communication-sensitive
configuration. It is supplemental: runtime compatibility is decided only by
`LINK_PROJECT_ID`, `LINK_COMPAT_VERSION`, and `LINK_DATA_LAYOUT_VERSION`.

The data-layout fingerprint covers species, move, ability, and item ID tables;
the transmitted Pokemon, contest, trainer-card, battle-header, and LinkPlayer
structures; expected structure sizes; and major link buffer sizes. The protocol
fingerprint covers link commands and types, Union Room activities, battle
controller commands, battle type flags, contest and battle gimmick enums, and
synchronization-relevant battle configuration.

Comments and whitespace are ignored. Text, graphics, audio, and map sources are
not inputs, so those changes do not require a compatibility version bump.

When a guarded input changes:

- Increase `LINK_COMPAT_VERSION` when synchronized battle results, controller
  commands, gimmick representation, or the link protocol changes.
- Increase `LINK_DATA_LAYOUT_VERSION` when species, move, ability, or item IDs
  change; a transmitted structure or packet changes; or saved gimmick bits such
  as Tera Type, Dynamax, Gigantamax, or Shadow state move.
- Run `python3 tools/link_compat/check_fingerprint.py --update` after reviewing
  the generated component changes.

Text translation, graphics, audio, maps, and fixes that cannot affect linked
results do not require a bump and are intentionally outside the inputs.

Both version numbers currently occupy four bits of the legacy `LinkPlayer`
byte, so valid values are 0 through 15. Reaching 15 requires a deliberate
protocol revision rather than silently wrapping. `LINK_BUILD_VARIANT` is shown
for diagnostics but is not compared. TEST and DEBUG builds therefore remain
compatible when their transmitted layouts and synchronized behavior are the
same. The RFU serial is not changed by this implementation.

The build fails if a fingerprint changes without the corresponding version
increase. After increasing the version, the build also remains blocked until
the reviewed state is deliberately recorded with `--update`. The update command
refuses to overwrite an existing changed baseline unless the required version
has increased.

Run the checker and its internal regression test directly with:

```sh
python3 tools/link_compat/check_fingerprint.py
python3 tools/link_compat/check_fingerprint.py --self-test
```
