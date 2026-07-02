#include "global.h"
#include "ultra_help.h"
#include "bg.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "strings.h"
#include "string_util.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

#define INFO_VIEWER_ALT_FRAME_PALETTE_NUM 14
#define INFO_VIEWER_HISTORY_SIZE 8

enum
{
    WIN_INFO_VIEWER,
};

enum
{
    INFO_VIEWER_MODE_PAGE,
    INFO_VIEWER_MODE_CHOICE,
};

struct InfoViewerStyle
{
    u8 windowFill;
    u8 framePaletteNum;
    const u16 *framePalette;
    u8 textColors[3];
};

struct InfoViewerChoice
{
    const u8 *text;
    u16 nextInfoId;
};

struct InfoViewerEntry
{
    const u8 *title;
    const u8 *const *pages;
    u8 pageCount;
    const struct InfoViewerChoice *choices;
    u8 choiceCount;
    const struct InfoViewerStyle *style;
};

#include "data/info_viewer.h"

static void CB2_InitInfoViewer(void);
static void MainCB2_InfoViewer(void);
static void VBlankCB_InfoViewer(void);
static void Task_InfoViewerFadeIn(u8 taskId);
static void Task_InfoViewerHandleInput(u8 taskId);
static void Task_InfoViewerBeginFadeOut(u8 taskId);
static void Task_InfoViewerExit(u8 taskId);
static void DrawInfoViewerPage(void);
static void DrawInfoViewerChoices(void);
static void DrawInfoViewerPageNumber(void);
static void ResetBgCoordinates(void);
static void ClearVramOamPlttRegs(void);
static void ClearTasksAndGraphicalStructs(void);
static const struct InfoViewerEntry *GetInfoViewerEntry(u16 infoId);
static const struct InfoViewerStyle *GetInfoViewerStyle(void);
static void LoadInfoViewerWindowGfx(void);
static bool32 InfoViewerHasChoices(void);
static void SwitchInfoViewerEntry(u16 infoId, bool32 saveHistory);
static void PushInfoViewerHistory(u16 infoId);
static bool32 PopInfoViewerHistory(u16 *infoId);
static void SelectInfoViewerChoice(u8 taskId);

EWRAM_DATA static u8 *sInfoViewerTilemapBuffer = NULL;
EWRAM_DATA static const struct InfoViewerEntry *sInfoViewerEntry = NULL;
EWRAM_DATA static u8 sInfoViewerPage = 0;
EWRAM_DATA static u8 sInfoViewerMode = INFO_VIEWER_MODE_PAGE;
EWRAM_DATA static u8 sInfoViewerChoiceCursor = 0;
EWRAM_DATA static u16 sInfoViewerInfoId = INFO_FRONTIER_RULES;
EWRAM_DATA static u16 sInfoViewerHistory[INFO_VIEWER_HISTORY_SIZE] = {0};
EWRAM_DATA static u8 sInfoViewerHistoryDepth = 0;
EWRAM_DATA static MainCallback sInfoViewerExitCallback = NULL;

static const struct BgTemplate sInfoViewerBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    }
};

static const struct WindowTemplate sInfoViewerWindowTemplates[] =
{
    [WIN_INFO_VIEWER] =
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 28,
        .height = 18,
        .paletteNum = STD_WINDOW_PALETTE_NUM,
        .baseBlock = 1
    },
    DUMMY_WIN_TEMPLATE
};

