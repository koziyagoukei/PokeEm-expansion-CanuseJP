#include "global.h"
#include "ultra_help.h"
#include "battle_main.h"
#include "bg.h"
#include "config/battle.h"
#include "config_changes.h"
#include "data.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "move.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "pokemon_storage_sort.h"
#include "scanline_effect.h"
#include "sound.h"
#include "strings.h"
#include "string_util.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/battle_set_effect.h"

#define INFO_VIEWER_ALT_FRAME_PALETTE_NUM 14
#define INFO_VIEWER_HISTORY_SIZE 8
#define INFO_VIEWER_VISIBLE_CHOICES 6
#define INFO_VIEWER_SPECIES_PER_PAGE 10

enum
{
    WIN_INFO_VIEWER,
};

enum
{
    INFO_VIEWER_MODE_PAGE,
    INFO_VIEWER_MODE_CHOICE,
};

struct InfoViewerHistory
{
    u16 infoId;
    u16 page;
    u8 choice;
    u8 mode;
};

static void DrawInfoViewerBattleStatus(void);
static void DrawInfoViewerMoveReference(void);

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
static const struct InfoViewerStyle *GetInfoViewerStyle(void);
static void LoadInfoViewerWindowGfx(void);
static bool32 InfoViewerHasChoices(void);
static void SwitchInfoViewerEntry(u16 infoId, bool32 saveHistory);
static void PushInfoViewerHistory(u16 infoId);
static void ReturnToPreviousInfoViewerEntry(u8 taskId);
static void SelectInfoViewerChoice(u8 taskId);
static void DrawInfoViewerBannedSpecies(void);

EWRAM_DATA static u8 *sInfoViewerTilemapBuffer = NULL;
EWRAM_DATA static const struct InfoViewerEntry *sInfoViewerEntry = NULL;
EWRAM_DATA static u16 sInfoViewerPage = 0;
EWRAM_DATA static u16 *sInfoViewerMoveOrder = NULL;
EWRAM_DATA static u16 sInfoViewerMoveCount = 0;
EWRAM_DATA static bool8 sInfoViewerShowMoveEffects = FALSE;
EWRAM_DATA static u8 sInfoViewerMode = INFO_VIEWER_MODE_PAGE;
EWRAM_DATA static u8 sInfoViewerChoiceCursor = 0;
EWRAM_DATA static u16 sInfoViewerInfoId = INFO_FRONTIER_RULES;
EWRAM_DATA static struct InfoViewerHistory sInfoViewerHistory[INFO_VIEWER_HISTORY_SIZE] = {0};
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
    if (sInfoViewerEntry->pageCount == 0)
        sInfoViewerMode = INFO_VIEWER_MODE_CHOICE;
    sInfoViewerHistoryDepth = 0;
    sInfoViewerExitCallback = exitCallback != NULL ? exitCallback : CB2_ReturnToFieldContinueScriptPlayMapMusic;
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
        if (sInfoViewerTilemapBuffer == NULL)
        {
            SetMainCallback2(sInfoViewerExitCallback);
            return;
        }
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
            if (sInfoViewerEntry->pageCount == 0)
                ReturnToPreviousInfoViewerEntry(taskId);
            else
            {
                sInfoViewerMode = INFO_VIEWER_MODE_PAGE;
                DrawInfoViewerPage();
            }
        }
    }
    else if (sInfoViewerInfoId == INFO_TIPS_MOVE_REFERENCE)
    {
        s32 delta = 0;

        if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            ReturnToPreviousInfoViewerEntry(taskId);
            return;
        }
        if (JOY_NEW(A_BUTTON))
        {
            sInfoViewerShowMoveEffects = !sInfoViewerShowMoveEffects;
        }
        else if (JOY_REPEAT(DPAD_UP))
            delta = -10;
        else if (JOY_REPEAT(DPAD_DOWN))
            delta = 10;
        else if (JOY_NEW(L_BUTTON))
            delta = -50;
        else if (JOY_NEW(R_BUTTON))
            delta = 50;
        else if (JOY_REPEAT(DPAD_LEFT))
            delta = -1;
        else if (JOY_REPEAT(DPAD_RIGHT))
            delta = 1;
        else
            return;

        sInfoViewerPage = min(max((s32)sInfoViewerPage + delta, 0), (s32)GetInfoViewerPageCount(sInfoViewerInfoId) - 1);
        PlaySE(SE_SELECT);
        DrawInfoViewerPage();
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (sInfoViewerPage + 1 < GetInfoViewerPageCount(sInfoViewerInfoId))
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
            ReturnToPreviousInfoViewerEntry(taskId);
        }
    }
    else if (JOY_NEW(R_BUTTON | DPAD_RIGHT))
    {
        if (sInfoViewerPage + 1 < GetInfoViewerPageCount(sInfoViewerInfoId))
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
            ReturnToPreviousInfoViewerEntry(taskId);
        }
        else
        {
            sInfoViewerPage--;
            DrawInfoViewerPage();
        }
    }
    else if (JOY_NEW(L_BUTTON | DPAD_LEFT))
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
        UnsetBgTilemapBuffer(0);
        Free(sInfoViewerTilemapBuffer);
        sInfoViewerTilemapBuffer = NULL;
        Free(sInfoViewerMoveOrder);
        sInfoViewerMoveOrder = NULL;
        sInfoViewerMoveCount = 0;
        sInfoViewerShowMoveEffects = FALSE;
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

    if (sInfoViewerMode == INFO_VIEWER_MODE_CHOICE)
    {
        DrawInfoViewerChoices();
        return;
    }

    if (sInfoViewerPage >= GetInfoViewerPageCount(sInfoViewerInfoId))
        sInfoViewerPage = 0;

    DrawStdFrameWithCustomTileAndPalette(WIN_INFO_VIEWER, FALSE, STD_WINDOW_BASE_TILE_NUM, style->framePaletteNum);
    FillWindowPixelBuffer(WIN_INFO_VIEWER, style->windowFill);
    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_NORMAL, 8, 6, 0, 0, style->textColors, TEXT_SKIP_DRAW, sInfoViewerEntry->title);
    if (sInfoViewerEntry->drawPage != NULL)
        sInfoViewerEntry->drawPage();
    else if (sInfoViewerInfoId == INFO_BANNED_MONS && sInfoViewerPage >= sInfoViewerEntry->pageCount)
        DrawInfoViewerBannedSpecies();
    else
        AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, 8, 28, 0, 2, style->textColors, TEXT_SKIP_DRAW, sInfoViewerEntry->pages[sInfoViewerPage]);
    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, 8, 132, 0, 0, style->textColors, TEXT_SKIP_DRAW,
        sInfoViewerInfoId == INFO_TIPS_MOVE_REFERENCE
            ? COMPOUND_STRING("{JPN}A:詳細 B:戻る {UP_ARROW}{DOWN_ARROW}:10 LR:50")
            : COMPOUND_STRING("{JPN}A:次 B:戻る"));
    DrawInfoViewerPageNumber();
    CopyWindowToVram(WIN_INFO_VIEWER, COPYWIN_FULL);
}

