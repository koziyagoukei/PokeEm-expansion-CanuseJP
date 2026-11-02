#include "global.h"
#include "test/battle.h"
#include "battle_anim.h"
#include "battle_gimmick.h"
#include "sprite.h"

SINGLE_BATTLE_TEST("Gimmick faint: only active Dynamax and Tera play the faint effect on either side")
{
    enum Gimmick gimmick = GIMMICK_NONE;
    struct BattlePokemon *fainted = NULL;
    struct BattlePokemon *attacker = NULL;
    for (u32 side = 0; side < 2; side++)
    {
        PARAMETRIZE { gimmick = GIMMICK_NONE; fainted = side ? opponent : player; attacker = side ? player : opponent; }
        PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; fainted = side ? opponent : player; attacker = side ? player : opponent; }
        PARAMETRIZE { gimmick = GIMMICK_TERA; fainted = side ? opponent : player; attacker = side ? player : opponent; }
    }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_WOBBUFFET) { HP(fainted == player ? 1 : 300); if (gimmick == GIMMICK_TERA) TeraType(TYPE_NORMAL); Speed(fainted == player ? 2 : 1); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(fainted == opponent ? 1 : 300); if (gimmick == GIMMICK_TERA) TeraType(TYPE_NORMAL); Speed(fainted == opponent ? 2 : 1); }
    } WHEN {
        TURN { MOVE(fainted, MOVE_SCRATCH, gimmick: gimmick); MOVE(attacker, MOVE_DRAGON_RAGE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, attacker);
        HP_BAR(fainted);
        if (gimmick == GIMMICK_DYNAMAX) {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_FAINT, fainted);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, fainted);
        } else if (gimmick == GIMMICK_TERA) {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_FAINT, fainted);
        } else {
            NONE_OF {
                ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_FAINT);
                ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_FAINT);
            }
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        EXPECT_EQ(fainted->hp, 0);
        EXPECT_EQ(GetActiveGimmick(fainted == player ? B_BATTLER_0 : B_BATTLER_1), gimmick == GIMMICK_DYNAMAX ? GIMMICK_NONE : gimmick);
        EXPECT_EQ(gAnimVisualTaskCount, 0);
        EXPECT_EQ(gAnimSoundTaskCount, 0);
    }
}

DOUBLE_BATTLE_TEST("Gimmick faint: effects work in all four positions without hiding surviving battlers")
{
    enum Gimmick gimmick = GIMMICK_NONE;
    struct BattlePokemon *fainted = NULL;
    struct BattlePokemon *attacker = NULL;
    for (u32 slot = 0; slot < MAX_BATTLERS_COUNT; slot++)
    {
        PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; fainted = &gBattleMons[slot]; attacker = slot & 1 ? playerLeft : opponentLeft; }
        PARAMETRIZE { gimmick = GIMMICK_TERA; fainted = &gBattleMons[slot]; attacker = slot & 1 ? playerLeft : opponentLeft; }
    }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_WOBBUFFET) { HP(fainted == playerLeft ? 1 : 300); if (gimmick == GIMMICK_TERA) TeraType(TYPE_NORMAL); }
        PLAYER(SPECIES_WOBBUFFET) { HP(fainted == playerRight ? 1 : 300); if (gimmick == GIMMICK_TERA) TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(fainted == opponentLeft ? 1 : 300); if (gimmick == GIMMICK_TERA) TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(fainted == opponentRight ? 1 : 300); if (gimmick == GIMMICK_TERA) TeraType(TYPE_NORMAL); }
    } WHEN {
        TURN { MOVE(fainted, MOVE_SCRATCH, gimmick: gimmick, target: attacker); MOVE(attacker, MOVE_DRAGON_RAGE, target: fainted); }
    } SCENE {
        HP_BAR(fainted);
        ANIMATION(ANIM_TYPE_GENERAL, gimmick == GIMMICK_DYNAMAX ? B_ANIM_DYNAMAX_FAINT : B_ANIM_TERA_FAINT, fainted);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        EXPECT_EQ(fainted->hp, 0);
        for (u32 battler = 0; battler < gBattlersCount; battler++)
        {
            if (&gBattleMons[battler] != fainted)
            {
                u32 sprite = gBattlerSpriteIds[battler];
                EXPECT_LT(sprite, MAX_SPRITES);
                EXPECT(gSprites[sprite].inUse);
                EXPECT(!gSprites[sprite].invisible);
            }
        }
        EXPECT_EQ(gAnimVisualTaskCount, 0);
        EXPECT_EQ(gAnimSoundTaskCount, 0);
    }
}

SINGLE_BATTLE_TEST("Gimmick faint: an expired Dynamax does not play a second burst when fainting")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_LEER, gimmick: GIMMICK_DYNAMAX); }
        TURN { MOVE(player, MOVE_LEER); }
        TURN { MOVE(player, MOVE_LEER); }
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_FORM_CHANGE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_FAINT);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_FAINT);
        }
    }
}

SINGLE_BATTLE_TEST("Gimmick faint: dedicated crystal dust supports every Tera type")
{
    enum Type type = TYPE_NORMAL;
    for (u32 t = TYPE_NORMAL; t <= TYPE_STELLAR; t++)
    {
        if (t != TYPE_MYSTERY && t != TYPE_NONE)
            PARAMETRIZE { type = t; }
    }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_WOBBUFFET) { HP(1); TeraType(type); Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_TERA); MOVE(opponent, MOVE_AERIAL_ACE); }
    } SCENE {
        HP_BAR(player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_FAINT, player);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        EXPECT_EQ(player->hp, 0);
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_0), GIMMICK_TERA);
        EXPECT_EQ(gAnimVisualTaskCount, 0);
        EXPECT_EQ(gAnimSoundTaskCount, 0);
    }
}
