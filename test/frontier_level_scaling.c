#include "global.h"
#include "battle.h"
#include "battle_pyramid.h"
#include "event_data.h"
#include "frontier_util.h"
#include "load_save.h"
#include "party_menu.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/battle_dome.h"
#include "constants/battle_frontier.h"
#include "constants/battle_pyramid.h"
#include "constants/frontier_util.h"

void CallBattleDomeFunction(void);

static const enum Species sSpecies[PARTY_SIZE] = {
    SPECIES_PIKACHU, SPECIES_BULBASAUR, SPECIES_CHARMANDER,
    SPECIES_SQUIRTLE, SPECIES_EEVEE, SPECIES_SNORLAX,
};
static const u8 sLevels[PARTY_SIZE] = {100, 51, 73, 49, 50, 25};

static void CallFrontierFunction(u32 function)
{
    gSpecialVar_0x8004 = function;
    CallFrontierUtilFunc();
}

static void SelectChallengeParty(u32 count)
{
    gSpecialVar_0x8005 = count;
    CallFrontierFunction(FRONTIER_UTIL_FUNC_SET_PARTY_ORDER);
}

static void SetUpChallenge(u32 facility, u32 levelMode)
{
    VarSet(VAR_FRONTIER_FACILITY, facility);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.lvlMode = levelMode;
    memset(gSaveBlock2Ptr->frontier.selectedPartyMons, 0, sizeof(gSaveBlock2Ptr->frontier.selectedPartyMons));
    memset(gSelectedOrderFromParty, 0, sizeof(gSelectedOrderFromParty));
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        CreateMonWithIVs(mon, sSpecies[i], sLevels[i], 0x12345678 + i, OTID_STRUCT_PRESET(0x12345678), 31);
        // Preserve fractional progress within each original level, not just the level number.
        if (sLevels[i] != MAX_LEVEL)
        {
            u32 exp = GetMonData(mon, MON_DATA_EXP) + 17;
            SetMonData(mon, MON_DATA_EXP, &exp);
            CalculateMonStats(mon);
        }
        for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
            SetMonMoveSlot(mon, MOVE_TACKLE, slot);
    }
    gPartiesCount[B_TRAINER_PLAYER] = PARTY_SIZE;
    SavePlayerParty();
    gSaveBlock2Ptr->frontier.selectedPartyMons[0] = 3;
    gSaveBlock2Ptr->frontier.selectedPartyMons[1] = 1;
    gSaveBlock2Ptr->frontier.selectedPartyMons[2] = 2;
}

TEST("Frontier level scaling: all owned-party facilities scale selected level 51 to 100 mons on entry")
{
    u32 facility = FRONTIER_FACILITY_TOWER;
    for (u32 i = 0; i < NUM_FRONTIER_FACILITIES; i++)
        if (i != FRONTIER_FACILITY_FACTORY)
            PARAMETRIZE { facility = i; }
    SetUpChallenge(facility, FRONTIER_LVL_50);
    SelectChallengeParty(FRONTIER_PARTY_SIZE);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], FRONTIER_PARTY_SIZE);
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
    {
        u32 originalSlot = gSaveBlock2Ptr->frontier.selectedPartyMons[i] - 1;
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL), 50);
        EXPECT_EQ(GetLevelFromMonExp(&gParties[B_TRAINER_PLAYER][i]), 50);
        EXPECT_EQ(GetMonData(GetSavedPlayerPartyMon(originalSlot), MON_DATA_LEVEL), sLevels[originalSlot]);
    }
}

TEST("Frontier level scaling: Sketch cleanup and room transitions never unscale the live party")
{
    u32 facility = FRONTIER_FACILITY_TOWER;
    for (u32 i = 0; i < NUM_FRONTIER_FACILITIES; i++)
        if (i != FRONTIER_FACILITY_FACTORY)
            PARAMETRIZE { facility = i; }
    struct Pokemon before[FRONTIER_PARTY_SIZE];
    u32 originalExp[PARTY_SIZE];
    SetUpChallenge(facility, FRONTIER_LVL_50);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        originalExp[i] = GetMonData(GetSavedPlayerPartyMon(i), MON_DATA_EXP);
    SelectChallengeParty(FRONTIER_PARTY_SIZE);
    memcpy(before, gParties[B_TRAINER_PLAYER], sizeof(before));
    for (u32 battle = 0; battle < FRONTIER_STAGES_PER_CHALLENGE; battle++)
    {
        CallFrontierFunction(FRONTIER_UTIL_FUNC_RESET_SKETCH_MOVES);
        EXPECT_EQ(memcmp(before, gParties[B_TRAINER_PLAYER], sizeof(before)), 0);
        for (u32 i = 0; i < PARTY_SIZE; i++)
            EXPECT_EQ(GetMonData(GetSavedPlayerPartyMon(i), MON_DATA_EXP), originalExp[i]);
    }
    LoadPlayerParty();
    for (u32 i = 0; i < PARTY_SIZE; i++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL), sLevels[i]);
}