static void DrawInfoViewerChoices(void)
{
    const struct InfoViewerStyle *style = GetInfoViewerStyle();
    u8 i;
    u8 first;

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
    first = sInfoViewerChoiceCursor / INFO_VIEWER_VISIBLE_CHOICES * INFO_VIEWER_VISIBLE_CHOICES;
    for (i = first; i < min(first + INFO_VIEWER_VISIBLE_CHOICES, sInfoViewerEntry->choiceCount); i++)
    {
        u8 y = 30 + (i - first) * 16;

        if (i == sInfoViewerChoiceCursor)
            AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_NORMAL, 8, y, 0, 0, style->textColors, TEXT_SKIP_DRAW, gText_SelectorArrow3);
        AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, 24, y + 1, 0, 0, style->textColors, TEXT_SKIP_DRAW, sInfoViewerEntry->choices[i].text);
    }

    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, 8, 132, 0, 0, style->textColors, TEXT_SKIP_DRAW,
        COMPOUND_STRING("{JPN}A:決定 B:戻る"));
    DrawInfoViewerPageNumber();
    CopyWindowToVram(WIN_INFO_VIEWER, COPYWIN_FULL);
}

static void DrawInfoViewerPageNumber(void)
{
    const struct InfoViewerStyle *style = GetInfoViewerStyle();
    u8 *txtPtr = gStringVar1;
    s32 x;

    u32 current = sInfoViewerMode == INFO_VIEWER_MODE_CHOICE ? sInfoViewerChoiceCursor + 1 : sInfoViewerPage + 1;
    u32 total = sInfoViewerMode == INFO_VIEWER_MODE_CHOICE ? sInfoViewerEntry->choiceCount : GetInfoViewerPageCount(sInfoViewerInfoId);
    u32 digits = total >= 1000 ? 4 : total >= 100 ? 3 : 2;

    txtPtr = ConvertIntToDecimalStringN(txtPtr, current, STR_CONV_MODE_LEFT_ALIGN, digits);
    txtPtr = StringCopy(txtPtr, COMPOUND_STRING("/"));
    ConvertIntToDecimalStringN(txtPtr, total, STR_CONV_MODE_LEFT_ALIGN, digits);

    x = GetStringRightAlignXOffset(FONT_SMALL, gStringVar1, 212);
    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, x, 132, 0, 0, style->textColors, TEXT_SKIP_DRAW, gStringVar1);
}

