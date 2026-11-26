#include "global.h"
#include "test/battle.h"

// M4-B1c-1b (gba-gc-extension docs/shell-bell.md): the GC's two after-move
// conditions on one hit. A hit that lands but takes no HP (the foe's Focus
// Sash at 1 max HP leaves it its 1 HP) is a hit for Life Orb
// (IsAnyTargetTurnDamaged: damagedByAttack is set by TryMoveDamageUpdate
// whatever the amount; the GC's MoveContext.landed) and nothing for Shell
// Bell (savedDmg 0; the GC's MoveContext.lost). No MESSAGE checks (the text
// is Japanese).

#define ITEM_ANIM(battler) ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, battler)

ASSUMPTIONS
{
    ASSUME(gItemsInfo[ITEM_SHELL_BELL].holdEffect == HOLD_EFFECT_SHELL_BELL);
    ASSUME(gItemsInfo[ITEM_LIFE_ORB].holdEffect == HOLD_EFFECT_LIFE_ORB);
    ASSUME(gItemsInfo[ITEM_FOCUS_SASH].holdEffect == HOLD_EFFECT_FOCUS_SASH);
}

SINGLE_BATTLE_TEST("GC V21 reference: a hit that takes no HP (Focus Sash at 1 max HP) gives Shell Bell nothing")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(1); HP(1); Item(ITEM_FOCUS_SASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HANGED_ON, opponent);  // the sash holds on
        NONE_OF {
            ITEM_ANIM(player);
            HP_BAR(player);
        }
    } THEN {
        Test_MgbaPrintf("V21_ZERO bell holder=%d foe=%d sash=%d", player->hp, opponent->hp, opponent->item);
        EXPECT_EQ(opponent->hp, 1);
        EXPECT_EQ(opponent->item, ITEM_NONE);  // the sash was used
        EXPECT_EQ(player->hp, 100);            // no heal
    }
}

SINGLE_BATTLE_TEST("GC V21 reference: the same hit that takes no HP still costs Life Orb its HP")
{
    static s16 loss;
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(400); Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(1); HP(1); Item(ITEM_FOCUS_SASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HANGED_ON, opponent);
        HP_BAR(player, captureDamage: &loss);
    } THEN {
        Test_MgbaPrintf("V21_ZERO orb holder=%d foe=%d loss=%d", player->hp, opponent->hp, loss);
        EXPECT_EQ(opponent->hp, 1);
        EXPECT_EQ(loss, 40);
        EXPECT_EQ(player->hp, 360);
    }
}
