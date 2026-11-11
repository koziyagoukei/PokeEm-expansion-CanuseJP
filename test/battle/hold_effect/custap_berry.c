#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gItemsInfo[ITEM_CUSTAP_BERRY].holdEffect == HOLD_EFFECT_CUSTAP_BERRY);
}

SINGLE_BATTLE_TEST("Custap Berry allows the holder to move first in its priority bracket when HP is below 1/4")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); MaxHP(160); HP(40); Item(ITEM_CUSTAP_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ITEM_POPUP(player, ITEM_CUSTAP_BERRY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, player);
        MESSAGE("{JPN}{JPN}ソーナンスは {JPN}イバンのみの おかげで\nいつもより 早く 動ける！");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    }
}

SINGLE_BATTLE_TEST("Custap Berry shows the popup on the opposing holder, not the next battler")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_REGIROCK) { Speed(1); MaxHP(160); HP(40); Item(ITEM_CUSTAP_BERRY); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ITEM_POPUP(opponent, ITEM_CUSTAP_BERRY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, opponent);
        MESSAGE("{JPN}{JPN}相手の {JPN}レジロックは {JPN}イバンのみの おかげで\nいつもより 早く 動ける！");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
    }
}

SINGLE_BATTLE_TEST("Custap Berry allows the holder to move first in its priority bracket when HP is below 1/2. If the holder has Gluttony")
{
    GIVEN {
        PLAYER(SPECIES_BELLSPROUT) { Speed(1); MaxHP(160); HP(80); Ability(ABILITY_GLUTTONY); Item(ITEM_CUSTAP_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, player);
        MESSAGE("{JPN}{JPN}マダツボミは {JPN}イバンのみの おかげで\nいつもより 早く 動ける！");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    }
}

SINGLE_BATTLE_TEST("Custap Berry activates even if the opposing mon switches out")
{
    GIVEN {
        PLAYER(SPECIES_REGIROCK) { HP(1); Item(ITEM_CUSTAP_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, player);
        MESSAGE("{JPN}{JPN}レジロックは {JPN}イバンのみの おかげで\nいつもより 早く 動ける！");
    }
}
