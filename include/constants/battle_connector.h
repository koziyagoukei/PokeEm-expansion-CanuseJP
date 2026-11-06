#ifndef GUARD_CONSTANTS_BATTLE_CONNECTOR_H
#define GUARD_CONSTANTS_BATTLE_CONNECTOR_H

// Battle Connector (src/battle_connector.c): the Shadow Pokemon search and rescue facility in Artisan Cave.

// Room types
#define CONNECTOR_ROOM_NONE           0
#define CONNECTOR_ROOM_SHADOW         1  // a Shadow Pokemon
#define CONNECTOR_ROOM_STRONG_SHADOW  2  // a stronger Shadow Pokemon
#define CONNECTOR_ROOM_RARE_SHADOW    3  // a rare Shadow Pokemon
#define CONNECTOR_ROOM_DARK_TRAINER   4  // a person who cut themselves off, and their Shadow Pokemon
#define CONNECTOR_ROOM_STRONG_TRAINER 5  // the same, stronger
#define CONNECTOR_ROOM_HEAL           6
#define CONNECTOR_ROOM_CALM           7  // a scent that opens the hearts of the Shadow Pokemon in the party
#define CONNECTOR_ROOM_ITEM           8
#define CONNECTOR_ROOM_RANDOM         9  // one of the others, decided on entering
#define CONNECTOR_ROOM_MIDBOSS        10 // a strong Shadow Pokemon on the way, without a branch before it
#define CONNECTOR_ROOM_BOSS           11 // the deepest room
#define CONNECTOR_ROOM_COUNT          12

// The three paths of a branch
#define CONNECTOR_PATH_LEFT   0
#define CONNECTOR_PATH_CENTER 1
#define CONNECTOR_PATH_RIGHT  2
#define CONNECTOR_PATH_COUNT  3

// BattleConnector_LeaveRoom: where the player goes from a cleared room
#define CONNECTOR_NEXT_BRANCH  0
#define CONNECTOR_NEXT_ROOM    1 // a room with no branch before it (the mid-boss, the boss)
#define CONNECTOR_NEXT_RESULTS 2

// VAR_RESULT after BattleConnector_StartRoomBattle
#define CONNECTOR_BATTLE_WON  0
#define CONNECTOR_BATTLE_LOST 1

// BattleConnector_ProtectDefeatedMon
#define CONNECTOR_PROTECT_JOINED     0 // protected, and it fights in the party
#define CONNECTOR_PROTECT_PARTY_FULL 1 // protected; the party is full (it can be swapped in)
#define CONNECTOR_PROTECT_LIST_FULL  2 // no room to protect another
#define CONNECTOR_PROTECT_NONE       3

// BattleConnector_GetReturnState: why the player is back at the reception
#define CONNECTOR_RETURN_NONE    0
#define CONNECTOR_RETURN_CLEARED 1
#define CONNECTOR_RETURN_FAILED  2
#define CONNECTOR_RETURN_RETIRED 3

#endif // GUARD_CONSTANTS_BATTLE_CONNECTOR_H
