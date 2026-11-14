#include "global.h"
#include "battle_z_move.h"
#include "move.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"

TEST("Z-Move text: every stat and stage uses the correct name and Latin plus sign")
{
    static const struct {
        enum ZEffect effect;
        const u8 *text;
    } cases[] = {
        {Z_EFFECT_ATK_UP_1, COMPOUND_STRING("{JPN}攻撃{ENG} +1")},
        {Z_EFFECT_ATK_UP_2, COMPOUND_STRING("{JPN}攻撃{ENG} +2")},
        {Z_EFFECT_ATK_UP_3, COMPOUND_STRING("{JPN}攻撃{ENG} +3")},
        {Z_EFFECT_DEF_UP_1, COMPOUND_STRING("{JPN}防御{ENG} +1")},
        {Z_EFFECT_DEF_UP_2, COMPOUND_STRING("{JPN}防御{ENG} +2")},
        {Z_EFFECT_DEF_UP_3, COMPOUND_STRING("{JPN}防御{ENG} +3")},
        {Z_EFFECT_SPD_UP_1, COMPOUND_STRING("{JPN}素早さ{ENG} +1")},
        {Z_EFFECT_SPD_UP_2, COMPOUND_STRING("{JPN}素早さ{ENG} +2")},
        {Z_EFFECT_SPD_UP_3, COMPOUND_STRING("{JPN}素早さ{ENG} +3")},
        {Z_EFFECT_SPATK_UP_1, COMPOUND_STRING("{JPN}特攻{ENG} +1")},
        {Z_EFFECT_SPATK_UP_2, COMPOUND_STRING("{JPN}特攻{ENG} +2")},
        {Z_EFFECT_SPATK_UP_3, COMPOUND_STRING("{JPN}特攻{ENG} +3")},
        {Z_EFFECT_SPDEF_UP_1, COMPOUND_STRING("{JPN}特防{ENG} +1")},
        {Z_EFFECT_SPDEF_UP_2, COMPOUND_STRING("{JPN}特防{ENG} +2")},
        {Z_EFFECT_SPDEF_UP_3, COMPOUND_STRING("{JPN}特防{ENG} +3")},
        {Z_EFFECT_ACC_UP_1, COMPOUND_STRING("{JPN}命中率{ENG} +1")},
        {Z_EFFECT_ACC_UP_2, COMPOUND_STRING("{JPN}命中率{ENG} +2")},
        {Z_EFFECT_ACC_UP_3, COMPOUND_STRING("{JPN}命中率{ENG} +3")},
        {Z_EFFECT_EVSN_UP_1, COMPOUND_STRING("{JPN}回避率{ENG} +1")},
        {Z_EFFECT_EVSN_UP_2, COMPOUND_STRING("{JPN}回避率{ENG} +2")},
        {Z_EFFECT_EVSN_UP_3, COMPOUND_STRING("{JPN}回避率{ENG} +3")},
    };
    u32 index = 0;
    for (u32 j = 0; j < ARRAY_COUNT(cases); j++)
        PARAMETRIZE { index = j; }
    u8 text[64];
    BufferZMoveEffectDescription(text, cases[index].effect);
    EXPECT_EQ(StringCompare(text, cases[index].text), 0);
    EXPECT_LE(GetStringWidth(FONT_NORMAL, text, 0), 96);
}

TEST("Z-Move text: Work Up shows its extra Attack boost")
{
    u8 text[64];
    BufferZMoveEffectDescription(text, GetMoveZEffect(MOVE_WORK_UP));
    EXPECT_EQ(StringCompare(text, COMPOUND_STRING("{JPN}攻撃{ENG} +1")), 0);
}

TEST("Z-Move text: Happy Hour and Conversion show all stats rising by one stage")
{
    enum Move move;
    PARAMETRIZE { move = MOVE_HAPPY_HOUR; }
    PARAMETRIZE { move = MOVE_CONVERSION; }
    u8 text[64];
    BufferZMoveEffectDescription(text, GetMoveZEffect(move));
    EXPECT_EQ(StringCompare(text, COMPOUND_STRING("{JPN}すべて {ENG}+1")), 0);
    EXPECT_LE(GetStringWidth(FONT_NORMAL, text, 0), 96);
}
