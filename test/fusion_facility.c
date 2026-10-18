#include "global.h"
#include "event_data.h"
#include "frontier_util.h"
#include "overworld.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/maps.h"

static const u16 sChallengeMaps[] = {
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_ELEVATOR,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_CORRIDOR,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_BATTLE_ROOM,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_PARTNER_ROOM,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_CORRIDOR,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_MULTI_BATTLE_ROOM,
    MAP_BATTLE_FRONTIER_BATTLE_DOME_CORRIDOR,
    MAP_BATTLE_FRONTIER_BATTLE_DOME_PRE_BATTLE_ROOM,
    MAP_BATTLE_FRONTIER_BATTLE_DOME_BATTLE_ROOM,
    MAP_BATTLE_FRONTIER_BATTLE_PALACE_CORRIDOR,
    MAP_BATTLE_FRONTIER_BATTLE_PALACE_BATTLE_ROOM,
    MAP_BATTLE_FRONTIER_BATTLE_ARENA_CORRIDOR,
    MAP_BATTLE_FRONTIER_BATTLE_ARENA_BATTLE_ROOM,
    MAP_BATTLE_FRONTIER_BATTLE_FACTORY_PRE_BATTLE_ROOM,
    MAP_BATTLE_FRONTIER_BATTLE_FACTORY_BATTLE_ROOM,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_CORRIDOR,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_THREE_PATH_ROOM,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_NORMAL,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_WILD_MONS,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_ROOM_FINAL,
    MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR,
    MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_TOP,
    MAP_SLATEPORT_CITY_BATTLE_TENT_CORRIDOR,
    MAP_SLATEPORT_CITY_BATTLE_TENT_BATTLE_ROOM,
    MAP_VERDANTURF_TOWN_BATTLE_TENT_CORRIDOR,
    MAP_VERDANTURF_TOWN_BATTLE_TENT_BATTLE_ROOM,
    MAP_FALLARBOR_TOWN_BATTLE_TENT_CORRIDOR,
    MAP_FALLARBOR_TOWN_BATTLE_TENT_BATTLE_ROOM,
};

static const u16 sAllowedMaps[] = {
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_DOME_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_PALACE_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_ARENA_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_FACTORY_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY,
    MAP_SLATEPORT_CITY_BATTLE_TENT_LOBBY,
    MAP_VERDANTURF_TOWN_BATTLE_TENT_LOBBY,
    MAP_FALLARBOR_TOWN_BATTLE_TENT_LOBBY,
    MAP_BATTLE_FRONTIER_OUTSIDE_EAST,
    MAP_BATTLE_FRONTIER_OUTSIDE_WEST,
    MAP_LITTLEROOT_TOWN,
};

static void SetMap(u16 map)
{
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(map);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(map);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(map), MAP_NUM(map));
}

TEST("Fusion facility restriction: all Frontier and Tent challenge rooms disallow separation")
{
    u32 index = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sChallengeMaps); i++)
        PARAMETRIZE { index = i; }
    SetMap(sChallengeMaps[index]);
    EXPECT(InBattleFacilityChallenge());
}

TEST("Fusion facility restriction: lobbies and ordinary maps allow separation")
{
    u32 index = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sAllowedMaps); i++)
        PARAMETRIZE { index = i; }
    SetMap(sAllowedMaps[index]);
    EXPECT(!InBattleFacilityChallenge());
}

TEST("Fusion facility restriction: stale saved challenge status does not lock separation outside a challenge")
{
    u32 facility = 0, status = 0;
    for (u32 f = 0; f < NUM_FRONTIER_FACILITIES; f++)
        for (u32 s = 0; s <= CHALLENGE_STATUS_LOST; s++)
            PARAMETRIZE { facility = f; status = s; }
    SetMap(MAP_BATTLE_FRONTIER_OUTSIDE_WEST);
    VarSet(VAR_FRONTIER_FACILITY, facility);
    gSaveBlock2Ptr->frontier.challengeStatus = status;
    EXPECT(!InBattleFacilityChallenge());
}

#if FREE_TRAINER_HILL == FALSE
TEST("Fusion facility restriction: Trainer Hill is restricted only while its challenge is active")
{
    SetMap(MAP_TRAINER_HILL_1F);
    gSaveBlock1Ptr->trainerHill.spokeToOwner = FALSE;
    VarSet(VAR_TRAINER_HILL_IS_ACTIVE, 0);
    EXPECT(!InBattleFacilityChallenge());
    VarSet(VAR_TRAINER_HILL_IS_ACTIVE, 1);
    EXPECT(InBattleFacilityChallenge());
    gSaveBlock1Ptr->trainerHill.spokeToOwner = TRUE;
    EXPECT(!InBattleFacilityChallenge());
}
#endif
