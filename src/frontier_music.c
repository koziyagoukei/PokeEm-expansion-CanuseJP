#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "event_data.h"
#include "frontier_music.h"
#include "international_string_util.h"
#include "list_menu.h"
#include "malloc.h"
#include "map_name_popup.h"
#include "menu.h"
#include "menu_helpers.h"
#include "random.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "util.h"
#include "constants/songs.h"
#include "constants/trainers.h"

#define FRONTIER_MUSIC_MAGIC 0x464D5553
#define FRONTIER_MUSIC_VERSION 1

struct FrontierMusicTrack
{
    u16 song;
    u8 name[32];
};

// Append only: these indices are also the saved unlock-bit positions.
static const struct FrontierMusicTrack sTracks[] =
{
    {MUS_VS_TRAINER, _("{JPN}ホウエン トレーナー")},
    {MUS_VS_WILD, _("{JPN}ホウエン やせい")},
    {MUS_VS_GYM_LEADER, _("{JPN}ホウエン ジムリーダー")},
    {MUS_VS_RIVAL, _("{JPN}ホウエン ライバル")},
    {MUS_VS_ELITE_FOUR, _("{JPN}ホウエン してんのう")},
    {MUS_VS_CHAMPION, _("{JPN}ホウエン チャンピオン")},
    {MUS_VS_AQUA_MAGMA, _("{JPN}アクアだん マグマだん")},
    {MUS_VS_AQUA_MAGMA_LEADER, _("{JPN}アクア マグマリーダー")},
    {MUS_VS_REGI, _("{JPN}レジロックたち")},
    {MUS_VS_KYOGRE_GROUDON, _("{JPN}カイオーガ グラードン")},
    {MUS_VS_RAYQUAZA, _("{JPN}レックウザ")},
    {MUS_VS_MEW, _("{JPN}ミュウ")},
    {MUS_VS_FRONTIER_BRAIN, _("{JPN}フロンティアブレーン")},
    {MUS_RG_VS_TRAINER, _("{JPN}カントー トレーナー")},
    {MUS_RG_VS_WILD, _("{JPN}カントー やせい")},
    {MUS_RG_VS_GYM_LEADER, _("{JPN}カントー ジムリーダー")},
    {MUS_RG_VS_CHAMPION, _("{JPN}カントー チャンピオン")},
    {MUS_RG_VS_LEGEND, _("{JPN}カントー でんせつ")},
    {MUS_RG_VS_MEWTWO, _("{JPN}ミュウツー")},
    {MUS_RG_VS_DEOXYS, _("{JPN}デオキシス")},
    {MUS_C_VS_LEGEND_BEAST, _("{JPN}スイクンたち")},
};

static const u8 sText_Cancel[] = _("{JPN}やめる");
static const u8 sText_Default[] = _("{JPN}もとの BGM");
static const u8 sText_Random[] = _("{JPN}かいほうずみから ランダム");
static const u8 sText_Price[] = _("{ENG}5BP");
static const u8 sText_Unlocked[] = _("{JPN}ずみ");
static const u8 sText_Selected[] = _("{JPN}せってい");

struct FrontierMusicCatalog
{
    struct ListMenuItem items[FRONTIER_MUSIC_TRACK_COUNT + 1];
    u16 scroll;
    u16 row;
    u8 windowId;
    u8 listTaskId;
    u8 arrowsTaskId;
};

static EWRAM_DATA struct FrontierMusicCatalog *sCatalog = NULL;

STATIC_ASSERT(ARRAY_COUNT(sTracks) == FRONTIER_MUSIC_TRACK_COUNT, FrontierMusicCatalogSize);
STATIC_ASSERT(FRONTIER_MUSIC_TRACK_COUNT < 32, FrontierMusicUnlockBits);
STATIC_ASSERT(offsetof(struct SaveBlock3, frontierMusic) == 152, FrontierMusicAppendOnlySave);
STATIC_ASSERT(sizeof(struct FrontierMusicSave) == 16, FrontierMusicSaveSize);

static u32 GetTrackIndex(u16 song)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sTracks); i++)
        if (sTracks[i].song == song)
            return i;
    return ARRAY_COUNT(sTracks);
}

static void UpdateChecksum(void)
{
    struct FrontierMusicSave *save = &gSaveBlock3Ptr->frontierMusic;
    save->checksum = CalcCRC16((const u8 *)save, offsetof(struct FrontierMusicSave, checksum));
}

