#include "global.h"
#include "battle.h"
#include "battle_gfx_sfx_util.h"
#include "contest.h"
#include "contest_util.h"
#include "data.h"
#include "event_data.h"
#include "main.h"
#include "malloc.h"
#include "pokemon.h"
#include "pokemon_animation.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "test/test.h"

static u32 CountTasks(void)
{
    u32 count = 0;
    for (u32 i = 0; i < NUM_TASKS; i++)
        if (gTasks[i].isActive)
            count++;
    return count;
}

static void Tick(u32 frames)
{
    for (u32 i = 0; i < frames; i++)
    {
        RunTasks();
        AnimateSprites();
    }
}

TEST("Contest entry: cancelling delayed and active Rayquaza animations leaves reused sprite slots untouched")
{
    u32 elapsed = 0;
    PARAMETRIZE { elapsed = 0; }
    PARAMETRIZE { elapsed = 30; }
    PARAMETRIZE { elapsed = 61; }
    ResetTasks();
    ResetSpriteData();
    u8 id = CreateSprite(&gDummySpriteTemplate, 0, 0, 0);
    EXPECT_LT(id, MAX_SPRITES);
    DoMonFrontSpriteAnimation(&gSprites[id], SPECIES_RAYQUAZA, TRUE, 0);
    Tick(elapsed);
    EXPECT_EQ(CountTasks(), 1);
    StopMonFrontSpriteAnimation(&gSprites[id]);
    EXPECT_EQ(CountTasks(), 0);
    DestroySprite(&gSprites[id]);
    u8 nextId = CreateSprite(&gDummySpriteTemplate, 0, 0, 0);
    EXPECT_EQ(nextId, id);
    gSprites[id].data[2] = 1234;
    Tick(180);
    EXPECT_EQ(CountTasks(), 0);
    EXPECT_EQ(gSprites[id].data[2], 1234);
    EXPECT(gSprites[id].callback == SpriteCallbackDummy);
    DestroySprite(&gSprites[id]);
}

TEST("Contest entry: stopping one picture does not cancel another picture's delay")
{
    ResetTasks();
    ResetSpriteData();
    u8 first = CreateSprite(&gDummySpriteTemplate, 0, 0, 0);
    u8 second = CreateSprite(&gDummySpriteTemplate, 0, 0, 0);
    DoMonFrontSpriteAnimation(&gSprites[first], SPECIES_RAYQUAZA, TRUE, 0);
    DoMonFrontSpriteAnimation(&gSprites[second], SPECIES_RAYQUAZA, TRUE, 0);
    EXPECT_EQ(CountTasks(), 2);
    StopMonFrontSpriteAnimation(&gSprites[first]);
    EXPECT_EQ(CountTasks(), 1);
    StopMonFrontSpriteAnimation(&gSprites[second]);
    EXPECT_EQ(CountTasks(), 0);
    DestroySprite(&gSprites[first]);
    DestroySprite(&gSprites[second]);
}

TEST("Contest entry: hide retains frame images until sprite destruction and is safe before the window opens")
{
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    gLinkContestFlags = 0;
    gSpecialVar_0x8006 = 0;
    gContestMons[0] = (struct ContestPokemon){.species = SPECIES_RAYQUAZA};
    ShowContestEntryMonPic();
    EXPECT(gMonSpritesGfxPtr != NULL);
    HideContestEntryMonPic();
    HideContestEntryMonPic();
    EXPECT(gMonSpritesGfxPtr != NULL);
    Tick(3);
    EXPECT(gMonSpritesGfxPtr == NULL);
    EXPECT_EQ(IndexOfSpritePaletteTag(SPECIES_RAYQUAZA), 0xFF);
    for (u32 i = 0; i < MAX_SPRITES; i++)
        EXPECT(!gSprites[i].inUse);
    // Let the independent cry task finish too.
    StopCryAndClearCrySongs();
    Tick(180);
    EXPECT_EQ(CountTasks(), 0);
}

TEST("Contest entry: completed front animations update KO counters only in battle")
{
    bool32 inBattle = FALSE;
    PARAMETRIZE { inBattle = FALSE; }
    PARAMETRIZE { inBattle = TRUE; }
    ResetTasks();
    ResetSpriteData();
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));
    gBattleStruct->battlerKOAnimsRunning = 1;
    gMain.inBattle = inBattle;
    u8 id = CreateSprite(&gDummySpriteTemplate, 0, 0, 0);
    LaunchAnimationTaskForFrontSprite(&gSprites[id], ANIM_H_SHAKE);
    RunTasks();
    gSprites[id].callback = SpriteCallbackDummy;
    RunTasks();
    EXPECT_EQ(CountTasks(), 0);
    EXPECT_EQ((u32)gBattleStruct->battlerKOAnimsRunning, inBattle ? 0 : 1);
    DestroySprite(&gSprites[id]);
    FREE_AND_SET_NULL(gBattleStruct);
    gMain.inBattle = FALSE;
}
