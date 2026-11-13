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
        ITEM_POPUP(player, ITEM_CUSTAP_BERRY);
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
        ITEM_POPUP(player, ITEM_CUSTAP_BERRY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, player);
        MESSAGE("{JPN}{JPN}レジロックは {JPN}イバンのみの おかげで\nいつもより 早く 動ける！");
    }
}

SINGLE_BATTLE_TEST("Custap Berry: Forretress displays its own item before exploding against Darkrai")
{
    GIVEN {
        PLAYER(SPECIES_FORRETRESS) { Speed(1); MaxHP(182); HP(5); Item(ITEM_CUSTAP_BERRY); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_DARKRAI) { Speed(200); MaxHP(1000); HP(1000); Item(ITEM_LEFTOVERS); }
    } WHEN {
        TURN { MOVE(player, MOVE_EXPLOSION); SEND_OUT(player, 1); }
    } SCENE {
        ITEM_POPUP(player, ITEM_CUSTAP_BERRY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, player);
        MESSAGE("{JPN}{JPN}フォレトスは {JPN}イバンのみの おかげで\nいつもより 早く 動ける！");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EXPLOSION, player);
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM), ITEM_NONE);
        EXPECT_EQ(opponent->item, ITEM_LEFTOVERS);
    }
}

DOUBLE_BATTLE_TEST("Custap Berry: the popup follows its holder in all four positions")
{
    u32 position = 0;
    for (u32 slot = 0; slot < MAX_BATTLERS_COUNT; slot++)
        PARAMETRIZE { position = slot; }
    struct BattlePokemon *holder = &gBattleMons[position];
    GIVEN {
        PLAYER(SPECIES_FORRETRESS) { Speed(100); MaxHP(160); HP(40); Item(position == 0 ? ITEM_CUSTAP_BERRY : ITEM_NONE); }
        PLAYER(SPECIES_REGIROCK) { Speed(200); MaxHP(160); HP(40); Item(position == 2 ? ITEM_CUSTAP_BERRY : ITEM_NONE); }
        OPPONENT(SPECIES_DARKRAI) { Speed(300); MaxHP(160); HP(40); Item(position == 1 ? ITEM_CUSTAP_BERRY : ITEM_NONE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(400); MaxHP(160); HP(40); Item(position == 3 ? ITEM_CUSTAP_BERRY : ITEM_NONE); }
    } WHEN {
        TURN { MOVE(holder, MOVE_CELEBRATE); }
    } SCENE {
        ITEM_POPUP(holder, ITEM_CUSTAP_BERRY);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, holder);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, holder);
    } THEN {
        EXPECT_EQ(holder->item, ITEM_NONE);
    }
}