const struct InfoViewerEntry *GetInfoViewerEntry(u16 infoId)
{
    const struct InfoViewerEntry *entry;

    if (infoId >= INFO_COUNT)
        infoId = INFO_FRONTIER_RULES;

    entry = &sInfoViewerEntries[infoId];
    if (entry->title == NULL || (((entry->pages == NULL && entry->drawPage == NULL) || entry->pageCount == 0)
        && (entry->choices == NULL || entry->choiceCount == 0)))
        entry = &sInfoViewerEntries[INFO_FRONTIER_RULES];

    return entry;
}

static bool32 IsInfoViewerBannedSpecies(enum Species species)
{
    if (!IsSpeciesEnabled(species) || !gSpeciesInfo[species].isFrontierBanned)
        return FALSE;
    enum Species base = GET_BASE_SPECIES_ID(species);
    return base == species || !gSpeciesInfo[base].isFrontierBanned;
}

u16 GetInfoViewerBannedSpecies(u32 index)
{
    // Group forms under their base species without depending on caught flags
    // or the optional one-restricted-Pokemon rule.
    for (enum Species species = 1; species < NUM_SPECIES; species++)
    {
        if (!IsInfoViewerBannedSpecies(species))
            continue;
        if (index-- == 0)
            return species;
    }
    return SPECIES_NONE;
}

u32 GetInfoViewerPageCount(u16 infoId)
{
    u32 count = 0;
    if (infoId == INFO_TIPS_MOVE_REFERENCE)
        return max(1, sInfoViewerMoveCount);
    if (infoId != INFO_BANNED_MONS)
        return GetInfoViewerEntry(infoId)->pageCount;
    for (enum Species species = 1; species < NUM_SPECIES; species++)
        if (IsInfoViewerBannedSpecies(species))
            count++;
    return GetInfoViewerEntry(infoId)->pageCount + (count + INFO_VIEWER_SPECIES_PER_PAGE - 1) / INFO_VIEWER_SPECIES_PER_PAGE;
}

static void PrintInfoViewerDetail(const u8 *text, u32 y)
{
    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, 8, y, 0, 0,
        GetInfoViewerStyle()->textColors, TEXT_SKIP_DRAW, text);
}

static u8 *AppendDetailNumber(u8 *dest, const u8 *label, u32 number)
{
    return ConvertIntToDecimalStringN(StringCopy(dest, label), number, STR_CONV_MODE_LEFT_ALIGN, 3);
}

static void DrawInfoViewerBattleStatus(void)
{
    u32 sleepRules = GetConfig(B_SLEEP_TURNS);
    const u8 *paralysis = GetConfig(B_PARALYSIS_CHANCE) >= GEN_CHAMPIONS
        ? COMPOUND_STRING("{JPN}まひで 動けない確率: {ENG}12.5%")
        : COMPOUND_STRING("{JPN}まひで 動けない確率: {ENG}25%");
    u8 *end;

    PrintInfoViewerDetail(paralysis, 29);
    AppendDetailNumber(gStringVar4, COMPOUND_STRING("{JPN}まひの 素早さ: 通常の "), GetConfig(B_PARALYSIS_SPEED) >= GEN_7 ? 50 : 25);
    StringAppend(gStringVar4, COMPOUND_STRING("{ENG}%"));
    PrintInfoViewerDetail(gStringVar4, 44);

    if (sleepRules >= GEN_CHAMPIONS)
        PrintInfoViewerDetail(COMPOUND_STRING("{JPN}ねむり: 動けないのは 必ず1ターン"), 59);
    else
    {
        end = AppendDetailNumber(gStringVar4, COMPOUND_STRING("{JPN}ねむり: 動けないのは 1から"),
            sleepRules >= GEN_5 ? 3 : sleepRules >= GEN_3 ? 4 : 7);
        StringCopy(end, COMPOUND_STRING("ターン"));
        PrintInfoViewerDetail(gStringVar4, 59);
    }

    end = AppendDetailNumber(gStringVar4, COMPOUND_STRING("{JPN}こんらんで 自分を攻撃: "),
        GetConfig(B_CONFUSION_SELF_DMG_CHANCE) >= GEN_7 ? 33 : 50);
    StringCopy(end, COMPOUND_STRING("{ENG}%"));
    PrintInfoViewerDetail(gStringVar4, 74);
    PrintInfoViewerDetail(COMPOUND_STRING("{JPN}技ごとの 発生率やPPは わざ図鑑へ。"), 99);
}

