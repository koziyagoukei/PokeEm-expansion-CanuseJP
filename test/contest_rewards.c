#include "global.h"
#include "contest.h"
#include "event_data.h"
#include "script.h"
#include "string_util.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/flags.h"

extern const u8 ContestHall_EventScript_AddWinnerBattlePoints[];

static void SetUpContestReward(u32 rank, u32 category, bool32 won)
{
    gSpecialVar_ContestRank = rank;
    gSpecialVar_ContestCategory = category;
    VarSet(VAR_CONTEST_TYPE, rank + CONTEST_TYPE_NPC_NORMAL);
    gContestPlayerMonIndex = CONTESTANT_COUNT - 1;
    gContestFinalStandings[0] = won ? 3 : 0;
    gContestFinalStandings[1] = 1;
    gContestFinalStandings[2] = 2;
    gContestFinalStandings[3] = won ? 0 : 3;
    gSpecialVar_Result = 123;
    gSaveBlock2Ptr->frontier.battlePoints = 100;
}

TEST("Contest BP: each rank awards 3 5 7 10 points for wins in every category")
{
    static const u16 rewards[] = {3, 5, 7, 10};
    u32 rank = 0, category = 0;
    for (u32 r = CONTEST_RANK_NORMAL; r <= CONTEST_RANK_MASTER; r++)
        for (u32 c = 0; c < CONTEST_CATEGORIES_COUNT; c++)
            PARAMETRIZE { rank = r; category = c; }
    SetUpContestReward(rank, category, TRUE);
    RunScriptImmediately(ContestHall_EventScript_AddWinnerBattlePoints);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100 + rewards[rank]);
    EXPECT_EQ(gSpecialVar_Result, rewards[rank]);
    u8 expected[3];
    ConvertIntToDecimalStringN(expected, rewards[rank], STR_CONV_MODE_LEFT_ALIGN, 2);
    EXPECT_EQ(StringCompare(gStringVar1, expected), 0);
}

TEST("Contest BP: repeat wins award points even when the ribbon ceremony is skipped")
{
    SetUpContestReward(CONTEST_RANK_NORMAL, CONTEST_CATEGORY_COOL, TRUE);
    FlagSet(FLAG_TEMP_2);
    RunScriptImmediately(ContestHall_EventScript_AddWinnerBattlePoints);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 103);
    RunScriptImmediately(ContestHall_EventScript_AddWinnerBattlePoints);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 106);
    EXPECT(FlagGet(FLAG_TEMP_2));
    FlagClear(FLAG_TEMP_2);
}

TEST("Contest BP: losses and linked contests do not award points")
{
    bool32 link = FALSE;
    PARAMETRIZE { link = FALSE; }
    PARAMETRIZE { link = TRUE; }
    SetUpContestReward(CONTEST_RANK_MASTER, CONTEST_CATEGORY_SMART, link);
    if (link)
        VarSet(VAR_CONTEST_TYPE, CONTEST_TYPE_LINK);
    RunScriptImmediately(ContestHall_EventScript_AddWinnerBattlePoints);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    EXPECT_EQ(gSpecialVar_Result, 0);
}

TEST("Contest BP: award respects the existing 9999 BP cap")
{
    u32 rank = 0, points = 0;
    for (u32 r = CONTEST_RANK_NORMAL; r <= CONTEST_RANK_MASTER; r++)
        for (u32 p = MAX_BATTLE_FRONTIER_POINTS - 1; p <= MAX_BATTLE_FRONTIER_POINTS; p++)
            PARAMETRIZE { rank = r; points = p; }
    SetUpContestReward(rank, CONTEST_CATEGORY_BEAUTY, TRUE);
    gSaveBlock2Ptr->frontier.battlePoints = points;
    RunScriptImmediately(ContestHall_EventScript_AddWinnerBattlePoints);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, MAX_BATTLE_FRONTIER_POINTS);
}

TEST("Contest BP: invalid rank does not award points")
{
    SetUpContestReward(CONTEST_RANK_LINK, CONTEST_CATEGORY_CUTE, TRUE);
    VarSet(VAR_CONTEST_TYPE, CONTEST_TYPE_NPC_NORMAL);
    RunScriptImmediately(ContestHall_EventScript_AddWinnerBattlePoints);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    EXPECT_EQ(gSpecialVar_Result, 0);
}
