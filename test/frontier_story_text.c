#include "global.h"
#include "frontier_story.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"
#include "constants/characters.h"

#include "../src/data/frontier_story_named.h"
#include "../src/data/frontier_story_targets.h"
#include "../src/data/frontier_story_contest.h"

extern const u8 FinalStory_Text_EveryoneAwake[];
extern const u8 FinalStory_Text_WantedToBePokemon[];
extern const u8 FinalStory_Text_CannotBePokemon[];
extern const u8 FinalStory_Text_NameDoesNotForgive[];
extern const u8 FinalStory_Text_Nevertheless[];
extern const u8 FinalStory_Text_GiveSpeciesName[];
extern const u8 FinalStory_Text_NameRemains[];

static void CheckStoryText(const u8 *text)
{
    static const u8 wideGlyph[] = _("ア");
    u8 line[1000] = {EXT_CTRL_CODE_BEGIN, EXT_CTRL_CODE_JPN};
    u32 used = 2, total = 1;

    if (text == NULL)
        return;
    while (TRUE)
    {
        u32 ch = *text++;
        total++;
        EXPECT_LT(total, sizeof(gStringVar4));
        if (total >= sizeof(gStringVar4))
            return;
        if (ch == EOS || ch == CHAR_NEWLINE || ch == CHAR_PROMPT_CLEAR || ch == CHAR_PROMPT_SCROLL)
        {
            line[used] = EOS;
            EXPECT_LE(GetStringWidth(FONT_NORMAL, line, 0), 208);
            used = 2;
            if (ch == EOS)
                return;
        }
        else if (ch == PLACEHOLDER_BEGIN)
        {
            u32 id = *text++;
            u32 length = id == PLACEHOLDER_ID_PLAYER ? 7 : id == PLACEHOLDER_ID_STRING_VAR_1 ? 6 : 5;
            EXPECT(id == PLACEHOLDER_ID_PLAYER || id == PLACEHOLDER_ID_STRING_VAR_1 || id == PLACEHOLDER_ID_STRING_VAR_2);
            total += length;
            for (u32 i = 0; i < length; i++)
                line[used++] = wideGlyph[0];
        }
        else if (ch == EXT_CTRL_CODE_BEGIN)
        {
            u32 length = GetExtCtrlCodeLength(*text);
            EXPECT_NE(*text, EXT_CTRL_CODE_ENG);
            EXPECT_GT(length, 0);
            EXPECT_LT(used + length + 1, sizeof(line));
            if (used + length + 1 >= sizeof(line))
                return;
            line[used++] = ch;
            for (u32 i = 0; i < length; i++)
                line[used++] = *text++;
            total += length;
        }
        else
        {
            EXPECT_LT(used + 1, sizeof(line));
            if (used + 1 >= sizeof(line))
                return;
            line[used++] = ch;
        }
    }
}

TEST("Frontier story text: the final naming dialogue fits the Japanese field window")
{
    CheckStoryText(FinalStory_Text_EveryoneAwake);
    CheckStoryText(FinalStory_Text_WantedToBePokemon);
    CheckStoryText(FinalStory_Text_CannotBePokemon);
    CheckStoryText(FinalStory_Text_NameDoesNotForgive);
    CheckStoryText(FinalStory_Text_Nevertheless);
    CheckStoryText(FinalStory_Text_GiveSpeciesName);
    CheckStoryText(FinalStory_Text_NameRemains);
}

TEST("Frontier story text: named character dialogue fits the Japanese field window")
{
    CheckStoryText(sText_NamedCommonWords);
    CheckStoryText(sText_NamedUneasy);
    CheckStoryText(sText_NamedCommonIntro);
    CheckStoryText(sText_NamedUneasyIntro);
    CheckStoryText(sText_NamedRescuedChallenge);
    for (u32 i = 0; i < ARRAY_COUNT(sNamedStoryTexts); i++)
    {
        CheckStoryText(sNamedStoryTexts[i].recovered);
        CheckStoryText(sNamedStoryTexts[i].visit);
        CheckStoryText(sNamedStoryTexts[i].cured);
        CheckStoryText(sNamedStoryTexts[i].inn);
    }
}

TEST("Frontier story text: target and rescue dialogue fits after placeholder expansion")
{
    for (u32 i = 0; i < ARRAY_COUNT(sStoryTargetTexts); i++)
    {
        CheckStoryText(sStoryTargetTexts[i].meet);
        CheckStoryText(sStoryTargetTexts[i].happy);
        CheckStoryText(sStoryTargetTexts[i].wish);
        CheckStoryText(sStoryTargetTexts[i].wishAgain);
        CheckStoryText(sStoryTargetTexts[i].othersGone);
        CheckStoryText(sStoryTargetTexts[i].wishGone);
        CheckStoryText(sStoryTargetTexts[i].wishShared);
        for (u32 j = 0; j < RESCUE_TEXT_COUNT; j++)
            CheckStoryText(sStoryTargetRescueTexts[i][j]);
    }
}

TEST("Frontier story text: all contest branches fit the Japanese field window")
{
    for (u32 scene = 0; scene < CONTEST_STORY_SCENE_COUNT; scene++)
        for (u32 speaker = 0; speaker < 2; speaker++)
            for (u32 stage = 0; stage < STORY_RESCUE_STAGE_COUNT; stage++)
                CheckStoryText(sContestSceneBranches[scene][speaker][stage]);
}
