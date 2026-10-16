#include "global.h"
#include "test/battle.h"
#include "rtc.h"
#include "sprite.h"
#include "task.h"

void AnimTask_AllBattlersInvisibleExceptAttackerAndTarget(u8 taskId);

static void ExpectSparksurferAnimationFinished(void)
{
    EXPECT_EQ(gAnimScriptActive, FALSE);
    EXPECT_EQ(gAnimVisualTaskCount, 0);
    EXPECT_EQ(gAnimSoundTaskCount, 0);
    EXPECT_EQ(gLoadFail, FALSE);
    for (u32 battler = 0; battler < gBattlersCount; battler++)
    {
        if (!IsBattlerSpritePresent(battler))
            continue;
        u32 mon = gBattlerSpriteIds[battler];
        u32 box = gHealthboxSpriteIds[battler];
        EXPECT_LT(mon, MAX_SPRITES);
        EXPECT(gSprites[mon].inUse);
        EXPECT(!gSprites[mon].invisible);
        EXPECT_EQ(gSprites[mon].x2, 0);
        EXPECT_EQ(gSprites[mon].y2, 0);
        EXPECT_LT(box, MAX_SPRITES);
        EXPECT(!gSprites[box].invisible);
    }
}

DOUBLE_BATTLE_TEST("Stoked Sparksurfer anim: opponent Z animation completes after the player Terastallizes")
{
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        RtcInitLocalTimeOffset(18, 0);
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_GRASS); HP(9999); MaxHP(9999); }
        PLAYER(SPECIES_WOBBUFFET) { HP(9999); MaxHP(9999); }
        OPPONENT(SPECIES_RAICHU_ALOLA) { Item(ITEM_ALORAICHIUM_Z); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE, gimmick: GIMMICK_TERA);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_THUNDERBOLT, target: playerRight, gimmick: GIMMICK_Z_MOVE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_CHARGE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, playerLeft);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ZMOVE_ACTIVATE, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STOKED_SPARKSURFER, opponentLeft);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectSparksurferAnimationFinished();
    }
}

TEST("Stoked Sparksurfer anim: hiding bystanders ignores invalid sprite ids")
{
    for (u32 i = 0; i < 2; i++)
    {
        u32 invalidId = i ? SPRITE_NONE : MAX_SPRITES;
        gBattlersCount = MAX_BATTLERS_COUNT;
        gBattleAnimAttacker = 0;
        gBattleAnimTarget = 1;
        gBattlerSpriteIds[0] = 0;
        gBattlerSpriteIds[1] = 1;
        gBattlerSpriteIds[2] = invalidId;
        gBattlerSpriteIds[3] = invalidId;
        gSprites[MAX_SPRITES].invisible = FALSE;
        gAnimVisualTaskCount = 1;
        u32 taskId = CreateTask(AnimTask_AllBattlersInvisibleExceptAttackerAndTarget, 10);
        AnimTask_AllBattlersInvisibleExceptAttackerAndTarget(taskId);
        EXPECT(!gSprites[MAX_SPRITES].invisible);
        EXPECT_EQ(gAnimVisualTaskCount, 0);
    }
}

DOUBLE_BATTLE_TEST("Stoked Sparksurfer anim: opponent Z animation ignores a fainted player partner")
{
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        RtcInitLocalTimeOffset(18, 0);
        PLAYER(SPECIES_WOBBUFFET) { HP(9999); MaxHP(9999); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_RAICHU_ALOLA) { Item(ITEM_ALORAICHIUM_Z); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_MEMENTO, target: opponentRight);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_THUNDERBOLT, target: playerLeft, gimmick: GIMMICK_Z_MOVE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ZMOVE_ACTIVATE, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STOKED_SPARKSURFER, opponentLeft);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        EXPECT_EQ(playerRight->hp, 0);
        ExpectSparksurferAnimationFinished();
    }
}

SINGLE_BATTLE_TEST("Stoked Sparksurfer anim: opponent and player finish the real Z animation at every time of day")
{
    u32 side = B_SIDE_OPPONENT, hour = 12;
    for (u32 i = 0; i < 2; i++)
        for (u32 h = 12; h <= 24; h += 6)
            PARAMETRIZE { side = i; hour = h % 24; }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        RtcInitLocalTimeOffset(hour, 0);
        PLAYER(side == B_SIDE_PLAYER ? SPECIES_RAICHU_ALOLA : SPECIES_WOBBUFFET) {
            Item(side == B_SIDE_PLAYER ? ITEM_ALORAICHIUM_Z : ITEM_NONE);
            HP(9999); MaxHP(9999);
        }
        OPPONENT(side == B_SIDE_OPPONENT ? SPECIES_RAICHU_ALOLA : SPECIES_WOBBUFFET) {
            Item(side == B_SIDE_OPPONENT ? ITEM_ALORAICHIUM_Z : ITEM_NONE);
            HP(9999); MaxHP(9999);
        }
    } WHEN {
        TURN {
            MOVE(player, side == B_SIDE_PLAYER ? MOVE_THUNDERBOLT : MOVE_CELEBRATE,
                gimmick: side == B_SIDE_PLAYER ? GIMMICK_Z_MOVE : GIMMICK_NONE);
            MOVE(opponent, side == B_SIDE_OPPONENT ? MOVE_THUNDERBOLT : MOVE_CELEBRATE,
                gimmick: side == B_SIDE_OPPONENT ? GIMMICK_Z_MOVE : GIMMICK_NONE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ZMOVE_ACTIVATE, side == B_SIDE_PLAYER ? player : opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STOKED_SPARKSURFER, side == B_SIDE_PLAYER ? player : opponent);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        EXPECT_EQ(gLocalTime.hours, hour);
        ExpectSparksurferAnimationFinished();
    }
}

DOUBLE_BATTLE_TEST("Stoked Sparksurfer anim: either opponent can attack either player in doubles")
{
    u32 right = 0, targetRight = 0;
    for (u32 i = 0; i < 2; i++)
        for (u32 j = 0; j < 2; j++)
            PARAMETRIZE { right = i; targetRight = j; }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        RtcInitLocalTimeOffset(18, 0);
        PLAYER(SPECIES_WOBBUFFET) { HP(9999); MaxHP(9999); }
        PLAYER(SPECIES_WOBBUFFET) { HP(9999); MaxHP(9999); }
        OPPONENT(right ? SPECIES_WOBBUFFET : SPECIES_RAICHU_ALOLA) { Item(right ? ITEM_NONE : ITEM_ALORAICHIUM_Z); }
        OPPONENT(right ? SPECIES_RAICHU_ALOLA : SPECIES_WOBBUFFET) { Item(right ? ITEM_ALORAICHIUM_Z : ITEM_NONE); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, right ? MOVE_CELEBRATE : MOVE_THUNDERBOLT,
                target: targetRight ? playerRight : playerLeft, gimmick: right ? GIMMICK_NONE : GIMMICK_Z_MOVE);
            MOVE(opponentRight, right ? MOVE_THUNDERBOLT : MOVE_CELEBRATE,
                target: targetRight ? playerRight : playerLeft, gimmick: right ? GIMMICK_Z_MOVE : GIMMICK_NONE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ZMOVE_ACTIVATE, right ? opponentRight : opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STOKED_SPARKSURFER, right ? opponentRight : opponentLeft);
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        ExpectSparksurferAnimationFinished();
    }
}
