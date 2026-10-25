#include "global.h"
#include "battle.h"
#include "battle_ai_main.h"
#include "battle_frontier.h"
#include "battle_setup.h"
#include "contest.h"
#include "contest_ai.h"
#include "event_data.h"
#include "malloc.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/battle_ai.h"
#include "constants/battle_frontier.h"

static const u8 sEVs[NUM_STATS] = {252, 4, 0, 0, 252, 0};

static void SetUpChallenge(u32 facility, u32 aiLevel)
{
    Test_RestoreFactoryOpponentStats();
    VarSet(VAR_FRONTIER_AI_LEVEL, aiLevel);
    VarSet(VAR_FRONTIER_FACILITY, facility);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_50;
    // A late round, so the IV handicap does not change the stats being checked.
    gSaveBlock2Ptr->frontier.towerWinStreaks[FRONTIER_MODE_SINGLES][FRONTIER_LVL_50] = 100;
    gSaveBlock2Ptr->frontier.factoryWinStreaks[FRONTIER_MODE_SINGLES][FRONTIER_LVL_50] = 100;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | (facility == FRONTIER_FACILITY_FACTORY ? BATTLE_TYPE_FACTORY : BATTLE_TYPE_BATTLE_TOWER);
    for (u32 trainer = 0; trainer < MAX_BATTLE_TRAINERS; trainer++)
        for (u32 slot = 0; slot < PARTY_SIZE; slot++)
            ZeroMonData(&gParties[trainer][slot]);
}

static void MakeMon(enum BattleTrainer trainer, u32 slot)
{
    struct Pokemon *mon = &gParties[trainer][slot];

    CreateMonWithIVs(mon, SPECIES_PIKACHU, 50, 0x12345678, OTID_STRUCT_PRESET(0x12345678), 31);
    for (u32 stat = 0; stat < NUM_STATS; stat++)
        SetMonData(mon, MON_DATA_HP_EV + stat, &sEVs[stat]);
    CalculateMonStats(mon);
}

static bool32 HasEVs(enum BattleTrainer trainer, u32 slot)
{
    for (u32 stat = 0; stat < NUM_STATS; stat++)
    {
        if (GetMonData(&gParties[trainer][slot], MON_DATA_HP_EV + stat) != sEVs[stat])
            return FALSE;
    }
    return TRUE;
}

static bool32 HasNoEVs(enum BattleTrainer trainer, u32 slot)
{
    for (u32 stat = 0; stat < NUM_STATS; stat++)
    {
        if (GetMonData(&gParties[trainer][slot], MON_DATA_HP_EV + stat) != 0)
            return FALSE;
    }
    return TRUE;
}

TEST("Frontier easy mode: opponents battle without EVs and the player keeps theirs")
{
    u32 maxHp;

    SetUpChallenge(FRONTIER_FACILITY_TOWER, FRONTIER_AI_LEVEL_EASY);
    MakeMon(B_TRAINER_PLAYER, 0);
    MakeMon(B_TRAINER_PARTNER, 0);
    MakeMon(B_TRAINER_OPPONENT_A, 0);
    MakeMon(B_TRAINER_OPPONENT_B, 1);
    maxHp = GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_MAX_HP);

    Test_ApplyFrontierOpponentHandicaps();

    EXPECT(HasNoEVs(B_TRAINER_OPPONENT_A, 0));
    EXPECT(HasNoEVs(B_TRAINER_OPPONENT_B, 1));
    EXPECT_LT(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_MAX_HP), maxHp);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_HP),
              GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_MAX_HP));
    EXPECT(HasEVs(B_TRAINER_PLAYER, 0));
    EXPECT(HasEVs(B_TRAINER_PARTNER, 0));
}

TEST("Frontier easy mode: other AI levels and link battles keep opponent EVs")
{
    u32 aiLevel = 0, battleFlags = 0;
    PARAMETRIZE { aiLevel = FRONTIER_AI_LEVEL_NORMAL; battleFlags = 0; }
    PARAMETRIZE { aiLevel = FRONTIER_AI_LEVEL_HARD; battleFlags = 0; }
    PARAMETRIZE { aiLevel = FRONTIER_AI_LEVEL_STRONGEST; battleFlags = 0; }
    PARAMETRIZE { aiLevel = FRONTIER_AI_LEVEL_EASY; battleFlags = BATTLE_TYPE_LINK; }
    PARAMETRIZE { aiLevel = FRONTIER_AI_LEVEL_EASY; battleFlags = BATTLE_TYPE_RECORDED; }

    SetUpChallenge(FRONTIER_FACILITY_TOWER, aiLevel);
    gBattleTypeFlags |= battleFlags;
    MakeMon(B_TRAINER_OPPONENT_A, 0);

    Test_ApplyFrontierOpponentHandicaps();

    EXPECT(HasEVs(B_TRAINER_OPPONENT_A, 0));
}

