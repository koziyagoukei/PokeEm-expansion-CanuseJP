#include "global.h"
#include "test/battle.h"
#include "battle_ai_util.h"

// Reported from the Battle Factory: the AI used Soak on a Terastallized Pokemon, twice
AI_SINGLE_BATTLE_TEST("AI does not use Soak on a Terastallized target")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SOAK) == EFFECT_SOAK);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SOAK, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_TERA); }
        TURN { MOVE(player, MOVE_CELEBRATE); SCORE_LT_VAL(opponent, MOVE_SOAK, AI_SCORE_DEFAULT); }
    }
}

// Reported from the Battle Factory: Ground moves into Levitate (the AI there had no move scoring before)
AI_SINGLE_BATTLE_TEST("AI does not use a Ground move on a Levitate Pokemon")
{
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_UXIE) { Ability(ABILITY_LEVITATE); }
        OPPONENT(SPECIES_DONPHAN) { Moves(MOVE_EARTHQUAKE, MOVE_TACKLE); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_TACKLE); }
        TURN { EXPECT_MOVE(opponent, MOVE_TACKLE); }
    }
}
