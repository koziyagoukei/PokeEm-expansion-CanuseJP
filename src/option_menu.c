#include "global.h"
#include "option_menu.h"
#include "bg.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "list_menu.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "gba/m4a_internal.h"
#include "constants/battle_frontier.h"
#include "constants/rgb.h"

#define tMenuSelection data[0]
#define tTextSpeed data[1]
#define tBattleSceneOff data[2]
#define tBattleStyle data[3]
#define tSound data[4]
#define tButtonMode data[5]
#define tWindowFrameType data[6]
#define tTypeIconsOff data[7]
#define tFrontierAi data[8]    // index in sFrontierAiLevels, easiest first
#define tAllowBanned data[9]
#define tScrollArrows data[10]

enum
{
    MENUITEM_TEXTSPEED,
    MENUITEM_BATTLESCENE,
    MENUITEM_BATTLESTYLE,
    MENUITEM_TYPEICONS,
    MENUITEM_SOUND,
    MENUITEM_BUTTONMODE,
    MENUITEM_FRAMETYPE,
    MENUITEM_FRONTIER_AI,
    MENUITEM_BANNED_SPECIES,
    MENUITEM_CANCEL,
    MENUITEM_COUNT,
};

enum
{
    WIN_HEADER,
    WIN_OPTIONS
};

#define OPTION_ROW_HEIGHT 14
#define OPTION_FONT FONT_SMALL
// The window shows this many rows; the list scrolls to keep the selected one in view.
#define OPTION_VISIBLE_ROWS 8
#define OPTION_WINDOW_TOP 40
#define YPOS_TEXTSPEED    RowY(MENUITEM_TEXTSPEED)
#define YPOS_BATTLESCENE  RowY(MENUITEM_BATTLESCENE)
#define YPOS_BATTLESTYLE  RowY(MENUITEM_BATTLESTYLE)
#define YPOS_TYPEICONS    RowY(MENUITEM_TYPEICONS)
#define YPOS_SOUND        RowY(MENUITEM_SOUND)
#define YPOS_BUTTONMODE   RowY(MENUITEM_BUTTONMODE)
#define YPOS_FRAMETYPE    RowY(MENUITEM_FRAMETYPE)
#define YPOS_FRONTIER_AI  RowY(MENUITEM_FRONTIER_AI)
#define YPOS_BANNED       RowY(MENUITEM_BANNED_SPECIES)
#define TAG_OPTION_SCROLL_ARROWS 0x1340
STATIC_ASSERT(OPTION_VISIBLE_ROWS * OPTION_ROW_HEIGHT <= 14 * 8, OptionRowsFitWindow);
STATIC_ASSERT(OPTION_VISIBLE_ROWS <= MENUITEM_COUNT, OptionVisibleRows);

static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void HighlightOptionMenuItem(u8 selection);
static u8 TextSpeed_ProcessInput(u8 selection);
static void TextSpeed_DrawChoices(u8 selection);
static u8 BattleScene_ProcessInput(u8 selection);
static void BattleScene_DrawChoices(u8 selection);
static void TypeIcons_DrawChoices(u8 selection);
static u8 BattleStyle_ProcessInput(u8 selection);
static void BattleStyle_DrawChoices(u8 selection);
static u8 Sound_ProcessInput(u8 selection);
static void Sound_DrawChoices(u8 selection);
static u8 FrameType_ProcessInput(u8 selection);
static void FrameType_DrawChoices(u8 selection);
static u8 ButtonMode_ProcessInput(u8 selection);
static void ButtonMode_DrawChoices(u8 selection);
static u8 FrontierAi_ProcessInput(u8 selection);
static void FrontierAi_DrawChoices(u8 selection);
static void BannedSpecies_DrawChoices(u8 selection);
static void DrawAllChoices(u8 taskId);
static bool32 ScrollToSelection(u8 selection);
static void DrawHeaderText(void);
static void DrawOptionMenuTexts(void);
static void DrawBgWindowFrames(void);

