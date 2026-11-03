#include "global.h"
#include "battle_anim.h"
#include "battle_gfx_sfx_util.h"
#include "sprite.h"
#include "test/battle.h"

static void ExpectCleanRockfallAnimation(void)
{
    EXPECT(!gAnimScriptActive);
    EXPECT_EQ(gAnimVisualTaskCount, 0);
    EXPECT_EQ(gAnimSoundTaskCount, 0);
    for (u32 battler = 0; battler < gBattlersCount; battler++)
    {
        u32 sprite = gBattlerSpriteIds[battler];
        EXPECT_LT(sprite, MAX_SPRITES);
        EXPECT(gSprites[sprite].inUse);
        EXPECT(!gSprites[sprite].invisible);
    }
}

SINGLE_BATTLE_TEST("Max Rockfall visuals: mixed rocks finish cleanly on both sides")
{
    struct BattlePokemon *user = player;
    struct BattlePokemon *foe = opponent;
    PARAMETRIZE { user = player; foe = opponent; }
    PARAMETRIZE { user = opponent; foe = player; }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
    } WHEN {
        TURN { MOVE(user, MOVE_ROCK_THROW, gimmick: GIMMICK_DYNAMAX); MOVE(foe, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_ROCKFALL, user);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectCleanRockfallAnimation();
    }
}

DOUBLE_BATTLE_TEST("Max Rockfall visuals: mixed rocks work from every doubles position")
{
    struct BattlePokemon *user = playerLeft;
    struct BattlePokemon *foe = opponentLeft;
    for (u32 slot = 0; slot < MAX_BATTLERS_COUNT; slot++)
        PARAMETRIZE { user = &gBattleMons[slot]; foe = slot & 1 ? playerLeft : opponentLeft; }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
    } WHEN {
        TURN {
            MOVE(user, MOVE_ROCK_THROW, target: foe, gimmick: GIMMICK_DYNAMAX);
            for (u32 battler = 0; battler < MAX_BATTLERS_COUNT; battler++)
                if (&gBattleMons[battler] != user) MOVE(&gBattleMons[battler], MOVE_SPLASH);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_ROCKFALL, user);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectCleanRockfallAnimation();
    }
}

SINGLE_BATTLE_TEST("Max Rockfall visuals: shared Rock Wrecker animation still finishes cleanly")
{
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_WRECKER); MOVE(opponent, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_WRECKER, player);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectCleanRockfallAnimation();
    }
}
