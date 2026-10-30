#include "global.h"
#include "test/battle.h"
#include "battle_setup.h"
#include "trainer_slide.h"

// The trainer slide check runs every frame of a Z-Move turn; it must not move the
// battler that a running script (here, Illusion breaking) is working on.
AI_SINGLE_BATTLE_TEST("Illusion: a trainer's Z-Move breaks the target's Illusion, not the attacker's")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_ZOROARK) { Ability(ABILITY_ILLUSION); Moves(MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Item(ITEM_NORMALIUM_Z); Moves(MOVE_QUICK_ATTACK); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_QUICK_ATTACK, gimmick: GIMMICK_Z_MOVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ILLUSION_OFF, player);
        NOT ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ILLUSION_OFF, opponent);
    } THEN {
        EXPECT_EQ(gBattleStruct->illusion[B_BATTLER_0].state, ILLUSION_OFF);
    }
}

AI_SINGLE_BATTLE_TEST("Illusion: checking for a trainer slide that does not play keeps the script battler")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } THEN {
        gBattleScripting.battler = B_BATTLER_0;
        EXPECT_EQ(ShouldDoTrainerSlide(B_BATTLER_1, TRAINER_SLIDE_OPPONENT_LAST_SWITCHIN), TRAINER_SLIDE_TARGET_NONE);
        EXPECT_EQ(ShouldDoTrainerSlide(B_BATTLER_1, TRAINER_SLIDE_ATTACKER_Z_MOVE), TRAINER_SLIDE_TARGET_NONE);
        EXPECT_EQ(gBattleScripting.battler, B_BATTLER_0);
    }
}
