#include "global.h"
#include "test/battle.h"
#include "battle_gimmick.h"
#include "battle_gimmick_extra.h"
#include "battle_z_move.h"

SINGLE_BATTLE_TEST("Ultra Z: Ultra Burst unlocks its Z move without unlocking other gimmicks")
{
    enum Species species;
    PARAMETRIZE { species = SPECIES_NECROZMA_DUSK_MANE; }
    PARAMETRIZE { species = SPECIES_NECROZMA_DAWN_WINGS; }
    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(species) { Item(ITEM_ULTRANECROZIUM_Z); Moves(MOVE_CELEBRATE, MOVE_PHOTON_GEYSER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_ULTRA_BURST); }
    } THEN {
        EXPECT_EQ(player->species, SPECIES_NECROZMA_ULTRA);
        EXPECT(HasBattlerUsedAnyGimmick(B_BATTLER_0));
        EXPECT(CanUseZMove(B_BATTLER_0));
        EXPECT_EQ(GetUsableZMove(B_BATTLER_0, MOVE_PHOTON_GEYSER), MOVE_LIGHT_THAT_BURNS_THE_SKY);
        EXPECT_EQ(GetUsableGimmickMask(B_BATTLER_0), 1u << GIMMICK_Z_MOVE);
        AssignUsableGimmicks();
        EXPECT_EQ(gBattleStruct->gimmick.usableGimmick[B_BATTLER_0], GIMMICK_Z_MOVE);
        EXPECT(IsViableZMove(B_BATTLER_0, MOVE_PHOTON_GEYSER));
    }
}

SINGLE_BATTLE_TEST("Ultra Z: either Necrozma fusion can Ultra Burst then use its Z move once")
{
    enum Species species;
    bool32 useOpponent;
    for (u32 side = 0; side < 2; side++)
    {
        PARAMETRIZE { species = SPECIES_NECROZMA_DUSK_MANE; useOpponent = side; }
        PARAMETRIZE { species = SPECIES_NECROZMA_DAWN_WINGS; useOpponent = side; }
    }
    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(useOpponent ? SPECIES_WOBBUFFET : species) {
            Item(useOpponent ? ITEM_NONE : ITEM_ULTRANECROZIUM_Z); HP(9999); MaxHP(9999);
        }
        OPPONENT(useOpponent ? species : SPECIES_WOBBUFFET) {
            Item(useOpponent ? ITEM_ULTRANECROZIUM_Z : ITEM_NONE); HP(9999); MaxHP(9999);
        }
    } WHEN {
        TURN { MOVE(useOpponent ? opponent : player, MOVE_CELEBRATE, gimmick: GIMMICK_ULTRA_BURST); }
        TURN { MOVE(useOpponent ? opponent : player, MOVE_PHOTON_GEYSER, gimmick: GIMMICK_Z_MOVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ULTRA_BURST, useOpponent ? opponent : player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ZMOVE_ACTIVATE, useOpponent ? opponent : player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LIGHT_THAT_BURNS_THE_SKY, useOpponent ? opponent : player);
    } THEN {
        enum BattlerId battler = useOpponent ? B_BATTLER_1 : B_BATTLER_0;
        EXPECT(HasTrainerUsedGimmick(battler, GIMMICK_Z_MOVE));
        EXPECT(!CanUseZMove(battler));
    }
}

SINGLE_BATTLE_TEST("Ultra Z: switching out after Ultra Burst preserves the unused Z move")
{
    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_NECROZMA_DUSK_MANE) { Item(ITEM_ULTRANECROZIUM_Z); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { HP(9999); MaxHP(9999); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_ULTRA_BURST); }
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
        TURN { MOVE(player, MOVE_PHOTON_GEYSER, gimmick: GIMMICK_Z_MOVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ULTRA_BURST, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ZMOVE_ACTIVATE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LIGHT_THAT_BURNS_THE_SKY, player);
    } THEN {
        EXPECT(!CanUseZMove(B_BATTLER_0));
    }
}

DOUBLE_BATTLE_TEST("Ultra Z: a teammate's used Z move still blocks Ultra Necrozma's Z move")
{
    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_NORMALIUM_Z); }
        PLAYER(SPECIES_NECROZMA_DAWN_WINGS) { Item(ITEM_ULTRANECROZIUM_Z); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(9999); MaxHP(9999); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(9999); MaxHP(9999); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE, gimmick: GIMMICK_ULTRA_BURST);
        }
        TURN {
            MOVE(playerLeft, MOVE_TACKLE, gimmick: GIMMICK_Z_MOVE, target: opponentLeft);
            MOVE(playerRight, MOVE_CELEBRATE);
        }
    } THEN {
        EXPECT_EQ(playerRight->species, SPECIES_NECROZMA_ULTRA);
        EXPECT(HasTrainerUsedGimmick(B_BATTLER_2, GIMMICK_Z_MOVE));
        EXPECT(!CanUseZMove(B_BATTLER_2));
    }
}
