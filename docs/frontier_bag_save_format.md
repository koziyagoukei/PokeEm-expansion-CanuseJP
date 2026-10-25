# Frontier bag and save format

This format deliberately does not load the previous project's saves. PC storage
holds 18 boxes of 30 Pokemon. Secret Base receiving, Frontier records, contests,
and resort data remain enabled. Hall of Fame records, e-Reader Trainer Hill data
and Frontier battle videos are no longer saved.

## Flash layout

`FREE_SPECIAL_SAVE_SECTORS` in `include/config/save.h` reassigns the four
sectors outside the save slots (Hall of Fame x2, Trainer Hill, battle video) to
the two save slots. Each slot now has 16 sectors and both slots keep their full
backup:

| Sectors in a slot | Contents |
| --- | --- |
| 0 | SaveBlock2 |
| 1-4 | SaveBlock1 |
| 5-15 | PokemonStorage (18 boxes, names and wallpapers) |

The four fusion partners (Kyurem, Necrozma, Calyrex and so on) moved from
PokemonStorage to the end of SaveBlock3, so the 11 storage sectors only hold
boxes. `FREE_TRAINER_HILL` is also enabled, because the Trainer Hill maps are
unreachable.

Consequences of removing the special sectors:

- The PC menu does not show the Hall of Fame choice, even with
  `FLAG_SYS_GAME_CLEAR` set. Hall of Fame saving only writes the normal slot.
- Frontier facilities and link battles never offer to record a battle video,
  and the Frontier Pass has no video to play back.
- Special sector reads and writes always fail without touching flash.
- Overwriting a different file no longer erases sectors 28-31, which now belong
  to save slot 2.

Setting `FREE_SPECIAL_SAVE_SECTORS` to `FALSE` restores the 14-box layout and
needs a new save signature.

## Box sorting

The box title menu has a Sort option (`ならびかえ`). It does not change the save
layout. The player picks the range (this box, or all boxes packed from box 1)
and a key:

- National Dex: every Pokemon in National Dex order.
- Type, Move, Ability: a list of the types, moves or abilities found in the
  range. Pokemon with the chosen one come first, then the rest, both in National
  Dex order. Move and ability names are listed in gojuon order.

Eggs always go after the other Pokemon, and empty slots go last. Pokemon with the
same key keep their previous order. The option is hidden while a Pokemon is held
or several are being moved. The logic is in `src/pokemon_storage_sort.c` and
tested in `test/pokemon_storage_sort.c`.

## Pockets

| Pocket | Distinct stacks | Quantity per stack |
| --- | ---: | ---: |
| Items | 206 | 999 |
| Poke Balls | 5 | 999 |
| Stones | 30 | 999 |
| Berries | 67 | 999 |
| Key Items | 12 | 1 normally distributed |

The Stones pocket replaces the visible TM/HM pocket. Every item with the
`HOLD_EFFECT_MEGA_STONE` or `HOLD_EFFECT_Z_CRYSTAL` effect belongs there,
including signature crystals and Ultranecrozium Z. It is a shared 30-type
capacity, not capacity for every stone in the game. Giving, taking, sorting,
quantity encryption, and temporary bag backup use the normal bag APIs.

TM/HM item IDs and move-teaching functions are retained. If a machine is obtained,
it is an ordinary item; it does not create an extra visible pocket. Their icon
and teaching checks identify the item itself rather than the pocket.

The ordinary capacity comes from the current Frontier/resort/contest acquisition
catalog, not the entire item enum. It includes hub distribution, the two item
selection menus, the Mart, BP exchanges, Pickup, Honey Gather, the resort's daily
gift, contest scarves, and starter PC items. Pyramid-only supplies use its own
bag. Debug distributions, external trade imports, disabled gifts, and unreachable
Hoenn mainland content are not extra ordinary-capacity requirements. The ferry
currently does not take the player to the mainland.

`tools/data/frontier_bag_catalog.json` records each included item and its source.
Regenerate after changing an acquisition source:

```sh
python3 tools/generate_bag_catalog.py
python3 tools/generate_bag_catalog.py --check
```

The build checks the generated catalog. If changing its count changes the bag
layout, review the save signature and link data-layout version before release.

## Initial distribution

New games receive these eight utility Key Items, leaving four slots for later
use: Mega Ring, Z-Power Ring, Dynamax Band, Tera Orb, Mach Bike, Pokeblock Case,
Wailmer Pail and Powder Jar. The watering can preserves access past Sudowoodo;
the other utility items preserve contests and Berry Crush. Rods are not included
because the currently reachable maps have no fishing encounter tables.
Story progression keys and tickets are not automatically distributed.

All 67 ordinary berry types start at 999. The obsolete e-Reader Enigma Berry is
excluded; the ordinary Enigma Berry remains available. Hub setup can restore
missing starter items without duplicating existing stacks, matching its existing
auto-item behavior. The Pyramid bag is unchanged.

## Intentional format break

`SAVE_FORMAT_SIGNATURE` in `include/config/save.h` is `0x46520002`. The previous
`0x08012025` signature, the 14-box `0x46520001` development format, and other
versions in the same format family are reported as incompatible. The title menu explains this in Japanese and offers
New Game and Options. Loading such a save does not copy its payload, erase it,
or automatically save over it. Starting a new game and explicitly saving can
replace it, as with normal new-game behavior.

Current-format backups retain the two alternating save slots and sector
rotation. Sector IDs are checked before table access and checksums. A valid
current backup can be loaded even when the other slot is old or damaged.

e-Reader entrypoints and its dedicated save data are disabled. Mystery Gift's
source remains in place, but its menus and reception are disabled through the
existing `FREE_MYSTERY_GIFT` and `FREE_MYSTERY_EVENT_BUFFERS` settings. The
instructions in `include/config/save.h` explain how to re-enable them. Any such
layout change requires a new save signature, not silent reuse of this format.

`LINK_DATA_LAYOUT_VERSION` is 7. Linked peers must use matching project,
compatibility, and data-layout versions. `LINK_COMPAT_VERSION` remains 13.

## Validation

```sh
make check -j4 --output-sync=target TESTS=test/frontier_inventory.c
make check -j4 --output-sync=target TESTS=test/save_format.c
make check -j4 --output-sync=target TESTS=test/save.c
make check -j4 --output-sync=target TESTS=test/link_compat.c
make -j4 --output-sync=target
```

Manual validation should cover all pocket tabs, held-item give/take, full-pocket
failure, new-game startup, save/reload, and the old-save warning. For storage,
also check boxes 15-18 (names, wallpapers, deposit and jump), a fusion and
unfusion followed by save/reload, the PC menu after game clear, and that Frontier
facilities skip the record prompt. Use copied saves and an ordinary boot, not a
save state made with an older ROM.
