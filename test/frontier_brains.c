#include "global.h"
#include "battle.h"
#include "battle_dome.h"
#include "battle_frontier.h"
#include "battle_setup.h"
#include "event_data.h"
#include "frontier_util.h"
#include "pokemon.h"
#include "random.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/battle_frontier_mons.h"
#include "constants/frontier_util.h"
#include "constants/trainers.h"

static const u16 sOriginalBrainStreaks[NUM_FRONTIER_FACILITIES][2] =
{
    [FRONTIER_FACILITY_TOWER] = {34, 69},
    [FRONTIER_FACILITY_DOME] = {4, 9},
    [FRONTIER_FACILITY_PALACE] = {20, 41},
    [FRONTIER_FACILITY_ARENA] = {27, 55},
    [FRONTIER_FACILITY_FACTORY] = {20, 41},
    [FRONTIER_FACILITY_PIKE] = {27, 139},
    [FRONTIER_FACILITY_PYRAMID] = {21, 70},
};

static void SetUpBrainChallenge(u32 facility, u32 symbols, u32 levelMode)
{
    u32 i;

    VarSet(VAR_FRONTIER_FACILITY, facility);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.lvlMode = levelMode;
    for (i = 0; i < NUM_FRONTIER_FACILITIES; i++)
    {
        FlagClear(gFrontierBrainInfo[i].silverSymbolFlag);
        FlagClear(gFrontierBrainInfo[i].goldSymbolFlag);
    }
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    if (symbols >= 1)
        FlagSet(gFrontierBrainInfo[facility].silverSymbolFlag);
    if (symbols >= 2)
        FlagSet(gFrontierBrainInfo[facility].goldSymbolFlag);
}

static void SetChallengeStreak(u32 facility, u16 streak)
{
    u32 mode = VarGet(VAR_FRONTIER_BATTLE_MODE);
    u32 level = gSaveBlock2Ptr->frontier.lvlMode;

    switch (facility)
    {
    case FRONTIER_FACILITY_TOWER:
        gSaveBlock2Ptr->frontier.towerWinStreaks[mode][level] = streak;
        break;
    case FRONTIER_FACILITY_DOME:
        gSaveBlock2Ptr->frontier.domeWinStreaks[mode][level] = streak;
        break;
    case FRONTIER_FACILITY_PALACE:
        gSaveBlock2Ptr->frontier.palaceWinStreaks[mode][level] = streak;
        break;
    case FRONTIER_FACILITY_ARENA:
        gSaveBlock2Ptr->frontier.arenaWinStreaks[level] = streak;
        break;
    case FRONTIER_FACILITY_FACTORY:
        gSaveBlock2Ptr->frontier.factoryWinStreaks[mode][level] = streak;
        break;
    case FRONTIER_FACILITY_PIKE:
        gSaveBlock2Ptr->frontier.pikeWinStreaks[level] = streak;
        break;
    case FRONTIER_FACILITY_PYRAMID:
        gSaveBlock2Ptr->frontier.pyramidWinStreaks[level] = streak;
        break;
    }
}

TEST("Frontier Brain appears at the original silver and gold milestones in both level modes")
{
    u32 facility = 0, symbols = 0, level = 0;

    for (u32 f = 0; f < NUM_FRONTIER_FACILITIES; f++)
        for (u32 s = 0; s < 2; s++)
            for (u32 l = 0; l < FRONTIER_LVL_TENT; l++)
                PARAMETRIZE { facility = f; symbols = s; level = l; }

    SetUpBrainChallenge(facility, symbols, level);
    SetChallengeStreak(facility, sOriginalBrainStreaks[facility][symbols] - 1);
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_NOT_READY);
    SetChallengeStreak(facility, sOriginalBrainStreaks[facility][symbols]);
    EXPECT_EQ(GetFrontierBrainStatus(), symbols + 1);
    EXPECT_EQ(GetPlayerSymbolCountForFacility(facility), symbols);
}

