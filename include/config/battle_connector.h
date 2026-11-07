#ifndef GUARD_CONFIG_BATTLE_CONNECTOR_H
#define GUARD_CONFIG_BATTLE_CONNECTOR_H

// Battle Connector (docs/shadow_pokemon.md). The tables of who appears are in src/data/battle_connector.h.

// A run: BATTLE_CONNECTOR_BRANCHES three-way branches, each leading to a room. The mid-boss room follows the room of
// branch BATTLE_CONNECTOR_MIDBOSS_AFTER (0: no mid-boss), and the boss room follows the last one.
#define BATTLE_CONNECTOR_BRANCHES      4
#define BATTLE_CONNECTOR_MIDBOSS_AFTER 3

// The party in the facility: up to BATTLE_CONNECTOR_BRING_COUNT Pokemon brought plus protected Shadow Pokemon, up to
// BATTLE_CONNECTOR_PARTY_SIZE in all. A run protects at most BATTLE_CONNECTOR_MAX_CANDIDATES (they are the list the
// player takes one from at the end).
#define BATTLE_CONNECTOR_BRING_COUNT    2
#define BATTLE_CONNECTOR_PARTY_SIZE     6
#define BATTLE_CONNECTOR_MAX_CANDIDATES 6

// The Shadow Pokemon of the rooms and the mid-boss are Battle Frontier pool Pokemon (moves, item, nature, ability,
// EVs of their set) up to this National Dex number (386: generations 1-3). Legendary and Mythical Pokemon are left
// out of the rooms. The base stat total sorts them: below STRONG_BST the ordinary rooms, from RARE_BST the rare ones.
#define BATTLE_CONNECTOR_MAX_DEX_NUM    386
#define BATTLE_CONNECTOR_STRONG_BST     480
#define BATTLE_CONNECTOR_RARE_BST       535

// Panic Rush: it costs BATTLE_CONNECTOR_PANIC_COST BP, the player picks BATTLE_CONNECTOR_PANIC_PICKS Pokemon for double
// battles against Pokemon of the Battle Frontier pool (no Legendary or Mythical) and earns BATTLE_CONNECTOR_PANIC_BP_PER_WIN
// BP for each battle won. The foes grow stronger: the strong tier from PANIC_STRONG_AT wins, the rare one from
// PANIC_RARE_AT.
#define BATTLE_CONNECTOR_PANIC_COST       5
#define BATTLE_CONNECTOR_PANIC_PICKS      4
#define BATTLE_CONNECTOR_PANIC_BP_PER_WIN 1
#define BATTLE_CONNECTOR_PANIC_STRONG_AT  5
#define BATTLE_CONNECTOR_PANIC_RARE_AT    10

// IVs of 31 every Shadow Pokemon of the facility has at least (at random stats)
#define BATTLE_CONNECTOR_PERFECT_IVS 3

// Opponents' levels: the level of the Pokemon brought plus these, at least BATTLE_CONNECTOR_LEVEL_MIN
#define BATTLE_CONNECTOR_LEVEL_MIN            10
#define BATTLE_CONNECTOR_LEVEL_SHADOW         0
#define BATTLE_CONNECTOR_LEVEL_STRONG_SHADOW  3
#define BATTLE_CONNECTOR_LEVEL_RARE_SHADOW    2
#define BATTLE_CONNECTOR_LEVEL_TRAINER        0
#define BATTLE_CONNECTOR_LEVEL_STRONG_TRAINER 3
#define BATTLE_CONNECTOR_LEVEL_MIDBOSS        4
#define BATTLE_CONNECTOR_LEVEL_BOSS           5

#endif // GUARD_CONFIG_BATTLE_CONNECTOR_H
