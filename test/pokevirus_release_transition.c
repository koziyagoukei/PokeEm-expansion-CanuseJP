#include "global.h"
#include "battle_setup.h"
#include "battle_transition_pokevirus.h"
#include "palette.h"
#include "script.h"
#include "sprite.h"
#include "task.h"
#include "test/test.h"
#include "test/overworld_script.h"
#include "constants/event_objects.h"
#include "constants/opponents.h"

asm(".set LOCALID_NONE, " STR(LOCALID_NONE) "\n"
    ".set TRAINER_NONE, " STR(TRAINER_NONE) "\n");

static void ResetTransition(void)
{
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetPaletteFade();
}

static u32 CountChains(bool32 fromTop)
{
    u32 count = 0;
    for (u32 i = 0; i < MAX_SPRITES; i++)
        if (gSprites[i].inUse && gSprites[i].template->tileTag == POKEVIRUS_CHAIN_TAG && gSprites[i].vFlip != fromTop)
            count++;
    return count;
}

static void FinishTransition(void)
{
    for (u32 frame = 0; frame < 240 && FuncIsActiveTask(Task_PokevirusRelease); frame++)
    {
        RunTasks();
        UpdatePaletteFade();
        TransferPlttBuffer();
    }
    EXPECT(!FuncIsActiveTask(Task_PokevirusRelease));
    EXPECT_EQ(GetSpriteTileStartByTag(POKEVIRUS_CHAIN_TAG), TAG_NONE);
    EXPECT_EQ(IndexOfSpritePaletteTag(POKEVIRUS_CHAIN_TAG), 0xFF);
}

static void ConfigureRelease(bool32 release)
{
    TrainerBattleParameter args = {0};
    struct ScriptContext ctx;
    args.params.opponentA = TRAINER_SIDNEY;
    args.params.objEventLocalIdA = LOCALID_NONE;
    args.params.skipFlagCheck = TRUE;
    InitScriptContext(&ctx, NULL, NULL);
    ctx.scriptPtr = args.data;
    if (release)
        Script_TrainerBattlePokevirus(&ctx);
    else
        ConfigureTrainerBattle(&ctx);
}

TEST("Pokevirus transition: exactly three top and four bottom chains use one shared sheet")
{
    ResetTransition();
    CreateTask(Task_PokevirusRelease, 0);
    RunTasks();
    RunTasks();
    EXPECT_EQ(CountChains(TRUE), 3 * POKEVIRUS_CHAIN_SEGMENTS);
    EXPECT_EQ(CountChains(FALSE), 4 * POKEVIRUS_CHAIN_SEGMENTS);
    EXPECT_NE(GetSpriteTileStartByTag(POKEVIRUS_CHAIN_TAG), TAG_NONE);
    FinishTransition();
    EXPECT_EQ(CountChains(TRUE) + CountChains(FALSE), 0);
}

TEST("Pokevirus transition: chains grow from opposite edges and off-screen pieces stay hidden")
{
    ResetTransition();
    CreateTask(Task_PokevirusRelease, 0);
    RunTasks();
    for (u32 f = 0; f < 20; f++) RunTasks();
    u32 topVisible = 0, bottomVisible = 0;
    for (u32 i = 0; i < MAX_SPRITES; i++)
    {
        struct Sprite *sprite = &gSprites[i];
        if (!sprite->inUse || sprite->template->tileTag != POKEVIRUS_CHAIN_TAG) continue;
        if (sprite->y <= -32 || sprite->y >= DISPLAY_HEIGHT + 32)
            EXPECT(sprite->invisible);
        if (!sprite->invisible)
        {
            if (sprite->vFlip) { EXPECT_GT(sprite->y, DISPLAY_HEIGHT / 2); bottomVisible++; }
            else { EXPECT_LT(sprite->y, DISPLAY_HEIGHT / 2); topVisible++; }
        }
    }
    EXPECT_EQ(topVisible, 3);
    EXPECT_EQ(bottomVisible, 4);
    FinishTransition();
}

TEST("Pokevirus transition: sprite exhaustion fades out and preserves unrelated sprites")
{
    ResetTransition();
    for (u32 i = 0; i < MAX_SPRITES - 5; i++)
        EXPECT_NE(CreateSprite(&gDummySpriteTemplate, 20, 20, 0), MAX_SPRITES);
    CreateTask(Task_PokevirusRelease, 0);
    FinishTransition();
    u32 count = 0;
    for (u32 i = 0; i < MAX_SPRITES; i++) count += gSprites[i].inUse;
    EXPECT_EQ(count, MAX_SPRITES - 5);
    ResetTransition();
}

TEST("Pokevirus transition: palette exhaustion safely releases its tile allocation")
{
    ResetTransition();
    for (u32 i = 0; i < 16; i++) EXPECT_NE(AllocSpritePalette(0x6000 + i), 0xFF);
    CreateTask(Task_PokevirusRelease, 0);
    FinishTransition();
    for (u32 i = 0; i < 16; i++) EXPECT_NE(IndexOfSpritePaletteTag(0x6000 + i), 0xFF);
    ResetTransition();
}

TEST("Pokevirus transition: tile exhaustion also completes without waiting forever")
{
    ResetTransition();
    EXPECT_GE(AllocSpriteTiles(1024), 0);
    CreateTask(Task_PokevirusRelease, 0);
    FinishTransition();
    ResetTransition();
}

TEST("Pokevirus transition: custom battle command takes precedence once without changing trainer parameters")
{
    ConfigureRelease(TRUE);
    EXPECT_EQ(TRAINER_BATTLE_PARAM.opponentA, TRAINER_SIDNEY);
    EXPECT_EQ(GetTrainerBattleTransition(), B_TRANSITION_POKEVIRUS_RELEASE);
    EXPECT_NE(GetTrainerBattleTransition(), B_TRANSITION_POKEVIRUS_RELEASE);
    InitTrainerBattleParameter();
}

TEST("Pokevirus transition: configuring another trainer clears an unused release transition")
{
    ConfigureRelease(TRUE);
    ConfigureRelease(FALSE);
    EXPECT_NE(GetTrainerBattleTransition(), B_TRANSITION_POKEVIRUS_RELEASE);
    InitTrainerBattleParameter();
}

TEST("Pokevirus transition: script macro is assembled and marked as a battle effect")
{
    const u8 *script = OVERWORLD_SCRIPT(trainerbattle_pokevirus 261, 0, 0;);
    EXPECT(!Script_HasNoEffect(script));
}
