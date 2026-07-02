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
#include "string_util.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

enum
{
    WIN_HELP,
};

struct UltraHelpPage
{
    const u8 *title;
    const u8 *body;
};

struct UltraHelpTopic
{
    const struct UltraHelpPage *pages;
    u8 pageCount;
};

static void CB2_InitUltraHelp(void);
static void MainCB2_UltraHelp(void);
static void VBlankCB_UltraHelp(void);
static void Task_UltraHelpFadeIn(u8 taskId);
static void Task_UltraHelpHandleInput(u8 taskId);
static void Task_UltraHelpBeginFadeOut(u8 taskId);
static void Task_UltraHelpExit(u8 taskId);
static void DrawUltraHelpPage(void);
static void DrawUltraHelpPageNumber(void);
static void ResetBgCoordinates(void);
static void ClearVramOamPlttRegs(void);
static void ClearTasksAndGraphicalStructs(void);

EWRAM_DATA static u8 *sUltraHelpTilemapBuffer = NULL;
EWRAM_DATA static const struct UltraHelpTopic *sUltraHelpTopic = NULL;
EWRAM_DATA static u8 sUltraHelpPage = 0;
EWRAM_DATA static u16 sUltraHelpTopicId = ULTRA_HELP_TOPIC_FRONTIER_RULES;
EWRAM_DATA static MainCallback sUltraHelpExitCallback = NULL;

static const struct BgTemplate sUltraHelpBgTemplates[] =
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

static const struct WindowTemplate sUltraHelpWindowTemplates[] =
{
    [WIN_HELP] =
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

static const u8 sUltraHelpTextColors[] =
{
    TEXT_COLOR_TRANSPARENT,
    TEXT_COLOR_DARK_GRAY,
    TEXT_COLOR_LIGHT_GRAY
};

static const u8 sText_CannotEnterPokemonTitle1[] = _("{JPN}さんかできない ポケモン");
static const u8 sText_CannotEnterPokemonBody1[] = _(
    "{JPN}ミュウツー ホウオウ ルギア\n"
    "グラードン カイオーガ\n"
    "レックウザ ディアルガ パルキア\n"
    "ギラティナ レシラム ゼクロム\n"
    "キュレム ゼルネアス イベルタル");

static const u8 sText_CannotEnterPokemonTitle2[] = _("{JPN}さんかできない ポケモン");
static const u8 sText_CannotEnterPokemonBody2[] = _(
    "{JPN}ジガルデ ソルガレオ ルナアーラ\n"
    "ネクロズマ ザシアン ザマゼンタ\n"
    "ムゲンダイナ コライドン\n"
    "ミライドン テラパゴス\n"
    "そのほか とくべつな ポケモン");

static const u8 sText_CannotUseItemsTitle[] = _("{JPN}つかえない アイテム");
static const u8 sText_CannotUseItemsBody[] = _(
    "{JPN}こころのしずく など\n"
    "フロンティアの ルールで\n"
    "もちこめない もちものが\n"
    "あります");

static const struct UltraHelpPage sUltraHelpPages_FrontierRules[] =
{
    {
        .title = sText_CannotEnterPokemonTitle1,
        .body = sText_CannotEnterPokemonBody1
    },
    {
        .title = sText_CannotEnterPokemonTitle2,
        .body = sText_CannotEnterPokemonBody2
    },
    {
        .title = sText_CannotUseItemsTitle,
        .body = sText_CannotUseItemsBody
    }
};

static const struct UltraHelpTopic sUltraHelpTopics[] =
{
    [ULTRA_HELP_TOPIC_FRONTIER_RULES] =
    {
        .pages = sUltraHelpPages_FrontierRules,
        .pageCount = ARRAY_COUNT(sUltraHelpPages_FrontierRules)
    }
};

static void VBlankCB_UltraHelp(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void MainCB2_UltraHelp(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void ultra_help(void)
{
    StartUltraHelp(gSpecialVar_0x8004, CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

void StartUltraHelp(u16 topic, MainCallback exitCallback)
{
    sUltraHelpPage = 0;
    sUltraHelpTopic = NULL;
    if (topic >= ULTRA_HELP_TOPIC_COUNT)
        topic = ULTRA_HELP_TOPIC_FRONTIER_RULES;
    sUltraHelpTopicId = topic;
    sUltraHelpExitCallback = exitCallback;
    SetVBlankCallback(NULL);
    SetMainCallback2(CB2_InitUltraHelp);
}

static void CB2_InitUltraHelp(void)
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
        sUltraHelpTilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sUltraHelpBgTemplates, ARRAY_COUNT(sUltraHelpBgTemplates));
        SetBgTilemapBuffer(0, sUltraHelpTilemapBuffer);
        ResetBgCoordinates();
        gMain.state++;
        break;
    case 3:
        InitWindows(sUltraHelpWindowTemplates);
        LoadStdWindowGfx(WIN_HELP, STD_WINDOW_BASE_TILE_NUM, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
        DeactivateAllTextPrinters();
        gMain.state++;
        break;
    case 4:
        ShowBg(0);
        DrawUltraHelpPage();
        CopyBgTilemapBufferToVram(0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP);
        SetVBlankCallback(VBlankCB_UltraHelp);
        CreateTask(Task_UltraHelpFadeIn, 8);
        SetMainCallback2(MainCB2_UltraHelp);
        gMain.state = 0;
        break;
    }
}

static void Task_UltraHelpFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_UltraHelpHandleInput;
}

static void Task_UltraHelpHandleInput(u8 taskId)
{
    if (JOY_NEW(START_BUTTON))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].func = Task_UltraHelpBeginFadeOut;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (sUltraHelpPage + 1 < sUltraHelpTopic->pageCount)
        {
            sUltraHelpPage++;
            DrawUltraHelpPage();
        }
        else
            gTasks[taskId].func = Task_UltraHelpBeginFadeOut;
    }
    else if (JOY_NEW(R_BUTTON))
    {
        if (sUltraHelpPage + 1 < sUltraHelpTopic->pageCount)
        {
            PlaySE(SE_SELECT);
            sUltraHelpPage++;
            DrawUltraHelpPage();
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (sUltraHelpPage == 0)
            gTasks[taskId].func = Task_UltraHelpBeginFadeOut;
        else
        {
            sUltraHelpPage--;
            DrawUltraHelpPage();
        }
    }
    else if (JOY_NEW(L_BUTTON))
    {
        if (sUltraHelpPage != 0)
        {
            PlaySE(SE_SELECT);
            sUltraHelpPage--;
            DrawUltraHelpPage();
        }
    }
}

static void Task_UltraHelpBeginFadeOut(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = Task_UltraHelpExit;
}

static void Task_UltraHelpExit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        MainCallback exitCallback = sUltraHelpExitCallback;
        if (exitCallback == NULL)
            exitCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;
        SetMainCallback2(exitCallback);
        Free(sUltraHelpTilemapBuffer);
        sUltraHelpTilemapBuffer = NULL;
        sUltraHelpTopic = NULL;
        sUltraHelpTopicId = ULTRA_HELP_TOPIC_FRONTIER_RULES;
        sUltraHelpExitCallback = NULL;
        ClearStdWindowAndFrame(WIN_HELP, FALSE);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
    }
}