static void VBlankCB_InfoViewer(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void MainCB2_InfoViewer(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void Special_OpenInfoViewer(void)
{
    StartInfoViewer(gSpecialVar_0x8004, CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

void ultra_help(void)
{
    Special_OpenInfoViewer();
}

void StartUltraHelp(u16 topic, MainCallback exitCallback)
{
    StartInfoViewer(topic, exitCallback);
}

void StartInfoViewer(u16 infoId, MainCallback exitCallback)
{
    sInfoViewerPage = 0;
    sInfoViewerMode = INFO_VIEWER_MODE_PAGE;
    sInfoViewerChoiceCursor = 0;
    sInfoViewerInfoId = infoId;
    sInfoViewerEntry = GetInfoViewerEntry(infoId);
    sInfoViewerHistoryDepth = 0;
    sInfoViewerExitCallback = exitCallback;
    SetVBlankCallback(NULL);
    SetMainCallback2(CB2_InitInfoViewer);
}

static void CB2_InitInfoViewer(void)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        ClearVramOamPlttRegs();
        gMain.state++;
        break;
    case 1:
        ClearTasksAndGraphicalStructs();
        gMain.state++;
        break;
    case 2:
        sInfoViewerTilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sInfoViewerBgTemplates, ARRAY_COUNT(sInfoViewerBgTemplates));
        SetBgTilemapBuffer(0, sInfoViewerTilemapBuffer);
        ResetBgCoordinates();
        gMain.state++;
        break;
    case 3:
        InitWindows(sInfoViewerWindowTemplates);
        LoadInfoViewerWindowGfx();
        DeactivateAllTextPrinters();
        gMain.state++;
        break;
    case 4:
        ShowBg(0);
        DrawInfoViewerPage();
        CopyBgTilemapBufferToVram(0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP);
        SetVBlankCallback(VBlankCB_InfoViewer);
        CreateTask(Task_InfoViewerFadeIn, 8);
        SetMainCallback2(MainCB2_InfoViewer);
        gMain.state = 0;
        break;
    }
}

static void Task_InfoViewerFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_InfoViewerHandleInput;
}

static void Task_InfoViewerHandleInput(u8 taskId)
{
    if (JOY_NEW(START_BUTTON))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].func = Task_InfoViewerBeginFadeOut;
    }
    else if (sInfoViewerMode == INFO_VIEWER_MODE_CHOICE)
    {
        if (JOY_NEW(DPAD_UP))
        {
            PlaySE(SE_SELECT);
            if (sInfoViewerChoiceCursor == 0)
                sInfoViewerChoiceCursor = sInfoViewerEntry->choiceCount - 1;
            else
                sInfoViewerChoiceCursor--;
            DrawInfoViewerChoices();
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            PlaySE(SE_SELECT);
            sInfoViewerChoiceCursor++;
            if (sInfoViewerChoiceCursor >= sInfoViewerEntry->choiceCount)
                sInfoViewerChoiceCursor = 0;
            DrawInfoViewerChoices();
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            SelectInfoViewerChoice(taskId);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            sInfoViewerMode = INFO_VIEWER_MODE_PAGE;
            DrawInfoViewerPage();
        }
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (sInfoViewerPage + 1 < sInfoViewerEntry->pageCount)
        {
            sInfoViewerPage++;
            DrawInfoViewerPage();
        }
        else if (InfoViewerHasChoices())
        {
            sInfoViewerMode = INFO_VIEWER_MODE_CHOICE;
            sInfoViewerChoiceCursor = 0;
            DrawInfoViewerChoices();
        }
        else
        {
            gTasks[taskId].func = Task_InfoViewerBeginFadeOut;
        }
    }
    else if (JOY_NEW(R_BUTTON))
    {
        if (sInfoViewerPage + 1 < sInfoViewerEntry->pageCount)
        {
            PlaySE(SE_SELECT);
            sInfoViewerPage++;
            DrawInfoViewerPage();
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (sInfoViewerPage == 0)
        {
            gTasks[taskId].func = Task_InfoViewerBeginFadeOut;
        }
        else
        {
            sInfoViewerPage--;
            DrawInfoViewerPage();
        }
    }
    else if (JOY_NEW(L_BUTTON))
    {
        if (sInfoViewerPage != 0)
        {
            PlaySE(SE_SELECT);
            sInfoViewerPage--;
            DrawInfoViewerPage();
        }
    }
}

static void Task_InfoViewerBeginFadeOut(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = Task_InfoViewerExit;
}

