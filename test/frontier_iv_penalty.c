#include "global.h"
#include "battle.h"
#include "battle_frontier.h"
#include "event_data.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/battle_pike.h"

static void SetStreak(u32 facility, u16 streak, u32 mode, u32 level)
{
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

static void SetUpChallenge(u32 facility, u16 streak, u32 mode, u32 level)
{
    Test_RestoreFactoryOpponentIVs();
    VarSet(VAR_FRONTIER_FACILITY, facility);
    VarSet(VAR_FRONTIER_BATTLE_MODE, mode);
    gSaveBlock2Ptr->frontier.lvlMode = level;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    if (facility == FRONTIER_FACILITY_FACTORY)
        gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_FACTORY;
    SetStreak(facility, streak, mode, level);
    for (u32 trainer = 0; trainer < MAX_BATTLE_TRAINERS; trainer++)
        for (u32 slot = 0; slot < PARTY_SIZE; slot++)
            ZeroMonData(&gParties[trainer][slot]);
}

static void MakeMon(enum BattleTrainer trainer, u32 slot, u32 fixedIV)
{
    CreateMonWithIVs(&gParties[trainer][slot], SPECIES_PIKACHU, 50,
        0x12345678, OTID_STRUCT_PRESET(0x12345678), fixedIV);
}

TEST("Frontier IV penalty: all facility round boundaries follow 30 20 10 0")
{
    for (u32 facility = 0; facility < NUM_FRONTIER_FACILITIES; facility++)
    {
        u32 stages = facility == FRONTIER_FACILITY_DOME ? 1
            : facility == FRONTIER_FACILITY_PIKE ? NUM_PIKE_ROOMS : FRONTIER_STAGES_PER_CHALLENGE;
        EXPECT_EQ(Test_GetFrontierOpponentIVPenalty(facility, 0), 30);
        EXPECT_EQ(Test_GetFrontierOpponentIVPenalty(facility, 2 * stages - 1), 30);
        EXPECT_EQ(Test_GetFrontierOpponentIVPenalty(facility, 2 * stages), 20);
        EXPECT_EQ(Test_GetFrontierOpponentIVPenalty(facility, 4 * stages - 1), 20);
        EXPECT_EQ(Test_GetFrontierOpponentIVPenalty(facility, 4 * stages), 10);
        EXPECT_EQ(Test_GetFrontierOpponentIVPenalty(facility, 6 * stages - 1), 10);
        EXPECT_EQ(Test_GetFrontierOpponentIVPenalty(facility, 6 * stages), 0);
        EXPECT_EQ(Test_GetFrontierOpponentIVPenalty(facility, 9999), 0);
    }
    EXPECT_EQ(Test_GetFrontierOpponentIVPenalty(NUM_FRONTIER_FACILITIES, 0), 0);
}

TEST("Frontier IV penalty: saved streaks use the selected facility mode and level")
{
    u32 facility = 0, mode = 0, level = 0;
    for (u32 f = 0; f < NUM_FRONTIER_FACILITIES; f++)
        for (u32 m = 0; m <= (f == FRONTIER_FACILITY_TOWER ? FRONTIER_MODE_MULTIS : FRONTIER_MODE_DOUBLES); m++)
            for (u32 l = 0; l < FRONTIER_LVL_MODE_COUNT; l++)
                PARAMETRIZE { facility = f; mode = m; level = l; }
    u32 stages = facility == FRONTIER_FACILITY_DOME ? 1
        : facility == FRONTIER_FACILITY_PIKE ? NUM_PIKE_ROOMS : FRONTIER_STAGES_PER_CHALLENGE;
    SetUpChallenge(facility, 2 * stages, mode, level);
    MakeMon(B_TRAINER_OPPONENT_A, 0, 31);
    Test_ApplyFrontierOpponentIVPenalty();
    for (u32 stat = 0; stat < NUM_STATS; stat++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_HP_IV + stat), 11);
}

