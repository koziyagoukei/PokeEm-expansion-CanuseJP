#include "global.h"
#include "battle.h"
#include "battle_frontier.h"
#include "battle_tower.h"
#include "event_data.h"
#include "field_screen_effect.h"
#include "overworld.h"
#include "pokemon.h"
#include "random.h"
#include "script.h"
#include "task.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/battle_frontier_mons.h"
#include "constants/battle_tower.h"
#include "constants/map_groups.h"
#include "constants/map_scripts.h"
#include "constants/maps.h"

extern const u8 BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost[];
extern const u8 BattleFrontier_BattlePyramid_EventScript_WarpToLobby[];

static void SetPyramidMap(bool32 summit, u32 levelMode)
{
    u32 group = summit ? MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_TOP) : MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR);
    u32 map = summit ? MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_TOP) : MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(group, map);
    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_PYRAMID);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.lvlMode = levelMode;
    ResetTasks();
}

TEST("Frontier transitions: Pyramid loss, draw and forfeit defer warping until the frame script")
{
    bool32 summit = FALSE;
    u32 outcome = B_OUTCOME_LOST, levelMode = FRONTIER_LVL_50;
    static const u32 outcomes[] = {B_OUTCOME_LOST, B_OUTCOME_DREW, B_OUTCOME_FORFEITED};
    for (u32 m = 0; m < 2; m++)
        for (u32 o = 0; o < ARRAY_COUNT(outcomes); o++)
            for (u32 l = 0; l < FRONTIER_LVL_TENT; l++)
                PARAMETRIZE { summit = m; outcome = outcomes[o]; levelMode = l; }

    SetPyramidMap(summit, levelMode);
    gSaveBlock2Ptr->frontier.challengeStatus = 0;
    gBattleOutcome = outcome;
    RunOnResumeMapScript();

    EXPECT(!FuncIsActiveTask(Task_WarpAndLoadMap));
    EXPECT_EQ(VarGet(VAR_TEMP_CHALLENGE_STATUS), CHALLENGE_STATUS_LOST);
    EXPECT_EQ(MapHeaderCheckScriptTable(MAP_SCRIPT_ON_FRAME_TABLE), BattleFrontier_BattlePyramid_EventScript_WarpToLobbyLost);
}

TEST("Frontier transitions: Pyramid interrupted-save return also defers warping")
{
    bool32 summit = FALSE;
    u32 levelMode = FRONTIER_LVL_50;
    for (u32 m = 0; m < 2; m++)
        for (u32 l = 0; l < FRONTIER_LVL_TENT; l++)
            PARAMETRIZE { summit = m; levelMode = l; }

    SetPyramidMap(summit, levelMode);
    gSaveBlock2Ptr->frontier.challengeStatus = CHALLENGE_STATUS_SAVING;
    RunOnResumeMapScript();

    EXPECT(!FuncIsActiveTask(Task_WarpAndLoadMap));
    EXPECT_EQ(MapHeaderCheckScriptTable(MAP_SCRIPT_ON_FRAME_TABLE), BattleFrontier_BattlePyramid_EventScript_WarpToLobby);
}

TEST("Frontier transitions: Tower NPC partners initialize their own facility tables after boot or a Tent")
{
    bool32 fromTent = FALSE;
    u32 levelMode = FRONTIER_LVL_50;
    for (u32 t = 0; t < 2; t++)
        for (u32 l = 0; l < FRONTIER_LVL_TENT; l++)
            PARAMETRIZE { fromTent = t; levelMode = l; }

    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_TOWER);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_MULTIS);
    gSaveBlock2Ptr->frontier.lvlMode = levelMode;
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_SWAMPERT, 50, 0, OTID_STRUCT_PLAYER_ID, 31);
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][1], SPECIES_SKARMORY, 50, 0, OTID_STRUCT_PLAYER_ID, 31);
    gPartiesCount[B_TRAINER_PLAYER] = 2;
    gFacilityTrainers = fromTent ? gSlateportBattleTentTrainers : NULL;
    gFacilityTrainerMons = fromTent ? gSlateportBattleTentMons : NULL;
    SeedRng(1);
    gSpecialVar_0x8004 = BATTLE_TOWER_FUNC_LOAD_PARTNERS;
    CallBattleTowerFunc();

    EXPECT_EQ(gFacilityTrainers, gBattleFrontierTrainers);
    EXPECT_EQ(gFacilityTrainerMons, gBattleFrontierMons);
    for (u32 i = 0; i < 6; i++)
    {
        u32 trainer = gSaveBlock2Ptr->frontier.trainerIds[i];
        EXPECT_LT(trainer, FRONTIER_TRAINERS_COUNT);
        for (u32 j = 0; j < i; j++)
        {
            u32 previous = gSaveBlock2Ptr->frontier.trainerIds[j];
            EXPECT(trainer != previous);
            EXPECT(gFacilityTrainers[trainer].facilityClass != gFacilityTrainers[previous].facilityClass);
        }
    }
    for (u32 i = 8; i < 20; i++)
    {
        u32 mon = gSaveBlock2Ptr->frontier.trainerIds[i];
        EXPECT_LT(mon, NUM_FRONTIER_MONS);
        enum Species species = gFacilityTrainerMons[mon].species;
        EXPECT(species != SPECIES_SWAMPERT);
        EXPECT(species != SPECIES_SKARMORY);
        for (u32 j = 8; j < i; j++)
            EXPECT(species != gFacilityTrainerMons[gSaveBlock2Ptr->frontier.trainerIds[j]].species);
    }
}