EWRAM_DATA static bool8 sArrowPressed = FALSE;
EWRAM_DATA static u16 sScroll = 0; // first row shown in the window

// The hub's difficulty machine settings, easiest first. The values match VAR_FRONTIER_AI_LEVEL.
static const u16 sFrontierAiLevels[] =
{
    FRONTIER_AI_LEVEL_EASY,
    FRONTIER_AI_LEVEL_NORMAL,
    FRONTIER_AI_LEVEL_HARD,
    FRONTIER_AI_LEVEL_STRONGEST,
};

static const u8 gText_Option[]             = _("せってい");
static const u8 gText_TextSpeedSlow[]      = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}おそい");
static const u8 gText_TextSpeedMid[]       = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}ふつう");
static const u8 gText_TextSpeedFast[]      = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}はやい");
static const u8 gText_BattleSceneOn[]      = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}あり");
static const u8 gText_BattleSceneOff[]     = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}なし");
static const u8 gText_BattleStyleShift[]   = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}いれかえ");
static const u8 gText_BattleStyleSet[]     = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}かちぬき");
static const u8 gText_SoundMono[]          = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}モノラル");
static const u8 gText_SoundStereo[]        = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}ステレオ");
static const u8 gText_FrameType[]          = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}タイプ");
static const u8 gText_FrameTypeNumber[]    = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}");
static const u8 gText_ButtonTypeNormal[]   = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}ノーマル");
static const u8 gText_ButtonTypeLR[]       = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}LR");
static const u8 gText_ButtonTypeLEqualsA[] = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}L=A");
static const u8 *const sText_FrontierAiLevels[] =
{
    COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}かんたん"),
    COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}いつもの"),
    COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}むずかしい"),
    COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}さいきょう"),
};
STATIC_ASSERT(ARRAY_COUNT(sText_FrontierAiLevels) == ARRAY_COUNT(sFrontierAiLevels), FrontierAiLevelNames);

// Window y of a row, after scrolling. Rows outside 0 to OPTION_VISIBLE_ROWS - 1 are not drawn.
static s32 RowY(u32 item)
{
    return ((s32)item - sScroll) * OPTION_ROW_HEIGHT;
}

static u32 GetFrontierAiIndex(void)
{
    u16 level = VarGet(VAR_FRONTIER_AI_LEVEL);

    for (u32 i = 0; i < ARRAY_COUNT(sFrontierAiLevels); i++)
    {
        if (sFrontierAiLevels[i] == level)
            return i;
    }
    return 1; // unknown values play like FRONTIER_AI_LEVEL_NORMAL
}
static const u8 gText_BannedAllow[]        = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}みとめる");
static const u8 gText_BannedForbid[]       = _("{COLOR GREEN}{SHADOW LIGHT_GREEN}みとめない");

static const u16 sOptionMenuText_Pal[] = INCGFX_U16("graphics/interface/option_menu_text.pal", ".gbapal");
// note: this is only used in the Japanese release
static const u8 sEqualSignGfx[] = INCGFX_U8("graphics/interface/option_menu_equals_sign.png", ".4bpp");

static const u8 *const sOptionMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = COMPOUND_STRING("はなしの はやさ"),
    [MENUITEM_BATTLESCENE] = COMPOUND_STRING("せんとうアニメ"),
    [MENUITEM_BATTLESTYLE] = COMPOUND_STRING("しあいの ルール"),
    [MENUITEM_TYPEICONS]   = COMPOUND_STRING("{JPN}タイプひょうじ"),
    [MENUITEM_SOUND]       = COMPOUND_STRING("サウンド"),
    [MENUITEM_BUTTONMODE]  = COMPOUND_STRING("ボタンモード"),
    [MENUITEM_FRAMETYPE]   = COMPOUND_STRING("ウィンドウ"),
    [MENUITEM_FRONTIER_AI] = COMPOUND_STRING("AIの つよさ"),
    [MENUITEM_BANNED_SPECIES] = COMPOUND_STRING("きんしポケモン"),
    [MENUITEM_CANCEL]      = COMPOUND_STRING("もどる"),
};

