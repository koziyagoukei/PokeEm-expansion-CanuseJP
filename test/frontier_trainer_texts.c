#include "global.h"
#include "battle_frontier.h"
#include "battle_message.h"
#include "frontier_util.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
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
