#include "global.h"
#include "event_data.h"
#include "frontier_util.h"
#include "battle.h"
#include "item.h"
#include "item_use.h"
#include "main.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokemon.h"
#include "recorded_battle.h"
#include "task.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/maps.h"
#include "constants/items.h"
#include "constants/party_menu.h"

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
};

static const u16 sAllowedMaps[] = {
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_DOME_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_PALACE_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_ARENA_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_FACTORY_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY,
    MAP_BATTLE_FRONTIER_OUTSIDE_EAST,
    MAP_BATTLE_FRONTIER_OUTSIDE_WEST,
    MAP_BATTLE_FRONTIER_OUTSIDE_SOUTH,
};

static void SetMap(u16 map)
{
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(map);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(map);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(map), MAP_NUM(map));
}

TEST("Fusion facility restriction: all Frontier challenge rooms disallow separation")
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

static void ReturnFromBlockedRecording(void)
{
}

TEST("Facility media restriction: challenge rooms reject playback without changing RNG or party state")
{
    u32 index = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sChallengeMaps); i++)
        PARAMETRIZE { index = i; }
    SetMap(sChallengeMaps[index]);
    EXPECT(!IsRecordedBattlePlaybackAllowed());

    ResetTasks();
    ZeroPlayerPartyMons();
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 50, 1234, OTID_STRUCT_PLAYER_ID, 31);
    gPartiesCount[B_TRAINER_PLAYER] = 1;
    struct Pokemon original = gParties[B_TRAINER_PLAYER][0];
    SeedRng(0x9876);
    gRecordedBattleRngSeed = gRngValue;
    rng_value_t recordingSeed = gRecordedBattleRngSeed;
    SeedRng(0x12345678);
    rng_value_t currentSeed = gRngValue;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_OPEN;
    gSaveBlock2Ptr->frontier.challengeStatus = CHALLENGE_STATUS_PAUSED;
    MainCallback callback = gMain.callback2;
    u8 mainState = gMain.state;

    PlayRecordedBattle(ReturnFromBlockedRecording);
    EXPECT(gMain.callback2 == ReturnFromBlockedRecording);
    EXPECT_EQ(memcmp(&gRngValue, &currentSeed, sizeof(currentSeed)), 0);
    EXPECT_EQ(memcmp(&gRecordedBattleRngSeed, &recordingSeed, sizeof(recordingSeed)), 0);
    EXPECT_EQ(memcmp(&gParties[B_TRAINER_PLAYER][0], &original, sizeof(original)), 0);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 1);
    EXPECT_EQ(gBattleTypeFlags, BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER);
    EXPECT(gSaveBlock2Ptr->frontier.lvlMode == FRONTIER_LVL_OPEN);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.challengeStatus, CHALLENGE_STATUS_PAUSED);
    for (u32 i = 0; i < NUM_TASKS; i++)
        EXPECT(!gTasks[i].isActive);
    gMain.callback2 = callback;
    gMain.state = mainState;
}

TEST("Facility media restriction: lobbies allow playback only if video storage is enabled")
{
    u32 index = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sAllowedMaps); i++)
        PARAMETRIZE { index = i; }
    SetMap(sAllowedMaps[index]);
    EXPECT_EQ(IsRecordedBattlePlaybackAllowed(), !FREE_SPECIAL_SAVE_SECTORS);
}

TEST("Facility media restriction: stale challenge status does not block battle videos in the hub")
{
    u32 facility = 0, status = 0;
    for (u32 f = 0; f < NUM_FRONTIER_FACILITIES; f++)
        for (u32 s = 0; s <= CHALLENGE_STATUS_LOST; s++)
            PARAMETRIZE { facility = f; status = s; }
    SetMap(MAP_ULTRA_FRONTIE_HUB);
    VarSet(VAR_FRONTIER_FACILITY, facility);
    gSaveBlock2Ptr->frontier.challengeStatus = status;
    EXPECT_EQ(IsRecordedBattlePlaybackAllowed(), !FREE_SPECIAL_SAVE_SECTORS);
}

TEST("Facility media restriction: Teachy TV remains unusable in every challenge room")
{
    u32 index = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sChallengeMaps); i++)
        PARAMETRIZE { index = i; }
    SetMap(sChallengeMaps[index]);
    EXPECT(GetItemFieldFunc(ITEM_TEACHY_TV) == ItemUseOutOfBattle_CannotUse);
    EXPECT_EQ(GetItemBattleUsage(ITEM_TEACHY_TV), 0);
}

static void SetFieldPartyMenu(void)
{
    gPartyMenu = (struct PartyMenu) {
        .menuType = PARTY_MENU_TYPE_FIELD,
        .layout = PARTY_LAYOUT_SINGLE,
        .action = PARTY_ACTION_CHOOSE_MON,
    };
}

TEST("Party resort warp: all Frontier challenge rooms block SELECT entry")
{
    u32 index = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sChallengeMaps); i++)
        PARAMETRIZE { index = i; }
    SetMap(sChallengeMaps[index]);
    SetFieldPartyMenu();
    EXPECT(!Test_CanEnterPartyRoamerParkFromPartyMenu());
}

TEST("Party resort warp: lobbies and ordinary maps retain SELECT entry")
{
    u32 index = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sAllowedMaps); i++)
        PARAMETRIZE { index = i; }
    SetMap(sAllowedMaps[index]);
    SetFieldPartyMenu();
    EXPECT(Test_CanEnterPartyRoamerParkFromPartyMenu());
}

TEST("Party resort warp: stale challenge status does not block entry from the hub")
{
    u32 facility = 0, status = 0;
    for (u32 f = 0; f < NUM_FRONTIER_FACILITIES; f++)
        for (u32 s = 0; s <= CHALLENGE_STATUS_LOST; s++)
            PARAMETRIZE { facility = f; status = s; }
    SetMap(MAP_ULTRA_FRONTIE_HUB);
    SetFieldPartyMenu();
    VarSet(VAR_FRONTIER_FACILITY, facility);
    gSaveBlock2Ptr->frontier.challengeStatus = status;
    EXPECT(Test_CanEnterPartyRoamerParkFromPartyMenu());
}

TEST("Party resort warp: resort dining bath villa shop and local room block recursive entry")
{
    u16 map = MAP_PARTY_ROAMER_PARK;
    PARAMETRIZE { map = MAP_PARTY_ROAMER_PARK; }
    PARAMETRIZE { map = MAP_PARTY_EAT_TIME; }
    PARAMETRIZE { map = MAP_PARTYBATHTIME; }
    PARAMETRIZE { map = MAP_MINI_MINI_VILLA; }
    PARAMETRIZE { map = MAP_MINI_MINI_VILLA_SHOP; }
    PARAMETRIZE { map = MAP_MINI_MINI_VILLA_LOCAL_ROOM; }
    SetMap(map);
    SetFieldPartyMenu();
    EXPECT(!Test_CanEnterPartyRoamerParkFromPartyMenu());
}

TEST("Party resort warp: battle and selection party menus remain unavailable")
{
    u32 type = PARTY_MENU_TYPE_IN_BATTLE;
    PARAMETRIZE { type = PARTY_MENU_TYPE_IN_BATTLE; }
    PARAMETRIZE { type = PARTY_MENU_TYPE_CHOOSE_HALF; }
    PARAMETRIZE { type = PARTY_MENU_TYPE_CONTEST; }
    SetMap(MAP_LITTLEROOT_TOWN);
    SetFieldPartyMenu();
    gPartyMenu.menuType = type;
    EXPECT(!Test_CanEnterPartyRoamerParkFromPartyMenu());
}