static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    [WIN_HEADER] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    [WIN_OPTIONS] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 5,
        .width = 26,
        .height = 14,
        .paletteNum = 1,
        .baseBlock = 0x36
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    }
};

static const u16 sOptionMenuBg_Pal[] = {RGB(17, 18, 31)};

static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_InitOptionMenu(void)
{
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        sScroll = 0;
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        ChangeBgX(2, 0, BG_COORD_SET);
        ChangeBgY(2, 0, BG_COORD_SET);
        ChangeBgX(3, 0, BG_COORD_SET);
        ChangeBgY(3, 0, BG_COORD_SET);
        InitWindows(sOptionMenuWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_DARKEN);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sOptionMenuBg_Pal, BG_PLTT_ID(0), sizeof(sOptionMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sOptionMenuText_Pal, BG_PLTT_ID(1), sizeof(sOptionMenuText_Pal));
        gMain.state++;
        break;
    case 6:
        PutWindowTilemap(WIN_HEADER);
        DrawHeaderText();
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_OPTIONS);
        DrawOptionMenuTexts();
        gMain.state++;
    case 9:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 10:
    {
        u8 taskId = CreateTask(Task_OptionMenuFadeIn, 0);

        gTasks[taskId].tMenuSelection = 0;
        gTasks[taskId].tTextSpeed = gSaveBlock2Ptr->optionsTextSpeed;
        gTasks[taskId].tBattleSceneOff = gSaveBlock2Ptr->optionsBattleSceneOff;
        gTasks[taskId].tBattleStyle = gSaveBlock2Ptr->optionsBattleStyle;
        gTasks[taskId].tSound = gSaveBlock2Ptr->optionsSound;
        gTasks[taskId].tButtonMode = gSaveBlock2Ptr->optionsButtonMode;
        gTasks[taskId].tWindowFrameType = gSaveBlock2Ptr->optionsWindowFrameType;
        gTasks[taskId].tTypeIconsOff = FlagGet(FLAG_HIDE_BATTLE_TYPE_ICONS);
        gTasks[taskId].tFrontierAi = GetFrontierAiIndex();
        gTasks[taskId].tAllowBanned = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);

        DrawAllChoices(taskId);
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
        gTasks[taskId].tScrollArrows = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, DISPLAY_WIDTH / 2,
            OPTION_WINDOW_TOP - 6, OPTION_WINDOW_TOP + OPTION_VISIBLE_ROWS * OPTION_ROW_HEIGHT + 6,
            MENUITEM_COUNT - OPTION_VISIBLE_ROWS, TAG_OPTION_SCROLL_ARROWS, TAG_OPTION_SCROLL_ARROWS, &sScroll);

        CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
        gMain.state++;
        break;
    }
    case 11:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput;
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        if (gTasks[taskId].tMenuSelection == MENUITEM_CANCEL)
            gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP | DPAD_DOWN))
    {
        if (JOY_NEW(DPAD_UP))
            gTasks[taskId].tMenuSelection = gTasks[taskId].tMenuSelection > 0 ? gTasks[taskId].tMenuSelection - 1 : MENUITEM_CANCEL;
        else
            gTasks[taskId].tMenuSelection = gTasks[taskId].tMenuSelection < MENUITEM_CANCEL ? gTasks[taskId].tMenuSelection + 1 : 0;
        if (ScrollToSelection(gTasks[taskId].tMenuSelection))
        {
            DrawOptionMenuTexts();
            DrawAllChoices(taskId);
            CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
        }
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else
    {
        u8 previousOption;

        switch (gTasks[taskId].tMenuSelection)
        {
        case MENUITEM_TEXTSPEED:
            previousOption = gTasks[taskId].tTextSpeed;
            gTasks[taskId].tTextSpeed = TextSpeed_ProcessInput(gTasks[taskId].tTextSpeed);

            if (previousOption != gTasks[taskId].tTextSpeed)
                TextSpeed_DrawChoices(gTasks[taskId].tTextSpeed);
            break;
        case MENUITEM_BATTLESCENE:
            previousOption = gTasks[taskId].tBattleSceneOff;
            gTasks[taskId].tBattleSceneOff = BattleScene_ProcessInput(gTasks[taskId].tBattleSceneOff);

            if (previousOption != gTasks[taskId].tBattleSceneOff)
                BattleScene_DrawChoices(gTasks[taskId].tBattleSceneOff);
            break;
        case MENUITEM_BATTLESTYLE:
            previousOption = gTasks[taskId].tBattleStyle;
            gTasks[taskId].tBattleStyle = BattleStyle_ProcessInput(gTasks[taskId].tBattleStyle);

            if (previousOption != gTasks[taskId].tBattleStyle)
                BattleStyle_DrawChoices(gTasks[taskId].tBattleStyle);
            break;
        case MENUITEM_SOUND:
            previousOption = gTasks[taskId].tSound;
            gTasks[taskId].tSound = Sound_ProcessInput(gTasks[taskId].tSound);

            if (previousOption != gTasks[taskId].tSound)
                Sound_DrawChoices(gTasks[taskId].tSound);
            break;
        case MENUITEM_TYPEICONS:
            previousOption = gTasks[taskId].tTypeIconsOff;
            gTasks[taskId].tTypeIconsOff = BattleScene_ProcessInput(gTasks[taskId].tTypeIconsOff);

            if (previousOption != gTasks[taskId].tTypeIconsOff)
                TypeIcons_DrawChoices(gTasks[taskId].tTypeIconsOff);
            break;
        case MENUITEM_BUTTONMODE:
            previousOption = gTasks[taskId].tButtonMode;
            gTasks[taskId].tButtonMode = ButtonMode_ProcessInput(gTasks[taskId].tButtonMode);

            if (previousOption != gTasks[taskId].tButtonMode)
                ButtonMode_DrawChoices(gTasks[taskId].tButtonMode);
            break;
        case MENUITEM_FRAMETYPE:
            previousOption = gTasks[taskId].tWindowFrameType;
            gTasks[taskId].tWindowFrameType = FrameType_ProcessInput(gTasks[taskId].tWindowFrameType);

            if (previousOption != gTasks[taskId].tWindowFrameType)
                FrameType_DrawChoices(gTasks[taskId].tWindowFrameType);
            break;
        case MENUITEM_FRONTIER_AI:
            previousOption = gTasks[taskId].tFrontierAi;
            gTasks[taskId].tFrontierAi = FrontierAi_ProcessInput(gTasks[taskId].tFrontierAi);

            if (previousOption != gTasks[taskId].tFrontierAi)
                FrontierAi_DrawChoices(gTasks[taskId].tFrontierAi);
            break;
        case MENUITEM_BANNED_SPECIES:
            previousOption = gTasks[taskId].tAllowBanned;
            gTasks[taskId].tAllowBanned = BattleScene_ProcessInput(gTasks[taskId].tAllowBanned);

            if (previousOption != gTasks[taskId].tAllowBanned)
                BannedSpecies_DrawChoices(gTasks[taskId].tAllowBanned);
            break;
        default:
            return;
        }

        if (sArrowPressed)
        {
            sArrowPressed = FALSE;
            CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
        }
    }
}