TEST("Frontier level scaling: Tower NPC multi partner room keeps both player mons scaled")
{
    struct Pokemon before[FRONTIER_MULTI_PARTY_SIZE], partner;
    SetUpChallenge(FRONTIER_FACILITY_TOWER, FRONTIER_LVL_50);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_MULTIS);
    gSaveBlock2Ptr->frontier.selectedPartyMons[2] = 0;
    CreateMonWithIVs(&gParties[B_TRAINER_PARTNER][0], SPECIES_EEVEE, 50,
        0x87654321, OTID_STRUCT_PRESET(0x87654321), 31);
    partner = gParties[B_TRAINER_PARTNER][0];
    SelectChallengeParty(FRONTIER_MULTI_PARTY_SIZE);
    memcpy(before, gParties[B_TRAINER_PLAYER], sizeof(before));
    // The partner room reuses pyramid_resetparty even before the first battle.
    gSpecialVar_0x8004 = BATTLE_PYRAMID_FUNC_RESTORE_PARTY;
    CallBattlePyramidFunction();
    EXPECT_EQ(memcmp(before, gParties[B_TRAINER_PLAYER], sizeof(before)), 0);
    EXPECT_EQ(memcmp(&partner, &gParties[B_TRAINER_PARTNER][0], sizeof(partner)), 0);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.selectedPartyMons[2], 0);
    LoadPlayerParty();
    SelectChallengeParty(FRONTIER_MULTI_PARTY_SIZE);
    for (u32 i = 0; i < FRONTIER_MULTI_PARTY_SIZE; i++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL), 50);
}

TEST("Frontier level scaling: Dome selection and post-battle saving keep level 50 for every round")
{
    SetUpChallenge(FRONTIER_FACILITY_DOME, FRONTIER_LVL_50);
    for (u32 round = 0; round < DOME_ROUNDS_COUNT; round++)
    {
        LoadPlayerParty();
        SelectChallengeParty(FRONTIER_PARTY_SIZE);
        CallFrontierFunction(FRONTIER_UTIL_FUNC_RESET_SKETCH_MOVES);
        gSelectedOrderFromParty[0] = 3;
        gSelectedOrderFromParty[1] = 1;
        gSelectedOrderFromParty[2] = 0;
        gSpecialVar_0x8004 = BATTLE_DOME_FUNC_REDUCE_PARTY;
        CallBattleDomeFunction();
        EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], DOME_BATTLE_PARTY_SIZE);
        for (u32 i = 0; i < DOME_BATTLE_PARTY_SIZE; i++)
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL), 50);
        gSpecialVar_0x8004 = BATTLE_DOME_FUNC_RESET_SKETCH;
        CallBattleDomeFunction();
        for (u32 i = 0; i < DOME_BATTLE_PARTY_SIZE; i++)
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL), 50);
    }
    LoadPlayerParty();
    for (u32 i = 0; i < PARTY_SIZE; i++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL), sLevels[i]);
}

TEST("Frontier level scaling: Pyramid saves reordered damaged and fainted mons without unscaling them")
{
    struct Pokemon temp, before[FRONTIER_PARTY_SIZE];
    u32 originalExp[PARTY_SIZE];
    SetUpChallenge(FRONTIER_FACILITY_PYRAMID, FRONTIER_LVL_50);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        originalExp[i] = GetMonData(GetSavedPlayerPartyMon(i), MON_DATA_EXP);
    SelectChallengeParty(FRONTIER_PARTY_SIZE);
    temp = gParties[B_TRAINER_PLAYER][0];
    gParties[B_TRAINER_PLAYER][0] = gParties[B_TRAINER_PLAYER][2];
    gParties[B_TRAINER_PLAYER][2] = temp;
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
    {
        u32 hp = i == 1 ? 0 : GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_MAX_HP) - 12;
        u32 status = STATUS1_POISON, pp = 3;
        SetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_HP, &hp);
        SetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_STATUS, &status);
        SetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_PP1, &pp);
    }
    memcpy(before, gParties[B_TRAINER_PLAYER], sizeof(before));
    gSpecialVar_0x8004 = BATTLE_PYRAMID_FUNC_RESTORE_PARTY;
    CallBattlePyramidFunction();
    EXPECT_EQ(memcmp(before, gParties[B_TRAINER_PLAYER], sizeof(before)), 0);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.selectedPartyMons[0], 2);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.selectedPartyMons[1], 1);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.selectedPartyMons[2], 3);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        EXPECT_EQ(GetMonData(GetSavedPlayerPartyMon(i), MON_DATA_EXP), originalExp[i]);
    LoadPlayerParty();
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 0);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_MAX_HP) - GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HP), 12);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_STATUS), STATUS1_POISON);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_PP1), 3);
}

