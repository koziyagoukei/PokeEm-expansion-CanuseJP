#ifndef GUARD_CONFIG_BATTLE_CONNECTOR_H
#define GUARD_CONFIG_BATTLE_CONNECTOR_H

// Battle Connector (docs/shadow_pokemon.md). The tables of who appears are in src/data/battle_connector.h.

// A run: BATTLE_CONNECTOR_BRANCHES three-way branches, each leading to a room. The mid-boss room follows the room of
// branch BATTLE_CONNECTOR_MIDBOSS_AFTER (0: no mid-boss), and the boss room follows the last one.
#define BATTLE_CONNECTOR_BRANCHES      4
#define BATTLE_CONNECTOR_MIDBOSS_AFTER 3

// The party in the facility: the Pokemon brought plus protected Shadow Pokemon. A run protects at most
// BATTLE_CONNECTOR_MAX_CANDIDATES (they are the list the player takes one from at the end).
#define BATTLE_CONNECTOR_PARTY_SIZE     3
#define BATTLE_CONNECTOR_MAX_CANDIDATES 6

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