TEST("Frontier Brain retries missed symbols only at a later challenge's final stage")
{
    u32 facility = 0, symbols = 0;
    u32 stages;

    for (u32 f = 0; f < NUM_FRONTIER_FACILITIES; f++)
        for (u32 s = 0; s < 2; s++)
            PARAMETRIZE { facility = f; symbols = s; }

    SetUpBrainChallenge(facility, symbols, FRONTIER_LVL_50);
    stages = facility == FRONTIER_FACILITY_DOME ? 1 : facility == FRONTIER_FACILITY_PIKE ? 14 : 7;
    if (stages != 1)
    {
        SetChallengeStreak(facility, sOriginalBrainStreaks[facility][symbols] + 1);
        EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_NOT_READY);
    }
    SetChallengeStreak(facility, sOriginalBrainStreaks[facility][symbols] + stages);
    EXPECT_EQ(GetFrontierBrainStatus(), symbols + 1);
}

TEST("Frontier Brain rematches retain the original schedule after both symbols")
{
    u32 facility = 0;

    for (u32 f = 0; f < NUM_FRONTIER_FACILITIES; f++)
        PARAMETRIZE { facility = f; }

    SetUpBrainChallenge(facility, 2, FRONTIER_LVL_50);
    SetChallengeStreak(facility, sOriginalBrainStreaks[facility][0]);
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_STREAK);
    EXPECT_EQ(GetFronterBrainSymbol(), 0);
    SetChallengeStreak(facility, sOriginalBrainStreaks[facility][1]);
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_STREAK_LONG);
    EXPECT_EQ(GetFronterBrainSymbol(), 1);
    SetChallengeStreak(facility, sOriginalBrainStreaks[facility][1] + gFrontierBrainInfo[facility].streakAppearances[2]);
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_STREAK_LONG);
    SetChallengeStreak(facility, sOriginalBrainStreaks[facility][1] + 1);
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_NOT_READY);
    EXPECT_EQ(GetFronterBrainSymbol(), 1);
}

TEST("Frontier Brain does not appear in doubles, multi battles, or Battle Tents")
{
    u32 mode = FRONTIER_MODE_DOUBLES;

    for (u32 m = FRONTIER_MODE_DOUBLES; m < FRONTIER_MODE_COUNT; m++)
        PARAMETRIZE { mode = m; }

    SetUpBrainChallenge(FRONTIER_FACILITY_TOWER, 0, FRONTIER_LVL_50);
    VarSet(VAR_FRONTIER_BATTLE_MODE, mode);
    SetChallengeStreak(FRONTIER_FACILITY_TOWER, 34);
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_NOT_READY);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_TENT;
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_NOT_READY);
    VarSet(VAR_FRONTIER_FACILITY, NUM_FRONTIER_FACILITIES);
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_NOT_READY);
}

TEST("Frontier Brain symbol awards set only the current facility's silver then gold flags")
{
    u32 facility = 0;

    for (u32 f = 0; f < NUM_FRONTIER_FACILITIES; f++)
        PARAMETRIZE { facility = f; }

    SetUpBrainChallenge(facility, 0, FRONTIER_LVL_50);
    gSpecialVar_0x8004 = FRONTIER_UTIL_FUNC_GIVE_FACILITY_SYMBOL;
    CallFrontierUtilFunc();
    EXPECT_EQ(FlagGet(gFrontierBrainInfo[facility].silverSymbolFlag), TRUE);
    EXPECT_EQ(FlagGet(gFrontierBrainInfo[facility].goldSymbolFlag), FALSE);
    CallFrontierUtilFunc();
    EXPECT_EQ(GetPlayerSymbolCountForFacility(facility), 2);
    for (u32 f = 0; f < NUM_FRONTIER_FACILITIES; f++)
        if (f != facility)
            EXPECT_EQ(GetPlayerSymbolCountForFacility(f), 0);
}