static void DrawUltraHelpPage(void)
{
    u16 topic = sUltraHelpTopicId;

    if (topic >= ULTRA_HELP_TOPIC_COUNT)
        topic = ULTRA_HELP_TOPIC_FRONTIER_RULES;

    sUltraHelpTopic = &sUltraHelpTopics[topic];
    if (sUltraHelpPage >= sUltraHelpTopic->pageCount)
        sUltraHelpPage = 0;

    DrawStdWindowFrame(WIN_HELP, FALSE);
    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(TEXT_COLOR_WHITE));
    AddTextPrinterParameterized4(WIN_HELP, FONT_NORMAL, 8, 6, 0, 0, sUltraHelpTextColors, TEXT_SKIP_DRAW, sUltraHelpTopic->pages[sUltraHelpPage].title);
    AddTextPrinterParameterized4(WIN_HELP, FONT_SMALL, 8, 28, 0, 2, sUltraHelpTextColors, TEXT_SKIP_DRAW, sUltraHelpTopic->pages[sUltraHelpPage].body);
    DrawUltraHelpPageNumber();
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static void DrawUltraHelpPageNumber(void)
{
    u8 *txtPtr = gStringVar1;
    s32 x;

    txtPtr = ConvertIntToDecimalStringN(txtPtr, sUltraHelpPage + 1, STR_CONV_MODE_LEFT_ALIGN, 2);
    txtPtr = StringCopy(txtPtr, COMPOUND_STRING("/"));
    ConvertIntToDecimalStringN(txtPtr, sUltraHelpTopic->pageCount, STR_CONV_MODE_LEFT_ALIGN, 2);

    x = GetStringRightAlignXOffset(FONT_SMALL, gStringVar1, 212);
    AddTextPrinterParameterized4(WIN_HELP, FONT_SMALL, x, 132, 0, 0, sUltraHelpTextColors, TEXT_SKIP_DRAW, gStringVar1);
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
