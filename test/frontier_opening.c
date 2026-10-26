#include "global.h"
#include "frontier_opening.h"
#include "intro.h"
#include "main.h"
#include "malloc.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"
#include "title_screen.h"
#include "test/test.h"

// The movie runs 11 bars of MUS_FRONTIER_OPENING (144 frames each); give it room.
#define OPENING_FRAME_LIMIT 2000

static void Frame(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    ProcessSpriteCopyRequests();
    UpdatePaletteFade();
    TransferPlttBuffer();
}

static void StartOpening(void)
{
    FrontierOpening_Stop();
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetPaletteFade();
    CreateTask(Task_FrontierOpeningStart, 0);
    Frame();
}

static void AdvanceTo(enum FrontierOpeningPhase phase)
{
    for (u32 i = 0; i < OPENING_FRAME_LIMIT && FrontierOpening_GetPhase() != phase; i++) Frame();
    EXPECT_EQ(FrontierOpening_GetPhase(), phase);
}

static struct Sprite *FindSprite(u16 tag)
{
    for (u32 i = 0; i < MAX_SPRITES; i++)
        if (gSprites[i].inUse && gSprites[i].template->tileTag == tag)
            return &gSprites[i];
    return NULL;
}

TEST("Opening movie: all six scenes complete and return to the unchanged title callback")
{
    MainCallback callback = gMain.callback2;
    StartOpening();
    u32 visited = 0;
    for (u32 i = 0; i < OPENING_FRAME_LIMIT && FrontierOpening_IsActive(); i++)
    {
        visited |= 1 << FrontierOpening_GetPhase();
        Frame();
    }
    EXPECT_EQ(visited, (1 << FRONTIER_OPENING_DONE) - 1);
    EXPECT(!FrontierOpening_IsActive());
    EXPECT_EQ(gMain.callback2, CB2_InitTitleScreen);
    EXPECT_EQ(gReservedSpriteTileCount, 0);
    for (u32 i = 0; i < MAX_SPRITES; i++) EXPECT(!gSprites[i].inUse);
    SetMainCallback2(callback);
}

TEST("Opening movie: bitmap-mode objects never overwrite the framebuffer pages")
{
    MainCallback callback = gMain.callback2;
    StartOpening();
    for (u32 i = 0; i < OPENING_FRAME_LIMIT && FrontierOpening_IsActive(); i++)
    {
        for (u32 j = 0; j < MAX_SPRITES; j++)
            if (gSprites[j].inUse) EXPECT_GE((u16)gSprites[j].oam.tileNum, 512);
        Frame();
    }
    SetMainCallback2(callback);
}

TEST("Opening movie: the Pokevirus floats above three guardians seen from behind")
{
    StartOpening();
    AdvanceTo(FRONTIER_OPENING_GUARDIANS);
    for (u32 i = 0; i < OPENING_FRAME_LIMIT && FrontierOpening_GetFrame() < 2 * 144 + 40; i++) Frame();
    EXPECT_EQ(FrontierOpening_GetPhase(), FRONTIER_OPENING_GUARDIANS);
    struct Sprite *virus = FindSprite(FRONTIER_OPENING_ACTOR_TAG);
    EXPECT(virus != NULL);
    EXPECT(!virus->invisible);
    EXPECT_LT(virus->y, 64);
    s16 left = DISPLAY_WIDTH;
    for (u32 i = 0; i < 3; i++)
    {
        struct Sprite *guardian = FindSprite(FRONTIER_OPENING_GUARDIAN_TAG + i);
        EXPECT(guardian != NULL);
        EXPECT(!guardian->invisible);
        // Standing on the bottom edge, below the Pokevirus.
        EXPECT_GE(guardian->y + guardian->y2 + 32, DISPLAY_HEIGHT - 4);
        EXPECT_GE(guardian->y + guardian->y2 - 32, virus->y + 32);
        left = min(left, guardian->x);
    }
    EXPECT_GE(left - 32, 0);
    FrontierOpening_Stop();
}

TEST("Opening movie: skipping any scene releases sprites, palettes and temporary allocations")
{
    for (u32 phase = FRONTIER_OPENING_FOREST; phase <= FRONTIER_OPENING_WHITE; phase++)
    {
        StartOpening();
        AdvanceTo(phase);
        FrontierOpening_Stop();
        FrontierOpening_Stop();
        EXPECT(!FrontierOpening_IsActive());
        EXPECT_EQ(gReservedSpriteTileCount, 0);
        for (u32 tag = 0; tag < FRONTIER_OPENING_TAG_COUNT; tag++)
        {
            EXPECT_EQ(GetSpriteTileStartByTag(FRONTIER_OPENING_ACTOR_TAG + tag), TAG_NONE);
            EXPECT_EQ(IndexOfSpritePaletteTag(FRONTIER_OPENING_ACTOR_TAG + tag), 0xFF);
        }
        for (u32 i = 0; i < MAX_SPRITES; i++) EXPECT(!gSprites[i].inUse);
        void *block = Alloc(0x10000);
        EXPECT(block != NULL);
        Free(block);
    }
}

TEST("Opening movie: repeated playback does not leave stale task or sprite state")
{
    MainCallback callback = gMain.callback2;
    for (u32 pass = 0; pass < 3; pass++)
    {
        StartOpening();
        AdvanceTo(FRONTIER_OPENING_DONE);
        EXPECT_EQ(gMain.callback2, CB2_InitTitleScreen);
        EXPECT_EQ(FrontierOpening_GetFrame(), 0);
    }
    SetMainCallback2(callback);
}

TEST("Opening movie: every scene accepts a key at any moment and reaches the title")
{
    MainCallback callback = gMain.callback2;
    u16 keys = gMain.newKeys;
    for (u32 phase = FRONTIER_OPENING_FOREST; phase <= FRONTIER_OPENING_WHITE; phase++)
    {
        StartOpening();
        AdvanceTo(phase);
        // Fades use the blend unit, never the palette fader that blocks skipping.
        EXPECT(!gPaletteFade.active);
        SetMainCallback2(MainCB2_Intro);
        gMain.newKeys = A_BUTTON;
        MainCB2_Intro();
        EXPECT_NE(gMain.callback2, MainCB2_Intro);
        gMain.newKeys = 0;
        for (u32 i = 0; i < 100 && gMain.callback2 != CB2_InitTitleScreen; i++)
        {
            gMain.callback2();
            TransferPlttBuffer();
        }
        EXPECT_EQ(gMain.callback2, CB2_InitTitleScreen);
        EXPECT(!FrontierOpening_IsActive());
        EXPECT_EQ(gReservedSpriteTileCount, 0);
    }
    gMain.newKeys = keys;
    SetMainCallback2(callback);
}

TEST("Opening movie: distant trainers use compact affine bounds without wrapping above the screen")
{
    StartOpening();
    AdvanceTo(FRONTIER_OPENING_NETWORK);
    u32 trainers = 0;
    for (u32 i = 0; i < MAX_SPRITES; i++)
    {
        if (gSprites[i].inUse && gSprites[i].template->tileTag >= FRONTIER_OPENING_TRAINER_TAG
         && gSprites[i].template->tileTag < FRONTIER_OPENING_TRAINER_TAG + 4)
        {
            EXPECT_EQ((u8)gSprites[i].oam.affineMode, ST_OAM_AFFINE_NORMAL);
            EXPECT_GE(gSprites[i].y + gSprites[i].centerToCornerVecY, 0);
            trainers++;
        }
    }
    EXPECT_EQ(trainers, 4);
    FrontierOpening_Stop();
}
