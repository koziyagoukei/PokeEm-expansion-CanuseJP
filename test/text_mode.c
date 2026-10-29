#include "global.h"
#include "battle.h"
#include "battle_message.h"
#include "contest.h"
#include "dynamic_placeholder_text_util.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"
#include "constants/characters.h"

TEST("Text mode: the check finds kana drawn with the English font")
{
    EXPECT_NE(FindTextModeMismatch(COMPOUND_STRING("{ENG}12こ")), 0);
    EXPECT_EQ(FindTextModeMismatch(COMPOUND_STRING("{ENG}12{JPN}こ")), 0);
    // "%" and "Lv" share codes with kana but are meant in English text.
    EXPECT_EQ(FindTextModeMismatch(COMPOUND_STRING("{JPN}10{ENG}%{JPN}で")), 0);
    EXPECT_EQ(FindTextModeMismatch(COMPOUND_STRING("{ENG}{LV}50")), 0);
}

TEST("Text mode: {AUTO} looks past punctuation to the next letter")
{
    EXPECT_EQ(FindTextModeMismatch(COMPOUND_STRING("{ENG}AB{AUTO}!\nこんにちは")), 0);
    EXPECT_NE(FindTextModeMismatch(COMPOUND_STRING("{JPN}こ{AUTO}!AB\nこんにちは")), 0);
}

// The contest results line that showed "おめでとう ございます!" with the English font:
// the winner's name is copied with {AUTO} codes and followed by "!".
TEST("Text mode: a contest name before punctuation hands the line back to Japanese")
{
    Contest_CopyDisplayName(gStringVar1, COMPOUND_STRING("たきのぼり"));
    Contest_CopyDisplayName(gStringVar3, COMPOUND_STRING("リキ"));
    StringCopy(gStringVar2, COMPOUND_STRING("4"));
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{JPN}エントリーナンバー {STR_VAR_2}ばん\n{STR_VAR_3}さん! そして {STR_VAR_1}!\lおめでとう ございます!"));
    EXPECT_EQ(FindTextModeMismatch(gStringVar4), 0);
}

TEST("Text mode: an English number in a buffer does not carry over to the text after it")
{
    StringCopy(StringCopy(gStringVar1, COMPOUND_STRING("{ENG}")), COMPOUND_STRING("12"));
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{STR_VAR_1}こ てにいれた"));
    EXPECT_EQ(FindTextModeMismatch(gStringVar4), 0);
}

TEST("Text mode: a Japanese name in English text gives the text back to English")
{
    StringCopy(gStringVar1, COMPOUND_STRING("{JPN}あいう"));
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{ENG}Hi {STR_VAR_1}!"));
    EXPECT_EQ(GetTextModeCode(gStringVar4, NULL), EXT_CTRL_CODE_ENG);
}

TEST("Text mode: buffers without font codes add nothing")
{
    StringCopy(gStringVar1, COMPOUND_STRING("リキ"));
    u8 *end = StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("{STR_VAR_1}さん"));
    EXPECT_EQ(end - gStringVar4, StringLength(COMPOUND_STRING("リキさん")));
}

TEST("Text mode: dynamic placeholders and battle buffers give the mode back too")
{
    DynamicPlaceholderTextUtil_Reset();
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, COMPOUND_STRING("{ENG}12"));
    DynamicPlaceholderTextUtil_ExpandPlaceholders(gStringVar4, COMPOUND_STRING("{DYNAMIC 0}こ"));
    EXPECT_EQ(FindTextModeMismatch(gStringVar4), 0);

    StringCopy(gBattleTextBuff1, COMPOUND_STRING("{ENG}12"));
    BattleStringExpandPlaceholders(COMPOUND_STRING("{B_BUFF1}こ"), gDisplayedStringBattle, sizeof(gDisplayedStringBattle));
    EXPECT_EQ(FindTextModeMismatch(gDisplayedStringBattle), 0);
}