static void Task_OptionMenuSave(u8 taskId)
{
    gSaveBlock2Ptr->optionsTextSpeed = gTasks[taskId].tTextSpeed;
    gSaveBlock2Ptr->optionsBattleSceneOff = gTasks[taskId].tBattleSceneOff;
    gSaveBlock2Ptr->optionsBattleStyle = gTasks[taskId].tBattleStyle;
    gSaveBlock2Ptr->optionsSound = gTasks[taskId].tSound;
    gSaveBlock2Ptr->optionsButtonMode = gTasks[taskId].tButtonMode;
    gSaveBlock2Ptr->optionsWindowFrameType = gTasks[taskId].tWindowFrameType;
    if (gTasks[taskId].tTypeIconsOff)
        FlagSet(FLAG_HIDE_BATTLE_TYPE_ICONS);
    else
        FlagClear(FLAG_HIDE_BATTLE_TYPE_ICONS);
    // The same settings as the hub's difficulty machine and banned-species clerk.
    VarSet(VAR_FRONTIER_AI_LEVEL, sFrontierAiLevels[gTasks[taskId].tFrontierAi]);
    if (gTasks[taskId].tAllowBanned)
        FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    else
        FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);

    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (gTasks[taskId].tScrollArrows != TASK_NONE)
            RemoveScrollIndicatorArrowPair(gTasks[taskId].tScrollArrows);
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void HighlightOptionMenuItem(u8 index)
{
    s32 y = OPTION_WINDOW_TOP + RowY(index);

    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(16, DISPLAY_WIDTH - 16));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(y, y + OPTION_ROW_HEIGHT));
}

