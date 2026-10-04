#include "global.h"
#include "test/battle.h"

#define USE_ARENA_RULES() (gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_ARENA)

SINGLE_BATTLE_TEST("Battle Arena: damaging phasing moves cannot force a switch")
{
    enum Move move;
    bool32 opponentUsesMove;

    PARAMETRIZE { move = MOVE_DRAGON_TAIL; opponentUsesMove = FALSE; }
    PARAMETRIZE { move = MOVE_CIRCLE_THROW; opponentUsesMove = FALSE; }
    PARAMETRIZE { move = MOVE_DRAGON_TAIL; opponentUsesMove = TRUE; }
    PARAMETRIZE { move = MOVE_CIRCLE_THROW; opponentUsesMove = TRUE; }

    GIVEN {
        USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(400); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(400); HP(400); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {
            if (opponentUsesMove)
                MOVE(opponent, move);
            else
                MOVE(player, move);
        }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 0);
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_1], 0);
        if (opponentUsesMove)
            EXPECT_LT(player->hp, player->maxHP);
        else
            EXPECT_LT(opponent->hp, opponent->maxHP);
    }
}

SINGLE_BATTLE_TEST("Battle Arena: damaging pivot moves keep the attacker in battle")
{
    enum Move move;
    bool32 opponentUsesMove;

    PARAMETRIZE { move = MOVE_U_TURN; opponentUsesMove = FALSE; }
    PARAMETRIZE { move = MOVE_VOLT_SWITCH; opponentUsesMove = FALSE; }
    PARAMETRIZE { move = MOVE_FLIP_TURN; opponentUsesMove = FALSE; }
    PARAMETRIZE { move = MOVE_U_TURN; opponentUsesMove = TRUE; }
    PARAMETRIZE { move = MOVE_VOLT_SWITCH; opponentUsesMove = TRUE; }
    PARAMETRIZE { move = MOVE_FLIP_TURN; opponentUsesMove = TRUE; }

    GIVEN {
        USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(400); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(400); HP(400); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {
            if (opponentUsesMove)
                MOVE(opponent, move);
            else
                MOVE(player, move);
        }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 0);
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_1], 0);
        if (opponentUsesMove)
            EXPECT_LT(player->hp, player->maxHP);
        else
            EXPECT_LT(opponent->hp, opponent->maxHP);
    }
}

SINGLE_BATTLE_TEST("Battle Arena: status switching moves cannot switch either Pokemon")
{
    enum Move move;

    PARAMETRIZE { move = MOVE_BATON_PASS; }
    PARAMETRIZE { move = MOVE_ROAR; }
    PARAMETRIZE { move = MOVE_WHIRLWIND; }
    PARAMETRIZE { move = MOVE_TELEPORT; }
    PARAMETRIZE { move = MOVE_SHED_TAIL; }
    PARAMETRIZE { move = MOVE_PARTING_SHOT; }
    PARAMETRIZE { move = MOVE_CHILLY_RECEPTION; }

    GIVEN {
        USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, move); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 0);
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_1], 0);
    }
}

SINGLE_BATTLE_TEST("Battle Arena: Red Card cannot force the attacker out")
{
    GIVEN {
        USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_RED_CARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 0);
        EXPECT_EQ(opponent->item, ITEM_RED_CARD);
    }
}

SINGLE_BATTLE_TEST("Battle Arena: Eject Button cannot switch its holder out")
{
    GIVEN {
        USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_BUTTON); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 0);
        EXPECT_EQ(player->item, ITEM_EJECT_BUTTON);
    }
}

SINGLE_BATTLE_TEST("Battle Arena: Eject Pack cannot switch its holder out")
{
    GIVEN {
        USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_EJECT_PACK); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCREECH); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 0);
        EXPECT_EQ(player->item, ITEM_EJECT_PACK);
    }
}

SINGLE_BATTLE_TEST("Battle Arena: a fainted Pokemon still gets replaced")
{
    GIVEN {
        USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); SEND_OUT(player, 1); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 1);
    }
}

SINGLE_BATTLE_TEST("Battle Arena: a fainted opponent still gets replaced")
{
    GIVEN {
        USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(opponent, 1); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_1], 1);
    }
}

SINGLE_BATTLE_TEST("Battle Arena guard does not affect ordinary U-turn switching")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_U_TURN); SEND_OUT(player, 1); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 1);
    }
}

SINGLE_BATTLE_TEST("Battle Arena guard does not affect ordinary Red Card switching")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_RED_CARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 1);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

#undef USE_ARENA_RULES
