#include "global.h"
#include "test/battle.h"
#include "battle_anim.h"
#include "battle_gfx_sfx_util.h"
#include "battle_interface.h"
#include "sprite.h"

static void ExpectBattlersAndHealthboxesVisible(void)
{
    for (u32 battler = 0; battler < gBattlersCount; battler++)
    {
        u32 mon = gBattlerSpriteIds[battler];
        u32 box = gHealthboxSpriteIds[battler];
        EXPECT_LT(mon, MAX_SPRITES);
        EXPECT_LT(box, MAX_SPRITES);
        EXPECT(gSprites[mon].inUse);
        EXPECT(!gSprites[mon].invisible);
        EXPECT(gSprites[mon].callback != SpriteCB_SetInvisible);
        EXPECT_EQ(gSprites[mon].x2, 0);
        EXPECT_EQ(gSprites[mon].y2, 0);
        EXPECT(gSprites[box].inUse);
        EXPECT(!gSprites[box].invisible);
        EXPECT(gSprites[box].callback != SpriteCB_SetInvisible);
        EXPECT(!gSprites[gSprites[box].oam.affineParam].invisible);
        EXPECT(!gSprites[gSprites[box].data[5]].invisible);
    }
}

DOUBLE_BATTLE_TEST("Gimmick visuals: Tera restores all four battlers and healthboxes in doubles")
{
    u32 slot = 0;
    for (u32 i = 0; i < MAX_BATTLERS_COUNT; i++)
        PARAMETRIZE { slot = i; }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_PIKACHU) { TeraType(TYPE_GRASS); }
        PLAYER(SPECIES_EEVEE) { TeraType(TYPE_WATER); }
        OPPONENT(SPECIES_BULBASAUR) { TeraType(TYPE_FIRE); }
        OPPONENT(SPECIES_CHARMANDER) { TeraType(TYPE_ICE); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE, gimmick: slot == 0 ? GIMMICK_TERA : GIMMICK_NONE);
            MOVE(opponentLeft, MOVE_CELEBRATE, gimmick: slot == 1 ? GIMMICK_TERA : GIMMICK_NONE);
            MOVE(playerRight, MOVE_CELEBRATE, gimmick: slot == 2 ? GIMMICK_TERA : GIMMICK_NONE);
            MOVE(opponentRight, MOVE_CELEBRATE, gimmick: slot == 3 ? GIMMICK_TERA : GIMMICK_NONE);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectBattlersAndHealthboxesVisible();
    }
}

DOUBLE_BATTLE_TEST("Gimmick visuals: Dynamax restores all four battlers and healthboxes in doubles")
{
    u32 slot = 0;
    for (u32 i = 0; i < MAX_BATTLERS_COUNT; i++)
        PARAMETRIZE { slot = i; }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_PIKACHU);
        PLAYER(SPECIES_EEVEE);
        OPPONENT(SPECIES_BULBASAUR);
        OPPONENT(SPECIES_CHARMANDER);
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE, gimmick: slot == 0 ? GIMMICK_DYNAMAX : GIMMICK_NONE);
            MOVE(opponentLeft, MOVE_CELEBRATE, gimmick: slot == 1 ? GIMMICK_DYNAMAX : GIMMICK_NONE);
            MOVE(playerRight, MOVE_CELEBRATE, gimmick: slot == 2 ? GIMMICK_DYNAMAX : GIMMICK_NONE);
            MOVE(opponentRight, MOVE_CELEBRATE, gimmick: slot == 3 ? GIMMICK_DYNAMAX : GIMMICK_NONE);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectBattlersAndHealthboxesVisible();
    }
}

SINGLE_BATTLE_TEST("Gimmick visuals: Tera does not overwrite unrelated sprite callbacks in singles")
{
    u32 side = B_SIDE_PLAYER;
    PARAMETRIZE { side = B_SIDE_PLAYER; }
    PARAMETRIZE { side = B_SIDE_OPPONENT; }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_PIKACHU) { TeraType(TYPE_GRASS); }
        OPPONENT(SPECIES_EEVEE) { TeraType(TYPE_WATER); }
    } WHEN {
        TURN {
            MOVE(player, MOVE_CELEBRATE, gimmick: side == B_SIDE_PLAYER ? GIMMICK_TERA : GIMMICK_NONE);
            MOVE(opponent, MOVE_CELEBRATE, gimmick: side == B_SIDE_OPPONENT ? GIMMICK_TERA : GIMMICK_NONE);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectBattlersAndHealthboxesVisible();
    }
}

DOUBLE_BATTLE_TEST("Gimmick visuals: Tera and Dynamax in the same turn leave every battler visible after Dynamax ends")
{
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_PIKACHU) { TeraType(TYPE_GRASS); }
        PLAYER(SPECIES_EEVEE);
        OPPONENT(SPECIES_BULBASAUR);
        OPPONENT(SPECIES_CHARMANDER);
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE, gimmick: GIMMICK_TERA);
            MOVE(opponentRight, MOVE_CELEBRATE, gimmick: GIMMICK_DYNAMAX);
        }
        TURN { MOVE(opponentRight, MOVE_CELEBRATE); }
        TURN { MOVE(opponentRight, MOVE_CELEBRATE); }
        TURN { MOVE(opponentRight, MOVE_CELEBRATE); }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectBattlersAndHealthboxesVisible();
    }
}