static const u8 *GetReferenceMoveName(u16 move)
{
    return GetMoveName(move);
}

static bool32 PrepareInfoViewerMoveOrder(void)
{
    if (sInfoViewerMoveOrder != NULL)
        return TRUE;

    sInfoViewerMoveOrder = Alloc(MOVES_COUNT_ALL * sizeof(*sInfoViewerMoveOrder));
    if (sInfoViewerMoveOrder == NULL)
        return FALSE;

    for (u32 move = MOVE_NONE + 1; move < MOVES_COUNT_ALL; move++)
    {
        if (GetMoveName(move) != NULL && GetMoveEffect(move) != EFFECT_PLACEHOLDER)
            sInfoViewerMoveOrder[sInfoViewerMoveCount++] = move;
    }
    // The list remains usable in move-ID order if the temporary sort buffer is unavailable.
    SortIdsByKanaName(sInfoViewerMoveOrder, sInfoViewerMoveCount, GetReferenceMoveName);
    return TRUE;
}

static const u8 *GetReferenceEffectName(enum MoveEffect effect)
{
    switch (effect)
    {
    case MOVE_EFFECT_SLEEP:     return COMPOUND_STRING("ねむり");
    case MOVE_EFFECT_POISON:    return COMPOUND_STRING("どく");
    case MOVE_EFFECT_TOXIC:     return COMPOUND_STRING("もうどく");
    case MOVE_EFFECT_BURN:      return COMPOUND_STRING("やけど");
    case MOVE_EFFECT_FREEZE:    return COMPOUND_STRING("こおり");
    case MOVE_EFFECT_FROSTBITE: return COMPOUND_STRING("しもやけ");
    case MOVE_EFFECT_PARALYSIS: return COMPOUND_STRING("まひ");
    case MOVE_EFFECT_CONFUSION: return COMPOUND_STRING("こんらん");
    case MOVE_EFFECT_FLINCH:    return COMPOUND_STRING("ひるみ");
    case MOVE_EFFECT_STAT_PLUS:  return COMPOUND_STRING("能力アップ");
    case MOVE_EFFECT_STAT_MINUS: return COMPOUND_STRING("能力ダウン");
    default:                   return COMPOUND_STRING("特殊効果");
    }
}

