#include "global.h"
#include "gpu_regs.h"
#include "main.h"
#include "main_menu.h"
#include "m4a.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"
#include "title_screen.h"
#include "constants/songs.h"
#include "test/test.h"

extern const struct SongHeader mus_frontier_title;

static const u32 sStillTiles[] = INCBIN_U32("graphics/custom/frontier_title/still.tiles.bin");
static const u16 sStillMap[] = INCBIN_U16("graphics/custom/frontier_title/still.map.bin");
static const u16 sStillPalette[] = INCBIN_U16("graphics/custom/frontier_title/still.palette.bin");

static void Frame(void)
{
    gMain.callback2();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void StartTitle(void)
{
    gMain.newKeys = 0;
    gMain.heldKeys = 0;
    SetMainCallback2(CB2_InitTitleScreen);
    for (u32 i = 0; i < 650; i++) Frame();
}

static void Cleanup(MainCallback callback, IntrCallback vblank)
{
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    SetVBlankCallback(vblank);
    SetMainCallback2(callback);
}

TEST("Frontier title: still tiles, tilemap and reserved palette survive the title animation")
{
    MainCallback callback = gMain.callback2;
    IntrCallback vblank = gMain.vblankCallback;
    StartTitle();
    EXPECT_NE(GetGpuReg(REG_OFFSET_DISPCNT) & DISPCNT_BG0_ON, 0);
    EXPECT_EQ(GetGpuReg(REG_OFFSET_DISPCNT) & DISPCNT_BG1_ON, 0);
    EXPECT_EQ(memcmp((void *)BG_CHAR_ADDR(2), sStillTiles, sizeof(sStillTiles)), 0);
    EXPECT_EQ(memcmp((void *)BG_SCREEN_ADDR(26), sStillMap, sizeof(sStillMap)), 0);
    EXPECT_EQ(memcmp(&gPlttBufferFaded[BG_PLTT_ID(15)], sStillPalette, sizeof(sStillPalette)), 0);
    for (u32 i = 0; i < 300; i++) Frame();
    EXPECT_EQ(GetGpuReg(REG_OFFSET_BG0HOFS), 0);
    EXPECT_EQ(GetGpuReg(REG_OFFSET_BG0VOFS), 0);
    EXPECT_EQ(memcmp(&gPlttBufferFaded[BG_PLTT_ID(15)], sStillPalette, sizeof(sStillPalette)), 0);
    Cleanup(callback, vblank);
}

TEST("Frontier title: dedicated music is appended without replacing the original song")
{
    MainCallback callback = gMain.callback2;
    IntrCallback vblank = gMain.vblankCallback;
    StartTitle();
    EXPECT(gMPlayInfo_BGM.songHeader == &mus_frontier_title);
    EXPECT(gSongTable[MUS_FRONTIER_TITLE].header == &mus_frontier_title);
    EXPECT_EQ(mus_frontier_title.trackCount, 7);
    EXPECT(gSongTable[MUS_TITLE].header != &mus_frontier_title);
    Cleanup(callback, vblank);
}

TEST("Frontier title: A and Start still fade into the original main menu")
{
    MainCallback callback = gMain.callback2;
    IntrCallback vblank = gMain.vblankCallback;
    const u16 keys[] = {A_BUTTON, START_BUTTON};
    for (u32 i = 0; i < ARRAY_COUNT(keys); i++)
    {
        StartTitle();
        MainCallback titleCallback = gMain.callback2;
        gMain.newKeys = keys[i];
        Frame();
        EXPECT_NE(gMain.callback2, titleCallback);
        gMain.newKeys = 0;
        for (u32 j = 0; j < 180 && gMain.callback2 != CB2_InitMainMenu; j++) Frame();
        EXPECT_EQ(gMain.callback2, CB2_InitMainMenu);
    }
    Cleanup(callback, vblank);
}
