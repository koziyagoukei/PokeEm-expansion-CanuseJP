#include "global.h"
#include "battle.h"
#include "contest.h"
#include "contest_effect.h"
#include "line_break.h"
#include "string_util.h"
#include "strings.h"
#include "text.h"
#include "test/test.h"
#include "../src/data/contest_text_tables.h"

extern const u8 gText_AppealNumWhichMoveWillBePlayed[];
extern const u8 gText_MonCantAppealNextTurn[];
extern const u8 gText_MonsMoveIsIgnored[];

TEST("Contest text: effect descriptions use Japanese mode and fit two lines")
{
    for (u32 effect = 0; effect <= CONTEST_EFFECT_DONT_EXCITE_AUDIENCE; effect++)
    {
        const u8 *text = gContestEffects[effect].description;
        EXPECT(IsJapaneseTextMode(text));
        EXPECT_LT(StringLength(text), 256);
        EXPECT_LE(GetStringWidth(FONT_NARROWER, text, 0), 144);
        EXPECT_LE(CountLineBreaks((u8 *)text) + 1, 2);
    }
}

static void PrepareNames(void)
{
    static const u8 name[] = _("abcdefghij");
    Contest_CopyDisplayName(gStringVar1, name);
    StringCopy(gStringVar2, COMPOUND_STRING("{JPN}キョダイヒャッカ"));
    StringCopy(gStringVar3, COMPOUND_STRING("{JPN}かっこよさ"));
}

static void CheckMessage(const u8 *text)
{
    EXPECT(IsJapaneseTextMode(text));
    StringExpandPlaceholders(gStringVar4, text);
    EXPECT_LT(StringLength(gStringVar4), sizeof(gStringVar4) - 64);
    u32 font = Contest_PrepareDisplayMessage(gStringVar4, 136);
    EXPECT_LE(GetStringWidth(font, gStringVar4, 0), 136);
}

TEST("Contest text: all appeal messages fit after long name substitution")
{
    PrepareNames();
    for (u32 i = 0; i < ARRAY_COUNT(sAppealResultTexts); i++)
        CheckMessage(sAppealResultTexts[i]);
}

TEST("Contest text: general messages fit without modifying source variables")
{
    PrepareNames();
    u8 before[sizeof(gStringVar1)];
    memcpy(before, gStringVar1, sizeof(before));
    CheckMessage(gText_MonCantAppealNextTurn);
    CheckMessage(gText_MonsMoveIsIgnored);
    CheckMessage(gText_JudgeLookedAtMonExpectantly);
    CheckMessage(gText_AppealComboWentOverExcellently);
    EXPECT_EQ(memcmp(before, gStringVar1, sizeof(before)), 0);
}

TEST("Contest text: fitting messages retain the authored breaks and pause controls")
{
    static const u8 text[] = _("{JPN}審査が 終了！\n結果 発表！{PAUSE 15}");
    Contest_PrepareDisplayMessage(text, 136);
    EXPECT_EQ(StringCompare(text, gStringVar4), 0);
}

TEST("Contest text: result text stays within its local winner buffer")
{
    static const u8 trainer[] = _("あいうえおかき");
    static const u8 mon[] = _("abcdefghij");
    struct { u8 before, text[100], after; } guarded;
    memset(&guarded, 0xA5, sizeof(guarded));
    Contest_CopyDisplayName(gStringVar1, trainer);
    Contest_CopyDisplayName(gStringVar2, mon);
    StringExpandPlaceholders(guarded.text, gText_ContestantsMonWon);
    EXPECT_EQ(guarded.before, 0xA5);
    EXPECT_EQ(guarded.after, 0xA5);
    EXPECT_LT(StringLength(guarded.text), sizeof(guarded.text));
}
