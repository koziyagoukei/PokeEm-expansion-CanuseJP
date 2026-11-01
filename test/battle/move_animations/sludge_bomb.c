#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Sludge Bomb animation: opponent can use it repeatedly without leaking sprites")
{
    FORCE_MOVE_ANIM(TRUE);
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { HP(9999); MaxHP(9999); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_MUK) { Moves(MOVE_SLUDGE_BOMB); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_SLUDGE_BOMB); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_SLUDGE_BOMB); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_SLUDGE_BOMB); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLUDGE_BOMB, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLUDGE_BOMB, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLUDGE_BOMB, opponent);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        EXPECT_EQ(gLoadFail, FALSE);
        EXPECT_EQ(gSpriteAllocs, 0);
    }
}
