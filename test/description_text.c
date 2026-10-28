#include "global.h"
#include "test/test.h"
#include "battle.h"
#include "battle_message.h"
#include "item.h"
#include "line_break.h"
#include "move.h"
#include "overworld.h"
#include "string_util.h"
#include "text.h"
#include "constants/abilities.h"
#include "constants/items.h"
#include "constants/moves.h"

static void CheckDescription(const u8 *text, u32 font, u32 width, u32 lines)
{
    EXPECT(text != NULL);
    EXPECT(IsJapaneseTextMode(text));
    EXPECT_LT(StringLength(text), 256);
    EXPECT_LE(CountLineBreaks((u8 *)text) + 1, lines);
    EXPECT_LE(GetStringWidth(font, text, 0), width);
}

TEST("Descriptions: all moves fit the summary and battle information windows")
{
    enum Move move = MOVE_NONE;
    for (u32 i = MOVE_NONE; i < MOVES_COUNT_ALL; i++)
        PARAMETRIZE_LABEL("%S", GetMoveName(i)) { move = i; }
    CheckDescription(GetMoveDescription(move), FONT_NARROWER, 144, 2);
    // The battle information header and possible font controls share this buffer.
    EXPECT_LT(StringLength(GetMoveDescription(move)) + 64, sizeof(gDisplayedStringBattle));
}

TEST("Descriptions: all abilities fit below the ability name")
{
    enum Ability ability = ABILITY_NONE;
    for (u32 i = ABILITY_NONE; i < ABILITIES_COUNT; i++)
        PARAMETRIZE_LABEL("%S", gAbilitiesInfo[i].name) { ability = i; }
    CheckDescription(gAbilitiesInfo[ability].description, FONT_NARROWER, 144, 1);
}

TEST("Descriptions: all items fit bag shop and item notification windows")
{
    enum Item item = ITEM_NONE;
    for (u32 i = ITEM_NONE; i < ITEMS_COUNT; i++)
        PARAMETRIZE_LABEL("%S", gItemsInfo[i].name) { item = i; }
    const u8 *description = GetItemDescription(item);
    CheckDescription(description, FONT_NORMAL, 102, 3);
    struct {
        u8 before;
        u8 text[256];
        u8 after;
    } guarded;
    memset(&guarded, 0xA5, sizeof(guarded));
    EXPECT_LE(FormatItemDescriptionForHeader(item, guarded.text), 3);
    EXPECT_EQ(guarded.before, 0xA5);
    EXPECT_EQ(guarded.after, 0xA5);
    EXPECT_LE(GetStringWidth(FONT_SMALL, guarded.text, -1), 196);
}

TEST("Descriptions: item names also remain free of kanji")
{
    for (enum Item item = ITEM_NONE; item < ITEMS_COUNT; item++)
    {
        const u8 *text = gItemsInfo[item].name;
        while (*text != EOS)
        {
            if (*text == EXT_CTRL_CODE_BEGIN)
            {
                EXPECT(!IS_KANJI_GLYPH_CODE(text[1]));
                text += 1 + GetExtCtrlCodeLength(text[1]);
            }
            else
                text++;
        }
    }
}

TEST("Descriptions: Shell Smash includes speed and applies every change to the user")
{
    static const u8 expected[] = _("{JPN}自分の攻撃 特攻 素早さ{ENG}+{JPN}2\n自分の防御 特防{ENG}-{JPN}1");
    const struct MoveInfo *move = &gMovesInfo[MOVE_SHELL_SMASH];
    EXPECT_EQ((u32)move->target, TARGET_USER);
    EXPECT_EQ((u32)move->additionalEffects[0].defense, 1);
    EXPECT_EQ((u32)move->additionalEffects[0].spDef, 1);
    EXPECT_EQ((u32)move->additionalEffects[1].attack, 2);
    EXPECT_EQ((u32)move->additionalEffects[1].spAtk, 2);
    EXPECT_EQ((u32)move->additionalEffects[1].speed, 2);
    EXPECT_EQ(StringCompare(GetMoveDescription(MOVE_SHELL_SMASH), expected), 0);
}

TEST("Descriptions: Quiver Dance includes the implemented speed boost")
{
    static const u8 expected[] = _("{JPN}自分の特攻 特防 素早さを\n1段階ずつ 上げる");
    const struct AdditionalEffect *effect = gMovesInfo[MOVE_QUIVER_DANCE].additionalEffects;
    EXPECT_EQ((u32)effect->spAtk, 1);
    EXPECT_EQ((u32)effect->spDef, 1);
    EXPECT_EQ((u32)effect->speed, 1);
    EXPECT_EQ(StringCompare(GetMoveDescription(MOVE_QUIVER_DANCE), expected), 0);
}

TEST("Descriptions: Tera Blast and Grassy Glide explain their conditional effects")
{
    static const u8 tera[] = _("{JPN}テラ中は タイプが変化\n攻撃と特攻の 高い方で攻撃");
    static const u8 grassy[] = _("{JPN}グラスフィールド中は\n地面にいると 優先度{ENG}+{JPN}1");
    EXPECT_EQ(GetMoveEffect(MOVE_TERA_BLAST), EFFECT_TERA_BLAST);
    EXPECT_EQ(GetMoveEffect(MOVE_GRASSY_GLIDE), EFFECT_GRASSY_GLIDE);
    EXPECT_EQ(StringCompare(GetMoveDescription(MOVE_TERA_BLAST), tera), 0);
    EXPECT_EQ(StringCompare(GetMoveDescription(MOVE_GRASSY_GLIDE), grassy), 0);
}