static void Task_InfoViewerExit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        MainCallback exitCallback = sInfoViewerExitCallback;

        if (exitCallback == NULL)
            exitCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;

        ClearStdWindowAndFrame(WIN_INFO_VIEWER, FALSE);
        FreeAllWindowBuffers();
        Free(sInfoViewerTilemapBuffer);
        sInfoViewerTilemapBuffer = NULL;
        sInfoViewerEntry = NULL;
        sInfoViewerInfoId = INFO_FRONTIER_RULES;
        sInfoViewerMode = INFO_VIEWER_MODE_PAGE;
        sInfoViewerChoiceCursor = 0;
        sInfoViewerHistoryDepth = 0;
        sInfoViewerExitCallback = NULL;
        DestroyTask(taskId);
        SetMainCallback2(exitCallback);
    }
}

static void DrawInfoViewerPage(void)
{
    const struct InfoViewerStyle *style;

    sInfoViewerEntry = GetInfoViewerEntry(sInfoViewerInfoId);
    style = GetInfoViewerStyle();

    if (sInfoViewerPage >= sInfoViewerEntry->pageCount)
        sInfoViewerPage = 0;

    DrawStdFrameWithCustomTileAndPalette(WIN_INFO_VIEWER, FALSE, STD_WINDOW_BASE_TILE_NUM, style->framePaletteNum);
    FillWindowPixelBuffer(WIN_INFO_VIEWER, style->windowFill);
    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_NORMAL, 8, 6, 0, 0, style->textColors, TEXT_SKIP_DRAW, sInfoViewerEntry->title);
    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, 8, 28, 0, 2, style->textColors, TEXT_SKIP_DRAW, sInfoViewerEntry->pages[sInfoViewerPage]);
    DrawInfoViewerPageNumber();
    CopyWindowToVram(WIN_INFO_VIEWER, COPYWIN_FULL);
}

static void DrawInfoViewerChoices(void)
{
    const struct InfoViewerStyle *style = GetInfoViewerStyle();
    u8 i;

    if (!InfoViewerHasChoices())
    {
        sInfoViewerMode = INFO_VIEWER_MODE_PAGE;
        DrawInfoViewerPage();
        return;
    }

    if (sInfoViewerChoiceCursor >= sInfoViewerEntry->choiceCount)
        sInfoViewerChoiceCursor = 0;

    DrawStdFrameWithCustomTileAndPalette(WIN_INFO_VIEWER, FALSE, STD_WINDOW_BASE_TILE_NUM, style->framePaletteNum);
    FillWindowPixelBuffer(WIN_INFO_VIEWER, style->windowFill);
    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_NORMAL, 8, 6, 0, 0, style->textColors, TEXT_SKIP_DRAW, sInfoViewerEntry->title);
    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, 8, 30, 0, 0, style->textColors, TEXT_SKIP_DRAW, COMPOUND_STRING("{JPN}どうしますか"));

    for (i = 0; i < sInfoViewerEntry->choiceCount; i++)
    {
        u8 y = 54 + i * 18;

        if (i == sInfoViewerChoiceCursor)
            AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_NORMAL, 8, y, 0, 0, style->textColors, TEXT_SKIP_DRAW, gText_SelectorArrow3);
        AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, 24, y + 1, 0, 0, style->textColors, TEXT_SKIP_DRAW, sInfoViewerEntry->choices[i].text);
    }

    CopyWindowToVram(WIN_INFO_VIEWER, COPYWIN_FULL);
}

static void DrawInfoViewerPageNumber(void)
{
    const struct InfoViewerStyle *style = GetInfoViewerStyle();
    u8 *txtPtr = gStringVar1;
    s32 x;

    txtPtr = ConvertIntToDecimalStringN(txtPtr, sInfoViewerPage + 1, STR_CONV_MODE_LEFT_ALIGN, 2);
    txtPtr = StringCopy(txtPtr, COMPOUND_STRING("/"));
    ConvertIntToDecimalStringN(txtPtr, sInfoViewerEntry->pageCount, STR_CONV_MODE_LEFT_ALIGN, 2);

    x = GetStringRightAlignXOffset(FONT_SMALL, gStringVar1, 212);
    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, x, 132, 0, 0, style->textColors, TEXT_SKIP_DRAW, gStringVar1);
}

