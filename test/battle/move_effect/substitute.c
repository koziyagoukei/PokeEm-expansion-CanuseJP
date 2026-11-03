#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_SUBSTITUTE) == EFFECT_SUBSTITUTE);
}

SINGLE_BATTLE_TEST("Substitute damage message: an attack from behind a substitute hits the actual target")
{
    GIVEN {
        PLAYER(SPECIES_SINISTCHA) { MaxHP(400); HP(400); }
        OPPONENT(SPECIES_BRELOOM) { MaxHP(400); HP(400); }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SUBSTITUTE); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SEED_BOMB); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SEED_BOMB, opponent);
        NOT MESSAGE("{JPN}{JPN}ヤバソチャに かわって 分身が 攻撃を うけた!");
        HP_BAR(player);
    } THEN {
        EXPECT_LT(player->hp, player->maxHP);
        EXPECT_EQ((u32)opponent->volatiles.substituteHP, 100);
    }
}

DOUBLE_BATTLE_TEST("Substitute damage message: a spread move names the actual substitute owner")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_SWIFT) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(400); HP(400); }
        OPPONENT(SPECIES_WYNAUT) { MaxHP(400); HP(400); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SPLASH);
            MOVE(playerRight, MOVE_SPLASH);
            MOVE(opponentLeft, MOVE_SPLASH);
            MOVE(opponentRight, MOVE_SUBSTITUTE);
        }
        TURN {
            MOVE(playerLeft, MOVE_SWIFT);
            MOVE(playerRight, MOVE_SPLASH);
            MOVE(opponentLeft, MOVE_SPLASH);
            MOVE(opponentRight, MOVE_SPLASH);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SWIFT, playerLeft);
        MESSAGE("{JPN}{JPN}相手の {JPN}ソーナノに かわって 分身が 攻撃を うけた!");
        SUB_HIT(opponentRight);
    } THEN {
        EXPECT_LT(opponentLeft->hp, 400);
        EXPECT_EQ(opponentRight->hp, 300);
        EXPECT_LT((u32)opponentRight->volatiles.substituteHP, 100);
    }
}

SINGLE_BATTLE_TEST("Substitute creates a Substitute at the cost of 1/4 users maximum HP")
{
    s16 maxHP = 0;
    s16 costHP = 0;

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); }
    } SCENE {
        maxHP = GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, player);
        HP_BAR(player, captureDamage: &costHP);
        MESSAGE("Wobbuffet put in a substitute!");
    }THEN {
        EXPECT_EQ(maxHP / 4, costHP);
    }
}

SINGLE_BATTLE_TEST("Substitute fails if the user doesn't have enough HP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); }
    } SCENE {
        MESSAGE("But it does not have enough HP left to make a substitute!");
    }
}

SINGLE_BATTLE_TEST("Substitute's HP cost can trigger a berry")
{
    GIVEN {
        ASSUME(gItemsInfo[ITEM_SITRUS_BERRY].battleUsage == EFFECT_ITEM_RESTORE_HP);
        PLAYER(SPECIES_WOBBUFFET) { HP(300); Item(ITEM_SITRUS_BERRY); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, player);
    }
}

SINGLE_BATTLE_TEST("Substitute's HP cost doesn't trigger effects that trigger on damage taken")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_AIR_BALLOON); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, player);
        MESSAGE("Wobbuffet put in a substitute!");
        NOT MESSAGE("Wobbuffet's Air Balloon popped!");
    }
}

SINGLE_BATTLE_TEST("Baton Pass passes Substitutes")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_BATON_PASS) == EFFECT_BATON_PASS);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(400); }
        PLAYER(SPECIES_WYNAUT) { MaxHP(200); HP(200); Defense(999); }
        OPPONENT(SPECIES_WOBBUFFET) { Attack(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); }
        TURN { MOVE(player, MOVE_BATON_PASS); SEND_OUT(player, 1); }
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BATON_PASS, player);
        SEND_IN_MESSAGE("Wynaut");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        SUB_HIT(player);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_WYNAUT);
        EXPECT_EQ(player->hp, player->maxHP);
        EXPECT(player->volatiles.substitute);
        EXPECT_EQ((u32)player->volatiles.substituteHP, 99);
    }
}