// Moves the window so the selected row is shown. Returns whether it moved.
static bool32 ScrollToSelection(u8 selection)
{
    u16 scroll = sScroll;

    if (selection < scroll)
        scroll = selection;
    else if (selection >= scroll + OPTION_VISIBLE_ROWS)
        scroll = selection - OPTION_VISIBLE_ROWS + 1;
    if (scroll == sScroll)
        return FALSE;
    sScroll = scroll;
    return TRUE;
}

static void DrawAllChoices(u8 taskId)
{
    TextSpeed_DrawChoices(gTasks[taskId].tTextSpeed);
    BattleScene_DrawChoices(gTasks[taskId].tBattleSceneOff);
    BattleStyle_DrawChoices(gTasks[taskId].tBattleStyle);
    TypeIcons_DrawChoices(gTasks[taskId].tTypeIconsOff);
    Sound_DrawChoices(gTasks[taskId].tSound);
    ButtonMode_DrawChoices(gTasks[taskId].tButtonMode);
    FrameType_DrawChoices(gTasks[taskId].tWindowFrameType);
    FrontierAi_DrawChoices(gTasks[taskId].tFrontierAi);
    BannedSpecies_DrawChoices(gTasks[taskId].tAllowBanned);
}

static void DrawOptionMenuChoice(const u8 *text, u8 x, s32 y, u8 style)
{
    u8 dst[24];
    u16 i;

    if (y < 0 || y >= OPTION_VISIBLE_ROWS * OPTION_ROW_HEIGHT)
        return; // scrolled out of the window

    for (i = 0; *text != EOS && i < ARRAY_COUNT(dst) - 1; i++)
        dst[i] = *(text++);

    if (style != 0)
    {
        dst[2] = TEXT_COLOR_RED;
        dst[5] = TEXT_COLOR_LIGHT_RED;
    }

    dst[i] = EOS;
    AddTextPrinterParameterized(WIN_OPTIONS, OPTION_FONT, dst, x, y + 1, TEXT_SKIP_DRAW, NULL);
}

static u8 TextSpeed_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void TextSpeed_DrawChoices(u8 selection)
{
    u8 styles[3];
    s32 widthSlow, widthMid, widthFast, xMid;

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_TextSpeedSlow, 104, YPOS_TEXTSPEED, styles[0]);

    widthSlow = GetStringWidth(OPTION_FONT, gText_TextSpeedSlow, 0);
    widthMid = GetStringWidth(OPTION_FONT, gText_TextSpeedMid, 0);
    widthFast = GetStringWidth(OPTION_FONT, gText_TextSpeedFast, 0);

    widthMid -= 94;
    xMid = (widthSlow - widthMid - widthFast) / 2 + 104;
    DrawOptionMenuChoice(gText_TextSpeedMid, xMid, YPOS_TEXTSPEED, styles[1]);

    DrawOptionMenuChoice(gText_TextSpeedFast, GetStringRightAlignXOffset(OPTION_FONT, gText_TextSpeedFast, 198), YPOS_TEXTSPEED, styles[2]);
}

