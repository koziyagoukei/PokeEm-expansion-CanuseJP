#include "global.h"
#include "bg.h"
#include "frontier_story.h"
#include "gpu_regs.h"
#include "main.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/rgb.h"

// The end of the story: a few closing words on a black screen, then "おしまい". The field and
// the script that asked for it come back afterwards (special FrontierStory_ShowEnding, then
// waitstate, with the screen already faded to black).

enum
{
    ENDING_FADE_IN,
    ENDING_PRINT,
    ENDING_READ,
    ENDING_FIN,
    ENDING_FIN_WAIT,
    ENDING_FADE_OUT,
    ENDING_EXIT,
};

#define tState data[0]
#define tPage  data[1]
#define tTimer data[2]

#define WIN_TEXT 0
#define WIN_FIN  1

static const u16 sEndingBlack[] = {RGB_BLACK};

static const u8 *const sEndingPages[] =
{
    COMPOUND_STRING("{JPN}ヒトポケルスの 意識は\n夢の 中へ とけて 消えた。"),
    COMPOUND_STRING("{JPN}けれど 夢の 中で 眠り続ける\n人たちは まだ 大勢 いる。"),
    COMPOUND_STRING("{JPN}その 最後の ひとりが\n目を 覚ます 日まで……"),
    COMPOUND_STRING("{JPN}{PLAYER}たちの 家族旅行は\nもう少しだけ 続きそうだ。"),
};

static const u8 sEndingFin[] = _("{JPN}おしまい");

static const struct BgTemplate sBgTemplate =
{
    .bg = 0,
    .charBaseIndex = 2,
    .mapBaseIndex = 30,
};

static const struct WindowTemplate sWindows[] =
{
    {.bg = 0, .tilemapLeft = 2, .tilemapTop = 15, .width = 26, .height = 4, .paletteNum = 15, .baseBlock = 1},
    {.bg = 0, .tilemapLeft = 11, .tilemapTop = 8, .width = 8, .height = 2, .paletteNum = 15, .baseBlock = 0x80},
    DUMMY_WIN_TEMPLATE,
};

static void VBlankCB_FrontierEnding(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_RunFrontierEnding(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void PrintEndingPage(const u8 *text)
{
    FillWindowPixelBuffer(WIN_TEXT, PIXEL_FILL(1));
    DrawDialogFrameWithCustomTileAndPalette(WIN_TEXT, FALSE, 0xF3, 15);
    StringExpandPlaceholders(gStringVar4, text);
    PutWindowTilemap(WIN_TEXT);
    CopyWindowToVram(WIN_TEXT, COPYWIN_FULL);
    AddTextPrinterForMessage(TRUE);
}

static void PrintEndingFin(void)
{
    static const u8 sWhite[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};

    ClearDialogWindowAndFrameToTransparent(WIN_TEXT, TRUE);
    FillWindowPixelBuffer(WIN_FIN, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_FIN, FONT_NORMAL, 8, 0, sWhite, TEXT_SKIP_DRAW, sEndingFin);
    PutWindowTilemap(WIN_FIN);
    CopyWindowToVram(WIN_FIN, COPYWIN_FULL);
}

static void Task_FrontierEnding(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (gPaletteFade.active)
        return;

    switch (tState)
    {
    case ENDING_FADE_IN:
        if (++tTimer >= 30)
        {
            tTimer = 0;
            tState = ENDING_PRINT;
        }
        break;
    case ENDING_PRINT:
        PrintEndingPage(sEndingPages[tPage]);
        tState = ENDING_READ;
        break;
    case ENDING_READ:
        if (!RunTextPrintersAndIsPrinter0Active() && JOY_NEW(A_BUTTON | B_BUTTON))
        {
            if (++tPage < (s16)ARRAY_COUNT(sEndingPages))
            {
                tState = ENDING_PRINT;
            }
            else
            {
                PrintEndingFin();
                tState = ENDING_FIN;
            }
        }
        break;
    case ENDING_FIN:
        if (++tTimer >= 60)
            tState = ENDING_FIN_WAIT;
        break;
    case ENDING_FIN_WAIT:
        if (JOY_NEW(A_BUTTON | B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 2, 0, 16, RGB_BLACK);
            tState = ENDING_FADE_OUT;
        }
        break;
    case ENDING_FADE_OUT:
        tState = ENDING_EXIT;
        break;
    case ENDING_EXIT:
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
        break;
    }
}

static void CB2_StartFrontierEnding(void)
{
    u8 taskId;

    SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    ScanlineEffect_Stop();
    ResetBgsAndClearDma3BusyFlags(0);
    DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
    DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
    InitBgsFromTemplates(0, &sBgTemplate, 1);
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetPaletteFade();
    ResetTempTileDataBuffers();
    InitWindows(sWindows);
    DeactivateAllTextPrinters();
    LoadMessageBoxGfx(WIN_TEXT, 0xF3, BG_PLTT_ID(15));
    LoadPalette(sEndingBlack, 0, sizeof(sEndingBlack));
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    taskId = CreateTask(Task_FrontierEnding, 0);
    gTasks[taskId].tState = ENDING_FADE_IN;
    BeginNormalPaletteFade(PALETTES_ALL, 2, 16, 0, RGB_BLACK);
    ShowBg(0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    SetVBlankCallback(VBlankCB_FrontierEnding);
    SetMainCallback2(CB2_RunFrontierEnding);
}

void FrontierStory_ShowEnding(void)
{
    SetMainCallback2(CB2_StartFrontierEnding);
}