TEST("Frontier Brain Tower status changes before battle 35 and keeps Anabel's trainer identity with a random party")
{
    u16 expectedIds[FRONTIER_PARTY_SIZE];
    u32 seed = 1;

    PARAMETRIZE { seed = 1; }
    PARAMETRIZE { seed = 1234; }
    PARAMETRIZE { seed = 9876; }

    SetUpBrainChallenge(FRONTIER_FACILITY_TOWER, 0, FRONTIER_LVL_50);
    SetChallengeStreak(FRONTIER_FACILITY_TOWER, 33);
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_NOT_READY);
    gSpecialVar_0x8004 = FRONTIER_UTIL_FUNC_INCREMENT_STREAK;
    CallFrontierUtilFunc();
    EXPECT_EQ(GetCurrentFacilityWinStreak(), 34);
    EXPECT_EQ(GetFrontierBrainStatus(), FRONTIER_BRAIN_SILVER);
    gSpecialVar_0x8004 = FRONTIER_UTIL_FUNC_SET_BRAIN_OBJECT;
    CallFrontierUtilFunc();
    EXPECT_EQ(TRAINER_BATTLE_PARAM.opponentA, TRAINER_FRONTIER_BRAIN);

    SetFacilityPtrsGetLevel();
    SeedRng(seed);
    Random32(); // Party OT ID is generated before pool selection.
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
        expectedIds[i] = GetRandomFrontierMonFromFullPool(expectedIds, i, NULL, 0, NULL, 0);
    SeedRng(seed);
    FillFrontierTrainerParty(FRONTIER_PARTY_SIZE);
    EXPECT_EQ(TRAINER_BATTLE_PARAM.opponentA, TRAINER_FRONTIER_BRAIN);
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
    {
        const struct TrainerMon *set = &gBattleFrontierMons[expectedIds[i]];
        struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][i];

        EXPECT_EQ(GetMonData(mon, MON_DATA_SPECIES), set->species);
        EXPECT_EQ(GetMonData(mon, MON_DATA_HELD_ITEM), set->heldItem);
        for (u32 m = 0; m < MAX_MON_MOVES; m++)
            EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE1 + m), set->moves[m]);
        for (u32 j = 0; j < i; j++)
            EXPECT_NE(GetMonData(mon, MON_DATA_SPECIES), GetMonData(&gParties[B_TRAINER_OPPONENT_A][j], MON_DATA_SPECIES));
    }
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][3], MON_DATA_SPECIES), SPECIES_NONE);
}

TEST("Frontier Brain Dome preview and battle use the same saved random sets")
{
    static const u16 monIds[FRONTIER_PARTY_SIZE] =
    {
        FRONTIER_MON_VENUSAUR_1, FRONTIER_MON_CHARIZARD_1, FRONTIER_MON_BLASTOISE_1,
    };

    SetUpBrainChallenge(FRONTIER_FACILITY_DOME, 0, FRONTIER_LVL_50);
    SetChallengeStreak(FRONTIER_FACILITY_DOME, 4);
    memset(gSaveBlock2Ptr->frontier.domeTrainers, 0, sizeof(gSaveBlock2Ptr->frontier.domeTrainers));
    gSaveBlock2Ptr->frontier.domeTrainers[2].trainerId = TRAINER_FRONTIER_BRAIN;
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
    {
        const struct TrainerMon *set = &gBattleFrontierMons[monIds[i]];

        gSaveBlock2Ptr->frontier.domeMonIds[2][i] = monIds[i];
        CreateMon(&gParties[B_TRAINER_PLAYER][i], SPECIES_PIKACHU, 50, 100 + i, OTID_STRUCT_PRESET(1234));
        EXPECT_EQ(GetFrontierBrainMonSpecies(i), set->species);
        EXPECT_EQ(GetFrontierBrainMonNature(i), set->nature);
        for (u32 m = 0; m < MAX_MON_MOVES; m++)
            EXPECT_EQ(GetFrontierBrainMonMove(i, m), set->moves[m]);
        for (u32 stat = 0; stat < NUM_STATS; stat++)
            EXPECT_EQ(GetFrontierBrainMonEvs(i, stat), set->ev != NULL ? set->ev[stat] : 0);
    }

    SetFrontierBrainObjEventGfx(FRONTIER_FACILITY_DOME);
    FillFrontierTrainerParty(2);
    for (u32 i = 0; i < 2; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][i];
        u32 match = FRONTIER_PARTY_SIZE;

        for (u32 j = 0; j < FRONTIER_PARTY_SIZE; j++)
            if (GetMonData(mon, MON_DATA_SPECIES) == gBattleFrontierMons[monIds[j]].species)
                match = j;
        EXPECT_LT(match, FRONTIER_PARTY_SIZE);
        if (match < FRONTIER_PARTY_SIZE)
        {
            const struct TrainerMon *set = &gBattleFrontierMons[monIds[match]];

            EXPECT_EQ(GetMonData(mon, MON_DATA_HELD_ITEM), set->heldItem);
            for (u32 m = 0; m < MAX_MON_MOVES; m++)
                EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE1 + m), set->moves[m]);
        }
    }
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][2], MON_DATA_SPECIES), SPECIES_NONE);
}