static u8 BattleScene_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void BattleScene_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOn, 104, YPOS_BATTLESCENE, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(OPTION_FONT, gText_BattleSceneOff, 198), YPOS_BATTLESCENE, styles[1]);
}

static void TypeIcons_DrawChoices(u8 selection)
{
    DrawOptionMenuChoice(gText_BattleSceneOn, 104, YPOS_TYPEICONS, selection == 0);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(OPTION_FONT, gText_BattleSceneOff, 198), YPOS_TYPEICONS, selection != 0);
}

static u8 BattleStyle_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void BattleStyle_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleStyleShift, 104, YPOS_BATTLESTYLE, styles[0]);
    DrawOptionMenuChoice(gText_BattleStyleSet, GetStringRightAlignXOffset(OPTION_FONT, gText_BattleStyleSet, 198), YPOS_BATTLESTYLE, styles[1]);
}

static u8 Sound_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        SetPokemonCryStereo(selection);
        sArrowPressed = TRUE;
    }

    return selection;
}

static void Sound_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_SoundMono, 104, YPOS_SOUND, styles[0]);
    DrawOptionMenuChoice(gText_SoundStereo, GetStringRightAlignXOffset(OPTION_FONT, gText_SoundStereo, 198), YPOS_SOUND, styles[1]);
}

static u8 FrameType_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection < WINDOW_FRAMES_COUNT - 1)
            selection++;
        else
            selection = 0;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = WINDOW_FRAMES_COUNT - 1;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        sArrowPressed = TRUE;
    }
    return selection;
}

static void FrameType_DrawChoices(u8 selection)
{
    u8 text[16] = {EOS};
    u8 n = selection + 1;
    u16 i;

    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];

    // Convert a number to decimal string
    if (n / 10 != 0)
    {
        text[i] = n / 10 + CHAR_0;
        i++;
        text[i] = n % 10 + CHAR_0;
        i++;
    }
    else
    {
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = CHAR_SPACER;
        i++;
    }

    text[i] = EOS;

    DrawOptionMenuChoice(gText_FrameType, 104, YPOS_FRAMETYPE, 0);
    DrawOptionMenuChoice(text, 128, YPOS_FRAMETYPE, 1);
}

static u8 ButtonMode_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void ButtonMode_DrawChoices(u8 selection)
{
    s32 widthNormal, widthLR, widthLA, xLR;
    u8 styles[3];

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_ButtonTypeNormal, 104, YPOS_BUTTONMODE, styles[0]);

    widthNormal = GetStringWidth(OPTION_FONT, gText_ButtonTypeNormal, 0);
    widthLR = GetStringWidth(OPTION_FONT, gText_ButtonTypeLR, 0);
    widthLA = GetStringWidth(OPTION_FONT, gText_ButtonTypeLEqualsA, 0);

    widthLR -= 94;
    xLR = (widthNormal - widthLR - widthLA) / 2 + 104;
    DrawOptionMenuChoice(gText_ButtonTypeLR, xLR, YPOS_BUTTONMODE, styles[1]);

    DrawOptionMenuChoice(gText_ButtonTypeLEqualsA, GetStringRightAlignXOffset(OPTION_FONT, gText_ButtonTypeLEqualsA, 198), YPOS_BUTTONMODE, styles[2]);
}

static u8 FrontierAi_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        selection = selection < ARRAY_COUNT(sFrontierAiLevels) - 1 ? selection + 1 : 0;
        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        selection = selection != 0 ? selection - 1 : ARRAY_COUNT(sFrontierAiLevels) - 1;
        sArrowPressed = TRUE;
    }
    return selection;
}