static const struct InfoViewerEntry *GetInfoViewerEntry(u16 infoId)
{
    const struct InfoViewerEntry *entry;

    if (infoId >= INFO_COUNT)
        infoId = INFO_FRONTIER_RULES;

    entry = &sInfoViewerEntries[infoId];
    if (entry->title == NULL || entry->pages == NULL || entry->pageCount == 0)
        entry = &sInfoViewerEntries[INFO_FRONTIER_RULES];

    return entry;
}

static const struct InfoViewerStyle *GetInfoViewerStyle(void)
{
    if (sInfoViewerEntry != NULL && sInfoViewerEntry->style != NULL)
        return sInfoViewerEntry->style;

    return &sInfoViewerStyle_Default;
}

static void LoadInfoViewerWindowGfx(void)
{
    const struct InfoViewerStyle *style = GetInfoViewerStyle();

    LoadStdWindowGfx(WIN_INFO_VIEWER, STD_WINDOW_BASE_TILE_NUM, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
    if (style->framePalette != NULL)
        LoadPalette(style->framePalette, BG_PLTT_ID(style->framePaletteNum), PLTT_SIZE_4BPP);
}

static bool32 InfoViewerHasChoices(void)
{
    return sInfoViewerEntry != NULL
        && sInfoViewerEntry->choices != NULL
        && sInfoViewerEntry->choiceCount != 0;
}

static void SwitchInfoViewerEntry(u16 infoId, bool32 saveHistory)
{
    if (saveHistory)
        PushInfoViewerHistory(sInfoViewerInfoId);

    sInfoViewerInfoId = infoId;
    sInfoViewerEntry = GetInfoViewerEntry(infoId);
    sInfoViewerPage = 0;
    sInfoViewerChoiceCursor = 0;
    sInfoViewerMode = INFO_VIEWER_MODE_PAGE;
    LoadInfoViewerWindowGfx();
    DrawInfoViewerPage();
}

static void PushInfoViewerHistory(u16 infoId)
{
    u8 i;

    if (sInfoViewerHistoryDepth < INFO_VIEWER_HISTORY_SIZE)
    {
        sInfoViewerHistory[sInfoViewerHistoryDepth++] = infoId;
    }
    else
    {
        for (i = 1; i < INFO_VIEWER_HISTORY_SIZE; i++)
            sInfoViewerHistory[i - 1] = sInfoViewerHistory[i];
        sInfoViewerHistory[INFO_VIEWER_HISTORY_SIZE - 1] = infoId;
    }
}

static bool32 PopInfoViewerHistory(u16 *infoId)
{
    if (sInfoViewerHistoryDepth == 0)
        return FALSE;

    *infoId = sInfoViewerHistory[--sInfoViewerHistoryDepth];
    return TRUE;
}

static void SelectInfoViewerChoice(u8 taskId)
{
    u16 nextInfoId;

    if (!InfoViewerHasChoices())
        return;

    nextInfoId = sInfoViewerEntry->choices[sInfoViewerChoiceCursor].nextInfoId;
    if (nextInfoId == INFO_VIEWER_CLOSE)
    {
        gTasks[taskId].func = Task_InfoViewerBeginFadeOut;
    }
    else if (nextInfoId == INFO_VIEWER_BACK)
    {
        if (PopInfoViewerHistory(&nextInfoId))
            SwitchInfoViewerEntry(nextInfoId, FALSE);
        else
            gTasks[taskId].func = Task_InfoViewerBeginFadeOut;
    }
    else
    {
        SwitchInfoViewerEntry(nextInfoId, TRUE);
    }
}

static void ClearVramOamPlttRegs(void)
{
    DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
    DmaClear32(3, OAM, OAM_SIZE);
    DmaClear16(3, PLTT, PLTT_SIZE);

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
}

static void ClearTasksAndGraphicalStructs(void)
{
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
}

static void ResetBgCoordinates(void)
{
    ChangeBgX(0, 0, BG_COORD_SET);
    ChangeBgY(0, 0, BG_COORD_SET);
}
