#include "global.h"
#include "battle.h"
#include "battle_frontier.h"
#include "battle_message.h"
#include "battle_setup.h"
#include "battle_tower.h"
#include "event_data.h"
#include "frontier_util.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/battle_tower.h"
#include "constants/characters.h"
#include "constants/frontier_util.h"
#include "constants/trainers.h"

static u32 CountLines(const u8 *str)
{
    u32 lines = 1;
    for (; *str != EOS; str++)
        if (*str == CHAR_NEWLINE || *str == CHAR_PROMPT_SCROLL || *str == CHAR_PROMPT_CLEAR)
            lines++;
    return lines;
}

TEST("Frontier trainers: every trainer has three written lines that fit the message box")
{
    static const u8 whichTexts[] = {FRONTIER_BEFORE_TEXT, FRONTIER_PLAYER_LOST_TEXT, FRONTIER_PLAYER_WON_TEXT};

    gFacilityTrainers = gBattleFrontierTrainers;
    for (u32 trainerId = 0; trainerId < FRONTIER_TRAINERS_COUNT; trainerId++)
    {
        for (u32 i = 0; i < ARRAY_COUNT(whichTexts); i++)
        {
            gStringVar4[0] = EOS;
            EXPECT(CopyFrontierTrainerOwnText(whichTexts[i], trainerId));
            EXPECT_NE(gStringVar4[0], EOS);
            EXPECT(IsJapaneseTextMode(gStringVar4));
            if (whichTexts[i] == FRONTIER_BEFORE_TEXT)
                EXPECT_LE(CountLines(gStringVar4), 2);
            for (u32 line = 0; line < CountLines(gStringVar4); line++)
                EXPECT_LE(GetStringLineWidth(FONT_NORMAL, gStringVar4, 0, line, sizeof(gStringVar4)), BATTLE_MSG_MAX_WIDTH);
        }
        // The written lines are the ones the facilities show.
        CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, trainerId);
        EXPECT(CopyFrontierTrainerOwnText(FRONTIER_PLAYER_WON_TEXT, trainerId));
    }
}

TEST("Frontier trainers: ids outside the Frontier's own trainers keep their Easy Chat words")
{
    gFacilityTrainers = gBattleFrontierTrainers;
    EXPECT(!CopyFrontierTrainerOwnText(FRONTIER_BEFORE_TEXT, FRONTIER_TRAINERS_COUNT));
    EXPECT(!CopyFrontierTrainerOwnText(FRONTIER_BEFORE_TEXT, TRAINER_FRONTIER_BRAIN));
    EXPECT(!CopyFrontierTrainerOwnText(FRONTIER_BEFORE_TEXT, TRAINER_RECORD_MIXING_FRIEND));
}

TEST("Frontier trainers: Tower script intros use written challenges for every mode and opponent")
{
    static const u32 modes[] = {FRONTIER_MODE_SINGLES, FRONTIER_MODE_DOUBLES, FRONTIER_MODE_MULTIS, FRONTIER_MODE_LINK_MULTIS};
    u32 levelMode = FRONTIER_LVL_50, mode = FRONTIER_MODE_SINGLES, opponent = 0;
    u8 expected[sizeof(gStringVar4)];

    for (u32 l = FRONTIER_LVL_50; l <= FRONTIER_LVL_OPEN; l++)
        for (u32 m = 0; m < ARRAY_COUNT(modes); m++)
            for (u32 side = 0; side < (modes[m] >= FRONTIER_MODE_MULTIS ? 2 : 1); side++)
                PARAMETRIZE { levelMode = l; mode = modes[m]; opponent = side; }

    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_TOWER);
    VarSet(VAR_FRONTIER_BATTLE_MODE, mode);
    gSaveBlock2Ptr->frontier.lvlMode = levelMode;
    for (u32 trainerId = 0; trainerId < FRONTIER_TRAINERS_COUNT; trainerId++)
    {
        gFacilityTrainers = gBattleFrontierTrainers;
        EXPECT(CopyFrontierTrainerOwnText(FRONTIER_BEFORE_TEXT, trainerId));
        StringCopy(expected, gStringVar4);

        TRAINER_BATTLE_PARAM.opponentA = opponent == 0 ? trainerId : (trainerId + 1) % FRONTIER_TRAINERS_COUNT;
        TRAINER_BATTLE_PARAM.opponentB = opponent == 1 ? trainerId : (trainerId + 1) % FRONTIER_TRAINERS_COUNT;
        gFacilityTrainers = NULL;
        gStringVar4[0] = EOS;
        gSpecialVar_0x8004 = BATTLE_TOWER_FUNC_GET_OPPONENT_INTRO;
        gSpecialVar_0x8005 = opponent;
        CallBattleTowerFunc();

        EXPECT_EQ(gFacilityTrainers, gBattleFrontierTrainers);
        EXPECT_EQ(StringCompare(gStringVar4, expected), 0);
        EXPECT(IsJapaneseTextMode(gStringVar4));
    }
}

TEST("Frontier trainers: Tent intros retain their original Easy Chat text")
{
    u8 expected[sizeof(gStringVar4)];
    u32 opponent = 0;
    PARAMETRIZE { opponent = 0; }
    PARAMETRIZE { opponent = 1; }

    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_FACTORY);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_DOUBLES);
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_TENT;
    TRAINER_BATTLE_PARAM.opponentA = 0;
    TRAINER_BATTLE_PARAM.opponentB = 1;
    FrontierSpeechToString(gSlateportBattleTentTrainers[opponent].speechBefore);
    StringCopy(expected, gStringVar4);
    gFacilityTrainers = NULL;
    gStringVar4[0] = EOS;
    gSpecialVar_0x8004 = BATTLE_TOWER_FUNC_GET_OPPONENT_INTRO;
    gSpecialVar_0x8005 = opponent;
    CallBattleTowerFunc();

    EXPECT_EQ(gFacilityTrainers, gSlateportBattleTentTrainers);
    EXPECT_EQ(StringCompare(gStringVar4, expected), 0);
    EXPECT(!CopyFrontierTrainerOwnText(FRONTIER_BEFORE_TEXT, opponent));
}