TEST("Frontier easy mode: Factory opponents get their EVs back after the battle")
{
    SetUpChallenge(FRONTIER_FACILITY_FACTORY, FRONTIER_AI_LEVEL_EASY);
    for (u32 slot = 0; slot < FRONTIER_PARTY_SIZE; slot++)
    {
        MakeMon(B_TRAINER_PLAYER, slot);
        MakeMon(B_TRAINER_OPPONENT_A, slot);
    }

    Test_ApplyFrontierOpponentHandicaps();
    for (u32 slot = 0; slot < FRONTIER_PARTY_SIZE; slot++)
    {
        EXPECT(HasNoEVs(B_TRAINER_OPPONENT_A, slot));
        // The player's rentals keep their EVs during the battle.
        EXPECT(HasEVs(B_TRAINER_PLAYER, slot));
    }

    // The opponents can be swapped into the player's rentals afterwards.
    Test_RestoreFactoryOpponentStats();
    for (u32 slot = 0; slot < FRONTIER_PARTY_SIZE; slot++)
        EXPECT(HasEVs(B_TRAINER_OPPONENT_A, slot));
}

TEST("Frontier easy mode: opponents use a simple AI and the partner keeps its AI")
{
    u32 facilityFlag = 0;
    PARAMETRIZE { facilityFlag = BATTLE_TYPE_BATTLE_TOWER; }
    PARAMETRIZE { facilityFlag = BATTLE_TYPE_FACTORY; }
    PARAMETRIZE { facilityFlag = BATTLE_TYPE_PALACE; }
    PARAMETRIZE { facilityFlag = BATTLE_TYPE_PYRAMID; }

    VarSet(VAR_FRONTIER_AI_LEVEL, FRONTIER_AI_LEVEL_EASY);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | facilityFlag;
    TRAINER_BATTLE_PARAM.opponentA = 1;

    EXPECT_EQ(Test_GetAiFlags(1, B_BATTLER_1), AI_FLAG_CHECK_BAD_MOVE);
    EXPECT_EQ(Test_GetAiFlags(1, B_BATTLER_3), AI_FLAG_CHECK_BAD_MOVE);
    if (facilityFlag == BATTLE_TYPE_BATTLE_TOWER)
        EXPECT(Test_GetAiFlags(1, B_BATTLER_2) & AI_FLAG_CHECK_VIABILITY);

    VarSet(VAR_FRONTIER_AI_LEVEL, FRONTIER_AI_LEVEL_NORMAL);
    EXPECT(Test_GetAiFlags(1, B_BATTLER_1) != AI_FLAG_CHECK_BAD_MOVE);
}

TEST("Frontier easy mode: contest NPCs only avoid bad moves outside link contests")
{
    u32 link = 0;
    PARAMETRIZE { link = FALSE; }
    PARAMETRIZE { link = TRUE; }

    gContestResources = AllocZeroed(sizeof(*gContestResources));
    gContestResources->aiData = AllocZeroed(sizeof(struct ContestAIInfo));
    gLinkContestFlags = link ? LINK_CONTEST_FLAG_IS_LINK : 0;
    gContestMons[1].aiFlags = CONTEST_AI_CHECK_BAD_MOVE | CONTEST_AI_CHECK_COMBO | CONTEST_AI_CHECK_EXCITEMENT | CONTEST_AI_CHECK_GOOD_MOVE;

    VarSet(VAR_FRONTIER_AI_LEVEL, FRONTIER_AI_LEVEL_EASY);
    ContestAI_ResetAI(1);
    EXPECT_EQ(eContestAI.aiFlags, link ? gContestMons[1].aiFlags : CONTEST_AI_CHECK_BAD_MOVE);

    VarSet(VAR_FRONTIER_AI_LEVEL, FRONTIER_AI_LEVEL_HARD);
    ContestAI_ResetAI(1);
    EXPECT_EQ(eContestAI.aiFlags, gContestMons[1].aiFlags);

    gLinkContestFlags = 0;
    FREE_AND_SET_NULL(gContestResources->aiData);
    FREE_AND_SET_NULL(gContestResources);
}
