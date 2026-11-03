#include "global.h"
#include "battle_anim.h"
#include "battle_gfx_sfx_util.h"
#include "sprite.h"
#include "test/battle.h"

static const enum Move sParticleMoves[] =
{
    MOVE_METAL_CLAW, MOVE_DRAGON_CLAW, MOVE_CRUSH_CLAW, MOVE_RAZOR_SHELL,
    MOVE_SOLAR_BLADE, MOVE_BITTER_BLADE, MOVE_ROCK_THROW,
    MOVE_ROCK_SLIDE, MOVE_ANCIENT_POWER, MOVE_ROCK_TOMB,
    MOVE_STOMPING_TANTRUM, MOVE_SUNSTEEL_STRIKE,
};

static void ExpectCleanAnimation(void)
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

SINGLE_BATTLE_TEST("Claw and rock visuals: shared assets finish cleanly on both sides")
{
    enum Move move = MOVE_DRAGON_CLAW;
    struct BattlePokemon *user = player;
    struct BattlePokemon *foe = opponent;
    for (u32 moveIndex = 0; moveIndex < ARRAY_COUNT(sParticleMoves); moveIndex++)
    {
        PARAMETRIZE { move = sParticleMoves[moveIndex]; user = player; foe = opponent; }
        PARAMETRIZE { move = sParticleMoves[moveIndex]; user = opponent; foe = player; }
    }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); Item(ITEM_POWER_HERB); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); Item(ITEM_POWER_HERB); }
    } WHEN {
        TURN { MOVE(user, move); MOVE(foe, MOVE_SPLASH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, user);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectCleanAnimation();
    }
}

DOUBLE_BATTLE_TEST("Claw and rock visuals: claws and spread rocks work from every doubles position")
{
    enum Move move = MOVE_DRAGON_CLAW;
    struct BattlePokemon *user = playerLeft;
    struct BattlePokemon *foe = opponentLeft;
    for (u32 slot = 0; slot < MAX_BATTLERS_COUNT; slot++)
    {
        PARAMETRIZE { move = MOVE_DRAGON_CLAW; user = &gBattleMons[slot]; foe = slot & 1 ? playerLeft : opponentLeft; }
        PARAMETRIZE { move = MOVE_ROCK_SLIDE; user = &gBattleMons[slot]; foe = slot & 1 ? playerLeft : opponentLeft; }
    }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(2000); HP(2000); }
    } WHEN {
        TURN {
            if (move == MOVE_ROCK_SLIDE)
                MOVE(user, move);
            else
                MOVE(user, move, target: foe);
            for (u32 battler = 0; battler < MAX_BATTLERS_COUNT; battler++)
                if (&gBattleMons[battler] != user) MOVE(&gBattleMons[battler], MOVE_SPLASH);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, user);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectCleanAnimation();
    }
}