TEST("Frontier IV penalty: each IV saturates at zero and stats are recalculated")
{
    static const u32 original[NUM_STATS] = {31, 0, 20, 30, 9, 10};
    static const u32 expected[NUM_STATS] = {11, 0, 0, 10, 0, 0};
    struct Pokemon expectedMon;
    SetUpChallenge(FRONTIER_FACILITY_TOWER, 14, FRONTIER_MODE_SINGLES, FRONTIER_LVL_50);
    MakeMon(B_TRAINER_OPPONENT_A, 0, 31);
    for (u32 stat = 0; stat < NUM_STATS; stat++)
        SetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_HP_IV + stat, &original[stat]);
    CalculateMonStats(&gParties[B_TRAINER_OPPONENT_A][0]);
    expectedMon = gParties[B_TRAINER_OPPONENT_A][0];
    for (u32 stat = 0; stat < NUM_STATS; stat++)
        SetMonData(&expectedMon, MON_DATA_HP_IV + stat, &expected[stat]);
    CalculateMonStats(&expectedMon);
    Test_ApplyFrontierOpponentIVPenalty();
    EXPECT_EQ(memcmp(&expectedMon, &gParties[B_TRAINER_OPPONENT_A][0], sizeof(expectedMon)), 0);
}

TEST("Frontier IV penalty: set-specific packed IVs are reduced after their override")
{
    static const struct TrainerMon fmon = {
        .species = SPECIES_PIKACHU,
        .iv = 31 | (30 << 5) | (20 << 10) | (10 << 15) | (0 << 20) | (31 << 25),
    };
    static const u32 expected[NUM_STATS] = {1, 0, 0, 0, 0, 1};
    SetUpChallenge(FRONTIER_FACILITY_TOWER, 0, FRONTIER_MODE_SINGLES, FRONTIER_LVL_50);
    CreateFacilityMon(&fmon, 50, 0, 0x12345678, 0, &gParties[B_TRAINER_OPPONENT_A][0]);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_HP_IV), 31);
    Test_ApplyFrontierOpponentIVPenalty();
    for (u32 stat = 0; stat < NUM_STATS; stat++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_HP_IV + stat), expected[stat]);
}

TEST("Frontier IV penalty: both opponents change but player and partner remain byte-identical")
{
    struct Pokemon player, partner;
    SetUpChallenge(FRONTIER_FACILITY_TOWER, 0, FRONTIER_MODE_MULTIS, FRONTIER_LVL_OPEN);
    MakeMon(B_TRAINER_PLAYER, 0, 31);
    MakeMon(B_TRAINER_PARTNER, 0, 31);
    MakeMon(B_TRAINER_OPPONENT_A, 0, 31);
    MakeMon(B_TRAINER_OPPONENT_B, 1, 31);
    player = gParties[B_TRAINER_PLAYER][0];
    partner = gParties[B_TRAINER_PARTNER][0];
    Test_ApplyFrontierOpponentIVPenalty();
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_ATK_IV), 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_B][1], MON_DATA_ATK_IV), 1);
    EXPECT_EQ(memcmp(&player, &gParties[B_TRAINER_PLAYER][0], sizeof(player)), 0);
    EXPECT_EQ(memcmp(&partner, &gParties[B_TRAINER_PARTNER][0], sizeof(partner)), 0);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][1], MON_DATA_SPECIES), SPECIES_NONE);
}

TEST("Frontier IV penalty: seventh round keeps the complete opponent unchanged")
{
    struct Pokemon before;
    SetUpChallenge(FRONTIER_FACILITY_TOWER, 42, FRONTIER_MODE_SINGLES, FRONTIER_LVL_50);
    MakeMon(B_TRAINER_OPPONENT_A, 0, 31);
    before = gParties[B_TRAINER_OPPONENT_A][0];
    Test_ApplyFrontierOpponentIVPenalty();
    EXPECT_EQ(memcmp(&before, &gParties[B_TRAINER_OPPONENT_A][0], sizeof(before)), 0);
}

TEST("Frontier IV penalty: losing and restarting returns to the first-round penalty")
{
    SetUpChallenge(FRONTIER_FACILITY_TOWER, 28, FRONTIER_MODE_SINGLES, FRONTIER_LVL_50);
    MakeMon(B_TRAINER_OPPONENT_A, 0, 31);
    Test_ApplyFrontierOpponentIVPenalty();
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_ATK_IV), 21);
    SetStreak(FRONTIER_FACILITY_TOWER, 0, FRONTIER_MODE_SINGLES, FRONTIER_LVL_50);
    MakeMon(B_TRAINER_OPPONENT_A, 0, 31);
    Test_ApplyFrontierOpponentIVPenalty();
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_ATK_IV), 1);
}