void FrontierMusic_EnsureSaveData(void)
{
    struct FrontierMusicSave *save = &gSaveBlock3Ptr->frontierMusic;
    u32 index = GetTrackIndex(save->selectedSong);

    if (save->magic != FRONTIER_MUSIC_MAGIC
     || save->version != FRONTIER_MUSIC_VERSION
     || save->checksum != CalcCRC16((const u8 *)save, offsetof(struct FrontierMusicSave, checksum)))
    {
        memset(save, 0, sizeof(*save));
        save->magic = FRONTIER_MUSIC_MAGIC;
        save->version = FRONTIER_MUSIC_VERSION;
    }
    save->unlocked &= (1u << FRONTIER_MUSIC_TRACK_COUNT) - 1;
    if (save->mode > FRONTIER_MUSIC_RANDOM
     || (save->mode == FRONTIER_MUSIC_FIXED
      && (index >= ARRAY_COUNT(sTracks) || !(save->unlocked & (1u << index))))
     || (save->mode == FRONTIER_MUSIC_RANDOM && save->unlocked == 0))
        save->mode = FRONTIER_MUSIC_DEFAULT;
    if (save->mode != FRONTIER_MUSIC_FIXED)
        save->selectedSong = MUS_DUMMY;
    UpdateChecksum();
}

u16 FrontierMusic_GetSong(u32 index)
{
    return index < ARRAY_COUNT(sTracks) ? sTracks[index].song : MUS_DUMMY;
}

bool8 FrontierMusic_IsUnlocked(u16 song)
{
    u32 index = GetTrackIndex(song);
    FrontierMusic_EnsureSaveData();
    return index < ARRAY_COUNT(sTracks) && (gSaveBlock3Ptr->frontierMusic.unlocked & (1u << index)) != 0;
}

u16 FrontierMusic_Unlock(u16 song)
{
    u32 index = GetTrackIndex(song);
    if (index >= ARRAY_COUNT(sTracks))
        return FRONTIER_MUSIC_INVALID;
    if (FrontierMusic_IsUnlocked(song))
        return FRONTIER_MUSIC_ALREADY_UNLOCKED;
    if (gSaveBlock2Ptr->frontier.battlePoints < FRONTIER_MUSIC_PRICE)
        return FRONTIER_MUSIC_NOT_ENOUGH_BP;
    gSaveBlock2Ptr->frontier.battlePoints -= FRONTIER_MUSIC_PRICE;
    gSaveBlock3Ptr->frontierMusic.unlocked |= 1u << index;
    UpdateChecksum();
    return FRONTIER_MUSIC_OK;
}

u16 FrontierMusic_SetMode(u8 mode, u16 song)
{
    FrontierMusic_EnsureSaveData();
    if (mode > FRONTIER_MUSIC_RANDOM)
        return FRONTIER_MUSIC_INVALID;
    if (mode == FRONTIER_MUSIC_FIXED && !FrontierMusic_IsUnlocked(song))
        return GetTrackIndex(song) < ARRAY_COUNT(sTracks) ? FRONTIER_MUSIC_NOT_UNLOCKED : FRONTIER_MUSIC_INVALID;
    if (mode == FRONTIER_MUSIC_RANDOM && gSaveBlock3Ptr->frontierMusic.unlocked == 0)
        return FRONTIER_MUSIC_EMPTY;
    gSaveBlock3Ptr->frontierMusic.mode = mode;
    gSaveBlock3Ptr->frontierMusic.selectedSong = mode == FRONTIER_MUSIC_FIXED ? song : MUS_DUMMY;
    UpdateChecksum();
    return FRONTIER_MUSIC_OK;
}

// Called once by each battle-music entry point, never by a frame/update task.
u16 FrontierMusic_ChooseBattleBGM(u16 defaultSong)
{
    u32 i, count = 0, selected;
    struct FrontierMusicSave *save = &gSaveBlock3Ptr->frontierMusic;

    if (defaultSong == MUS_POKEVIRUS_RELEASE)
        return defaultSong;

    if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) || !(gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
     || (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK)))
        return defaultSong;
    if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_FRONTIER_BRAIN
     || ((gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) && TRAINER_BATTLE_PARAM.opponentB == TRAINER_FRONTIER_BRAIN))
        return MUS_VS_FRONTIER_BRAIN;
    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
        return defaultSong;

    FrontierMusic_EnsureSaveData();
    if (save->mode == FRONTIER_MUSIC_FIXED)
        return save->selectedSong;
    if (save->mode != FRONTIER_MUSIC_RANDOM)
        return defaultSong;
    for (i = 0; i < ARRAY_COUNT(sTracks); i++)
        if (save->unlocked & (1u << i))
            count++;
    // The secondary stream leaves trainer generation and battle RNG untouched.
    selected = Random2() % count;
    for (i = 0; i < ARRAY_COUNT(sTracks); i++)
        if ((save->unlocked & (1u << i)) && selected-- == 0)
            return sTracks[i].song;
    return defaultSong;
}

void FrontierMusic_BufferSelection(void)
{
    struct FrontierMusicSave *save = &gSaveBlock3Ptr->frontierMusic;
    FrontierMusic_EnsureSaveData();
    StringCopy(gStringVar1, save->mode == FRONTIER_MUSIC_RANDOM ? sText_Random
        : save->mode == FRONTIER_MUSIC_FIXED ? sTracks[GetTrackIndex(save->selectedSong)].name : sText_Default);
}