// Four levels do not fit side by side, so the chosen one is shown between arrows like the window frame.
static void FrontierAi_DrawChoices(u8 selection)
{
    s32 y = YPOS_FRONTIER_AI;

    if (y < 0 || y >= OPTION_VISIBLE_ROWS * OPTION_ROW_HEIGHT)
        return;
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 104, y, 198 - 104, OPTION_ROW_HEIGHT);
    DrawOptionMenuChoice(COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}{LEFT_ARROW}"), 104, y, 0);
    DrawOptionMenuChoice(sText_FrontierAiLevels[selection],
        GetStringCenterAlignXOffset(OPTION_FONT, sText_FrontierAiLevels[selection], 198 - 104) + 104, y, 1);
    DrawOptionMenuChoice(COMPOUND_STRING("{COLOR GREEN}{SHADOW LIGHT_GREEN}{RIGHT_ARROW}"),
        GetStringRightAlignXOffset(OPTION_FONT, COMPOUND_STRING("{RIGHT_ARROW}"), 198), y, 0);
}

static void BannedSpecies_DrawChoices(u8 selection)
{
    DrawOptionMenuChoice(gText_BannedAllow, 104, YPOS_BANNED, selection != 0);
    DrawOptionMenuChoice(gText_BannedForbid, GetStringRightAlignXOffset(OPTION_FONT, gText_BannedForbid, 198), YPOS_BANNED, selection == 0);
}

static void DrawHeaderText(void)
{
    FillWindowPixelBuffer(WIN_HEADER, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_Option, 8, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_HEADER, COPYWIN_FULL);
}

static void DrawOptionMenuTexts(void)
{
    u8 i;

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (i = sScroll; i < MENUITEM_COUNT && i < sScroll + OPTION_VISIBLE_ROWS; i++)
        AddTextPrinterParameterized(WIN_OPTIONS, OPTION_FONT, sOptionMenuItemsNames[i], 8, RowY(i) + 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

#define TILE_TOP_CORNER_L 0x1A2
#define TILE_TOP_EDGE     0x1A3
#define TILE_TOP_CORNER_R 0x1A4
#define TILE_LEFT_EDGE    0x1A5
#define TILE_RIGHT_EDGE   0x1A7
#define TILE_BOT_CORNER_L 0x1A8
#define TILE_BOT_EDGE     0x1A9
#define TILE_BOT_CORNER_R 0x1AA

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw title window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  0, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1,  3,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2,  3, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28,  3,  1,  1,  7);

    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  4, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}

#if TESTING
static void Test_KeepFrontierSettings(u8 taskId)
{
    gTasks[taskId].tFrontierAi = GetFrontierAiIndex();
    gTasks[taskId].tAllowBanned = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    gTasks[taskId].tScrollArrows = TASK_NONE;
}

void Test_OptionMenuSaveTypeIcons(u8 taskId, bool32 off)
{
    Test_KeepFrontierSettings(taskId);
    gTasks[taskId].tTypeIconsOff = off;
    Task_OptionMenuSave(taskId);
}

// aiIndex counts from the easiest level, as the menu shows them.
void Test_OptionMenuSaveFrontier(u8 taskId, u32 aiIndex, bool32 allowBanned)
{
    Test_KeepFrontierSettings(taskId);
    gTasks[taskId].tTypeIconsOff = FlagGet(FLAG_HIDE_BATTLE_TYPE_ICONS);
    gTasks[taskId].tFrontierAi = aiIndex;
    gTasks[taskId].tAllowBanned = allowBanned;
    Task_OptionMenuSave(taskId);
}

u32 Test_OptionMenuReadFrontierAi(void)
{
    return GetFrontierAiIndex();
}

// The first row shown after moving the cursor from the top to `selection` one row at a time.
u32 Test_OptionMenuScrollFor(u32 selection)
{
    sScroll = 0;
    for (u32 i = 0; i <= selection; i++)
        ScrollToSelection(i);
    return sScroll;
}

u32 Test_OptionMenuRowCount(void)
{
    return MENUITEM_COUNT;
}

u32 Test_OptionMenuLastRowBottom(void)
{
    return (OPTION_VISIBLE_ROWS - 1) * OPTION_ROW_HEIGHT + 1 + GetFontAttribute(OPTION_FONT, FONTATTR_MAX_LETTER_HEIGHT);
}
#endif