TEST("Frontier IV penalty: normal wild link and recorded battles stay unchanged")
{
    u32 flags = 0;
    PARAMETRIZE { flags = BATTLE_TYPE_TRAINER; }
    PARAMETRIZE { flags = BATTLE_TYPE_BATTLE_TOWER; }
    PARAMETRIZE { flags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_LINK; }
    PARAMETRIZE { flags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_RECORDED; }
    PARAMETRIZE { flags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_RECORDED_LINK; }
    struct Pokemon before;
    SetUpChallenge(FRONTIER_FACILITY_TOWER, 0, FRONTIER_MODE_SINGLES, FRONTIER_LVL_50);
    MakeMon(B_TRAINER_OPPONENT_A, 0, 31);
    before = gParties[B_TRAINER_OPPONENT_A][0];
    gBattleTypeFlags = flags;
    Test_ApplyFrontierOpponentIVPenalty();
    EXPECT_EQ(memcmp(&before, &gParties[B_TRAINER_OPPONENT_A][0], sizeof(before)), 0);
}

TEST("Frontier IV penalty: tents and invalid facility or mode cannot index streak arrays")
{
    u32 facility = FRONTIER_FACILITY_TOWER, mode = FRONTIER_MODE_SINGLES, level = FRONTIER_LVL_50;
    PARAMETRIZE { level = FRONTIER_LVL_TENT; }
    PARAMETRIZE { facility = NUM_FRONTIER_FACILITIES; }
    PARAMETRIZE { mode = FRONTIER_MODE_LINK_MULTIS; }
    PARAMETRIZE { mode = 0xFFFF; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_DOME; mode = FRONTIER_MODE_MULTIS; }
    struct Pokemon before;
    SetUpChallenge(FRONTIER_FACILITY_TOWER, 0, FRONTIER_MODE_SINGLES, FRONTIER_LVL_50);
    VarSet(VAR_FRONTIER_FACILITY, facility);
    VarSet(VAR_FRONTIER_BATTLE_MODE, mode);
    gSaveBlock2Ptr->frontier.lvlMode = level;
    MakeMon(B_TRAINER_OPPONENT_A, 0, 31);
    before = gParties[B_TRAINER_OPPONENT_A][0];
    Test_ApplyFrontierOpponentIVPenalty();
    EXPECT_EQ(memcmp(&before, &gParties[B_TRAINER_OPPONENT_A][0], sizeof(before)), 0);
}

TEST("Frontier IV penalty: Factory restores original IVs before rental exchange and cannot restore twice")
{
    u32 original[FRONTIER_PARTY_SIZE];
    SetUpChallenge(FRONTIER_FACILITY_FACTORY, 0, FRONTIER_MODE_SINGLES, FRONTIER_LVL_50);
    for (u32 slot = 0; slot < FRONTIER_PARTY_SIZE; slot++)
    {
        MakeMon(B_TRAINER_OPPONENT_A, slot, 20 + slot);
        original[slot] = GetMonData(&gParties[B_TRAINER_OPPONENT_A][slot], MON_DATA_IVS);
    }
    Test_ApplyFrontierOpponentIVPenalty();
    for (u32 slot = 0; slot < FRONTIER_PARTY_SIZE; slot++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][slot], MON_DATA_ATK_IV), 0);
    Test_RestoreFactoryOpponentIVs();
    for (u32 slot = 0; slot < FRONTIER_PARTY_SIZE; slot++)
    {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][slot], MON_DATA_IVS), original[slot]);
        MakeMon(B_TRAINER_OPPONENT_A, slot, 31);
    }
    Test_RestoreFactoryOpponentIVs();
    for (u32 slot = 0; slot < FRONTIER_PARTY_SIZE; slot++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][slot], MON_DATA_ATK_IV), 31);
}