// Script inputs: 8005 = song id, 8006 = playback mode.
void FrontierMusic_ApplySelection(void)
{
    gSpecialVar_Result = gSpecialVar_0x8006 > FRONTIER_MUSIC_RANDOM ? FRONTIER_MUSIC_INVALID
        : FrontierMusic_SetMode(gSpecialVar_0x8006, gSpecialVar_0x8005);
}

void FrontierMusic_UnlockSelection(void)
{
    gSpecialVar_Result = FrontierMusic_Unlock(gSpecialVar_0x8005);
}

static void PrintTrackStatus(u8 windowId, u32 index, u8 y)
{
    const u8 *text;
    if (index >= ARRAY_COUNT(sTracks))
        return;
    text = !FrontierMusic_IsUnlocked(sTracks[index].song) ? sText_Price
        : gSaveBlock3Ptr->frontierMusic.mode == FRONTIER_MUSIC_FIXED
          && gSaveBlock3Ptr->frontierMusic.selectedSong == sTracks[index].song ? sText_Selected : sText_Unlocked;
    AddTextPrinterParameterized(windowId, FONT_NORMAL, text,
        GetStringRightAlignXOffset(FONT_NORMAL, text, 204), y, TEXT_SKIP_DRAW, NULL);
}

static void CatalogMoveCursor(s32 index, bool8 onInit, struct ListMenu *list)
{
    if (!onInit)
        PlaySE(SE_SELECT);
}

static void Task_Catalog(u8 taskId)
{
    s32 index = ListMenu_ProcessInput(sCatalog->listTaskId);
    ListMenuGetScrollAndRow(sCatalog->listTaskId, &sCatalog->scroll, &sCatalog->row);
    if (index == LIST_NOTHING_CHOSEN)
        return;
    PlaySE(SE_SELECT);
    gSpecialVar_Result = index >= 0 && index < ARRAY_COUNT(sTracks);
    if (gSpecialVar_Result)
    {
        gSpecialVar_0x8005 = sTracks[index].song;
        StringCopy(gStringVar1, sTracks[index].name);
    }
    DestroyListMenuTask(sCatalog->listTaskId, NULL, NULL);
    RemoveScrollIndicatorArrowPair(sCatalog->arrowsTaskId);
    ClearStdWindowAndFrameToTransparent(sCatalog->windowId, TRUE);
    RemoveWindow(sCatalog->windowId);
    Free(sCatalog);
    sCatalog = NULL;
    DestroyTask(taskId);
    ScriptContext_Enable();
}

static void Task_ResumeScript(u8 taskId)
{
    DestroyTask(taskId);
    ScriptContext_Enable();
}

void FrontierMusic_OpenCatalog(void)
{
    const struct WindowTemplate window = {0, 2, 4, 26, 12, 15, 64};
    struct ListMenuTemplate list = {0};
    u8 taskId = CreateTask(Task_ResumeScript, 8);
    u32 i;

    gSpecialVar_Result = FALSE;
    if (sCatalog != NULL)
        return;
    sCatalog = AllocZeroed(sizeof(*sCatalog));
    if (sCatalog == NULL)
        return;
    FrontierMusic_EnsureSaveData();
    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    sCatalog->windowId = AddWindow(&window);
    if (sCatalog->windowId == WINDOW_NONE)
    {
        Free(sCatalog);
        sCatalog = NULL;
        return;
    }
    for (i = 0; i < ARRAY_COUNT(sTracks); i++)
        sCatalog->items[i] = (struct ListMenuItem){sTracks[i].name, i};
    sCatalog->items[i] = (struct ListMenuItem){sText_Cancel, LIST_CANCEL};
    FillWindowPixelBuffer(sCatalog->windowId, PIXEL_FILL(1));
    SetStandardWindowBorderStyle(sCatalog->windowId, FALSE);
    list.items = sCatalog->items;
    list.moveCursorFunc = CatalogMoveCursor;
    list.itemPrintFunc = PrintTrackStatus;
    list.totalItems = ARRAY_COUNT(sCatalog->items);
    list.maxShowed = 6;
    list.windowId = sCatalog->windowId;
    list.item_X = 8;
    list.upText_Y = 1;
    list.cursorPal = 2;
    list.fillValue = 1;
    list.cursorShadowPal = 3;
    list.fontId = FONT_NORMAL;
    list.scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD;
    sCatalog->listTaskId = ListMenuInit(&list, 0, 0);
    sCatalog->arrowsTaskId = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP, 228, 32, 132, list.totalItems - list.maxShowed, 0xFCE0, 0xFCE0, &sCatalog->scroll);
    CopyWindowToVram(sCatalog->windowId, COPYWIN_FULL);
    gTasks[taskId].func = Task_Catalog;
}
