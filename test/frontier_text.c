#include "global.h"
#include "battle_frontier.h"
#include "easy_chat.h"
#include "frontier_util.h"
#include "item.h"
#include "line_break.h"
#include "overworld.h"
#include "string_util.h"
#include "strings.h"
#include "text.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/battle_frontier_trainers.h"
#include "constants/items.h"
#include "constants/frontier_util.h"

TEST("Frontier text: item header retains deliberate Japanese line breaks")
{
    u8 text[256];
    EXPECT_EQ(FormatItemDescriptionForHeader(ITEM_POTION, text), 2);
    EXPECT_EQ(StringCompare(text, GetItemDescription(ITEM_POTION)), 0);
    EXPECT_EQ(CountLineBreaks(text), 1);
}

TEST("Frontier text: all item descriptions fit the header width and stay above the message box")
{
    u8 text[256];
    enum Item item = ITEM_NONE;
    for (u32 i = ITEM_NONE; i < ITEMS_COUNT; i++)
        PARAMETRIZE { item = i; }
    EXPECT(StringLength(GetItemDescription(item)) < sizeof(text));
    u8 lines = FormatItemDescriptionForHeader(item, text);
    EXPECT(GetStringWidth(FONT_SMALL, text, -1) <= 196);
    EXPECT(lines * (GetFontAttribute(FONT_SMALL, FONTATTR_MAX_LETTER_HEIGHT)
        + GetFontAttribute(FONT_SMALL, FONTATTR_LINE_SPACING)) <= 96);
}

TEST("Frontier text: trainer Tokihito uses the Japanese original challenge")
{
    static const u8 expected[] = _("ボクは ぜったい まけ\nません {JPN}…！ ");
    gFacilityTrainers = gBattleFrontierTrainers;
    CopyFrontierTrainerText(FRONTIER_BEFORE_TEXT, FRONTIER_TRAINER_BRADY);
    EXPECT_EQ(StringCompare(gStringVar4, expected), 0);
}

TEST("Frontier text: trainer Hiromichi uses the original win and loss phrases")
{
    static const u8 win[] = _("わーい かって \nたのしい なあ ");
    static const u8 lose[] = _("わーん！！ \nやっぱり くやしい なあ");
    gFacilityTrainers = gBattleFrontierTrainers;
    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, FRONTIER_TRAINER_CONNER);
    EXPECT_EQ(StringCompare(gStringVar4, win), 0);
    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, FRONTIER_TRAINER_CONNER);
    EXPECT_EQ(StringCompare(gStringVar4, lose), 0);
}

TEST("Frontier text: original Hoenn species are remapped to Expansion IDs")
{
    bool32 gulpin = FALSE, loudred = FALSE, castform = FALSE, shelgon = FALSE, chimecho = FALSE;
    for (u32 trainer = 0; trainer < FRONTIER_TRAINERS_COUNT; trainer++)
    {
        const u16 *phrases[] = {gBattleFrontierTrainers[trainer].speechBefore,
            gBattleFrontierTrainers[trainer].speechWin, gBattleFrontierTrainers[trainer].speechLose};
        for (u32 phase = 0; phase < ARRAY_COUNT(phrases); phase++)
            for (u32 word = 0; word < EASY_CHAT_BATTLE_WORDS_COUNT; word++)
            {
                u16 value = phrases[phase][word];
                gulpin |= value == EC_POKEMON(GULPIN);
                loudred |= value == EC_POKEMON(LOUDRED);
                castform |= value == EC_POKEMON(CASTFORM);
                shelgon |= value == EC_POKEMON(SHELGON);
                chimecho |= value == EC_POKEMON(CHIMECHO);
            }
    }
    EXPECT(gulpin && loudred && castform && shelgon && chimecho);
}

TEST("Frontier text: all 900 original speeches contain valid words and fit the dialogue width")
{
    u8 text[64];
    u32 trainer = 0, phase = 0;
    for (u32 i = 0; i < FRONTIER_TRAINERS_COUNT; i++)
        for (u32 j = 0; j < 3; j++)
            PARAMETRIZE { trainer = i; phase = j; }
    const u16 *phrases[] = {gBattleFrontierTrainers[trainer].speechBefore,
        gBattleFrontierTrainers[trainer].speechWin, gBattleFrontierTrainers[trainer].speechLose};
    for (u32 i = 0; i < EASY_CHAT_BATTLE_WORDS_COUNT; i++)
    {
        CopyEasyChatWord(text, phrases[phase][i]);
        EXPECT_NE(StringCompare(text, gText_ThreeQuestionMarks), 0);
    }
    FrontierSpeechToString(phrases[phase]);
    // A two-line phrase may use scroll on its third line, just as in vanilla.
    for (u32 i = 0; gStringVar4[i] != EOS; i++)
        if (gStringVar4[i] == CHAR_PROMPT_SCROLL)
            gStringVar4[i] = CHAR_NEWLINE;
    EXPECT(GetStringWidth(FONT_NORMAL, gStringVar4, -1) <= 204);
}