TEST("Frontier level scaling: Pike pause resume and retirement preserve exact original EXP")
{
    u32 originalExp[PARTY_SIZE];
    SetUpChallenge(FRONTIER_FACILITY_PIKE, FRONTIER_LVL_50);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        originalExp[i] = GetMonData(GetSavedPlayerPartyMon(i), MON_DATA_EXP);
    SelectChallengeParty(FRONTIER_PARTY_SIZE);
    for (u32 pause = 0; pause < 3; pause++)
    {
        CallFrontierFunction(FRONTIER_UTIL_FUNC_SAVE_PARTY);
        for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL), 50);
        LoadPlayerParty();
        for (u32 i = 0; i < PARTY_SIZE; i++)
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_EXP), originalExp[i]);
        SavePlayerParty();
        SelectChallengeParty(FRONTIER_PARTY_SIZE);
    }
    LoadPlayerParty();
    for (u32 i = 0; i < PARTY_SIZE; i++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_EXP), originalExp[i]);
}

TEST("Frontier level scaling: level 50 and lower mons keep their original EXP and stats")
{
    struct Pokemon before[FRONTIER_PARTY_SIZE];
    SetUpChallenge(FRONTIER_FACILITY_TOWER, FRONTIER_LVL_50);
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
    {
        gSaveBlock2Ptr->frontier.selectedPartyMons[i] = i + 4;
        before[i] = gParties[B_TRAINER_PLAYER][i + 3];
    }
    SelectChallengeParty(FRONTIER_PARTY_SIZE);
    CallFrontierFunction(FRONTIER_UTIL_FUNC_RESET_SKETCH_MOVES);
    CallFrontierFunction(FRONTIER_UTIL_FUNC_SAVE_PARTY);
    EXPECT_EQ(memcmp(before, gParties[B_TRAINER_PLAYER], sizeof(before)), 0);
}

TEST("Frontier level scaling: Open Level and Battle Tents do not scale live or saved mons")
{
    u32 levelMode = FRONTIER_LVL_OPEN;
    PARAMETRIZE { levelMode = FRONTIER_LVL_OPEN; }
    PARAMETRIZE { levelMode = FRONTIER_LVL_TENT; }
    struct Pokemon before[FRONTIER_PARTY_SIZE];
    SetUpChallenge(FRONTIER_FACILITY_TOWER, levelMode);
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
        before[i] = *GetSavedPlayerPartyMon(gSaveBlock2Ptr->frontier.selectedPartyMons[i] - 1);
    SelectChallengeParty(FRONTIER_PARTY_SIZE);
    CallFrontierFunction(FRONTIER_UTIL_FUNC_RESET_SKETCH_MOVES);
    CallFrontierFunction(FRONTIER_UTIL_FUNC_SAVE_PARTY);
    EXPECT_EQ(memcmp(before, gParties[B_TRAINER_PLAYER], sizeof(before)), 0);
}

TEST("Frontier level scaling: Sketch cleanup still restores copied moves without changing level")
{
    SetUpChallenge(FRONTIER_FACILITY_TOWER, FRONTIER_LVL_50);
    SelectChallengeParty(FRONTIER_PARTY_SIZE);
    SetMonMoveSlot(&gParties[B_TRAINER_PLAYER][0], MOVE_SURF, 0);
    CallFrontierFunction(FRONTIER_UTIL_FUNC_RESET_SKETCH_MOVES);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MOVE1), MOVE_SKETCH);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), 50);
    EXPECT_EQ(GetMonData(GetSavedPlayerPartyMon(2), MON_DATA_MOVE1), MOVE_SKETCH);
    EXPECT_EQ(GetMonData(GetSavedPlayerPartyMon(2), MON_DATA_LEVEL), 73);
}

TEST("Frontier level scaling: invalid save slots do not touch the party or its backup")
{
    struct Pokemon before, saved;
    SetUpChallenge(FRONTIER_FACILITY_TOWER, FRONTIER_LVL_50);
    SelectChallengeParty(FRONTIER_PARTY_SIZE);
    before = gParties[B_TRAINER_PLAYER][0];
    saved = *GetSavedPlayerPartyMon(0);
    SaveFrontierPlayerPartyMon(PARTY_SIZE, &gParties[B_TRAINER_PLAYER][0]);
    SaveFrontierPlayerPartyMon((u16)-1, &gParties[B_TRAINER_PLAYER][0]);
    EXPECT_EQ(memcmp(&before, &gParties[B_TRAINER_PLAYER][0], sizeof(before)), 0);
    EXPECT_EQ(memcmp(&saved, GetSavedPlayerPartyMon(0), sizeof(saved)), 0);
}