static void DrawInfoViewerMoveReference(void)
{
    enum Move move;
    u8 *end;

    if (!PrepareInfoViewerMoveOrder() || sInfoViewerMoveCount == 0)
    {
        PrintInfoViewerDetail(COMPOUND_STRING("{JPN}わざ図鑑を 開けません。"), 29);
        return;
    }
    move = sInfoViewerMoveOrder[sInfoViewerPage];
    PrintInfoViewerDetail(GetMoveName(move), 28);

    if (sInfoViewerShowMoveEffects)
    {
        u32 count = GetMoveAdditionalEffectCount(move);

        PrintInfoViewerDetail(COMPOUND_STRING("{JPN}登録効果の 基本発生率"), 43);
        if (count == 0)
            PrintInfoViewerDetail(COMPOUND_STRING("{JPN}表示できる 確率つき効果は ありません。"), 60);
        for (u32 i = 0; i < count; i++)
        {
            const struct AdditionalEffect *effect = GetMoveAdditionalEffectById(move, i);

            end = StringCopy(gStringVar4, COMPOUND_STRING("{JPN}"));
            end = StringCopy(end, GetReferenceEffectName(effect->moveEffect));
            end = StringCopy(end, effect->self ? COMPOUND_STRING(" (自分): ") : COMPOUND_STRING(": "));
            if (effect->chance == 0)
                StringCopy(end, COMPOUND_STRING("確率判定なし"));
            else
                StringCopy(ConvertIntToDecimalStringN(end, effect->chance, STR_CONV_MODE_LEFT_ALIGN, 3), COMPOUND_STRING("{ENG}%"));
            PrintInfoViewerDetail(gStringVar4, 57 + (count > 5 ? 10 : 14) * i);
        }
        return;
    }

    end = StringCopy(gStringVar4, COMPOUND_STRING("{JPN}タイプ: "));
    end = StringCopy(end, gTypesInfo[GetMoveType(move)].name);
    AppendDetailNumber(end, COMPOUND_STRING("   PP: "), GetMovePP(move));
    PrintInfoViewerDetail(gStringVar4, 45);

    end = StringCopy(gStringVar4, COMPOUND_STRING("{JPN}威力: "));
    end = GetMovePower(move) < 2
        ? StringCopy(end, COMPOUND_STRING("--"))
        : ConvertIntToDecimalStringN(end, GetMovePower(move), STR_CONV_MODE_LEFT_ALIGN, 3);
    end = StringCopy(end, COMPOUND_STRING("   命中: "));
    if (GetMoveAccuracy(move) == 0)
        StringCopy(end, COMPOUND_STRING("--"));
    else
        StringCopy(ConvertIntToDecimalStringN(end, GetMoveAccuracy(move), STR_CONV_MODE_LEFT_ALIGN, 3), COMPOUND_STRING("{ENG}%"));
    PrintInfoViewerDetail(gStringVar4, 62);

    if (GetMoveDescription(move) != NULL)
        PrintInfoViewerDetail(GetMoveDescription(move), 82);
}

static void DrawInfoViewerBannedSpecies(void)
{
    const struct InfoViewerStyle *style = GetInfoViewerStyle();
    const u8 *status = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES)
        ? COMPOUND_STRING("{JPN}現在: 1匹まで 参加できます")
        : COMPOUND_STRING("{JPN}現在: 参加できません");
    u32 first = (sInfoViewerPage - sInfoViewerEntry->pageCount) * INFO_VIEWER_SPECIES_PER_PAGE;

    AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, 8, 28, 0, 0, style->textColors, TEXT_SKIP_DRAW, status);
    for (u32 i = 0; i < INFO_VIEWER_SPECIES_PER_PAGE; i++)
    {
        enum Species species = GetInfoViewerBannedSpecies(first + i);
        if (species == SPECIES_NONE)
            break;
        AddTextPrinterParameterized4(WIN_INFO_VIEWER, FONT_SMALL, 8 + (i % 2) * 104, 50 + (i / 2) * 15,
            0, 0, style->textColors, TEXT_SKIP_DRAW, GetSpeciesName(species));
    }
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
    sInfoViewerMode = sInfoViewerEntry->pageCount == 0 ? INFO_VIEWER_MODE_CHOICE : INFO_VIEWER_MODE_PAGE;
    LoadInfoViewerWindowGfx();
    DrawInfoViewerPage();
}

static void PushInfoViewerHistory(u16 infoId)
{
    u8 i;

    if (sInfoViewerHistoryDepth < INFO_VIEWER_HISTORY_SIZE)
    {
        sInfoViewerHistoryDepth++;
    }
    else
    {
        for (i = 1; i < INFO_VIEWER_HISTORY_SIZE; i++)
            sInfoViewerHistory[i - 1] = sInfoViewerHistory[i];
    }
    sInfoViewerHistory[sInfoViewerHistoryDepth - 1] = (struct InfoViewerHistory){
        .infoId = infoId, .page = sInfoViewerPage, .choice = sInfoViewerChoiceCursor, .mode = sInfoViewerMode,
    };
}

static void ReturnToPreviousInfoViewerEntry(u8 taskId)
{
    if (sInfoViewerHistoryDepth == 0)
    {
        gTasks[taskId].func = Task_InfoViewerBeginFadeOut;
    }
    else
    {
        struct InfoViewerHistory history = sInfoViewerHistory[--sInfoViewerHistoryDepth];
        SwitchInfoViewerEntry(history.infoId, FALSE);
        sInfoViewerPage = history.page;
        sInfoViewerChoiceCursor = history.choice;
        sInfoViewerMode = history.mode;
        DrawInfoViewerPage();
    }
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
        ReturnToPreviousInfoViewerEntry(taskId);
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
