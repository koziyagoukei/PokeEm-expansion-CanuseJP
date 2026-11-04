#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "bg.h"
#include "event_data.h"
#include "frontier_music.h"
#include "international_string_util.h"
#include "list_menu.h"
#include "malloc.h"
#include "main.h"
#include "map_name_popup.h"
#include "menu.h"
#include "menu_helpers.h"
#include "random.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "util.h"
#include "constants/flags.h"
#include "constants/songs.h"
#include "constants/trainers.h"

#define FRONTIER_MUSIC_MAGIC 0x464D5553
#define FRONTIER_MUSIC_VERSION 1

struct FrontierMusicTrack
{
    u16 song;
    u8 name[32];
    u8 category;
};

// Append only: these indices are also the saved unlock-bit positions.
static const struct FrontierMusicTrack sTracks[] =
{
    {MUS_VS_TRAINER, _("{JPN}ホウエン トレーナー"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_WILD, _("{JPN}ホウエン やせい"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_GYM_LEADER, _("{JPN}ホウエン ジムリーダー"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_RIVAL, _("{JPN}ホウエン ライバル"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_ELITE_FOUR, _("{JPN}ホウエン してんのう"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_CHAMPION, _("{JPN}ホウエン チャンピオン"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_AQUA_MAGMA, _("{JPN}アクアだん マグマだん"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_AQUA_MAGMA_LEADER, _("{JPN}アクア マグマリーダー"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_REGI, _("{JPN}レジロックたち"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_KYOGRE_GROUDON, _("{JPN}カイオーガ グラードン"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_RAYQUAZA, _("{JPN}レックウザ"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_MEW, _("{JPN}ミュウ"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_FRONTIER_BRAIN, _("{JPN}フロンティアブレーン"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_RG_VS_TRAINER, _("{JPN}カントー トレーナー"), FRONTIER_MUSIC_CATEGORY_KANTO},
    {MUS_RG_VS_WILD, _("{JPN}カントー やせい"), FRONTIER_MUSIC_CATEGORY_KANTO},
    {MUS_RG_VS_GYM_LEADER, _("{JPN}カントー ジムリーダー"), FRONTIER_MUSIC_CATEGORY_KANTO},
    {MUS_RG_VS_CHAMPION, _("{JPN}カントー チャンピオン"), FRONTIER_MUSIC_CATEGORY_KANTO},
    {MUS_RG_VS_LEGEND, _("{JPN}カントー でんせつ"), FRONTIER_MUSIC_CATEGORY_KANTO},
    {MUS_RG_VS_MEWTWO, _("{JPN}ミュウツー"), FRONTIER_MUSIC_CATEGORY_KANTO},
    {MUS_RG_VS_DEOXYS, _("{JPN}デオキシス"), FRONTIER_MUSIC_CATEGORY_KANTO},
    {MUS_C_VS_LEGEND_BEAST, _("{JPN}スイクンたち"), FRONTIER_MUSIC_CATEGORY_KANTO},
    {MUS_POKEVIRUS_RELEASE, _("{JPN}かいほうせん"), FRONTIER_MUSIC_CATEGORY_ORIGINAL},
    {MUS_POKEVIRUS_FINAL, _("{JPN}ヒトポケルス"), FRONTIER_MUSIC_CATEGORY_ORIGINAL},
    {MUS_VS_COLOSSEUM, _("{JPN}コロシアム トレーナー"), FRONTIER_MUSIC_CATEGORY_ORRE},
    {MUS_VS_COLOSSEUM_ADMIN, _("{JPN}コロシアム かんぶ"), FRONTIER_MUSIC_CATEGORY_ORRE},
    {MUS_VS_COLOSSEUM_2, _("{JPN}コロシアム トレーナー2"), FRONTIER_MUSIC_CATEGORY_ORRE},
    {MUS_VS_PMD_RAYQUAZA, _("{JPN}きゅうじょたい レックウザ"), FRONTIER_MUSIC_CATEGORY_DUNGEON},
    {MUS_VS_PMD_BOSS, _("{JPN}きゅうじょたい ボス"), FRONTIER_MUSIC_CATEGORY_DUNGEON},
    {MUS_VS_ZINNIA, _("{JPN}ORAS ヒガナ"), FRONTIER_MUSIC_CATEGORY_HOENN},
    {MUS_VS_PMD_DIALGA, _("{JPN}たんけんたい ディアルガ"), FRONTIER_MUSIC_CATEGORY_DUNGEON},
};

static const u16 sRescuedFlags[] =
{
    FLAG_STORY_RESCUED_TOWER, FLAG_STORY_RESCUED_DOME, FLAG_STORY_RESCUED_PALACE, FLAG_STORY_RESCUED_ARENA,
    FLAG_STORY_RESCUED_FACTORY, FLAG_STORY_RESCUED_PIKE, FLAG_STORY_RESCUED_PYRAMID, FLAG_STORY_RESCUED_CONTEST,
};

static const u8 sText_Cancel[] = _("{JPN}やめる");
static const u8 sText_Back[] = _("{JPN}もどる");
static const u8 sText_CatalogTitle[] = _("{JPN}BGM さくひん");
static const u8 sCategoryNames[FRONTIER_MUSIC_CATEGORY_COUNT][16] =
{
    [FRONTIER_MUSIC_CATEGORY_KANTO] = _("{JPN}カントー"),
    [FRONTIER_MUSIC_CATEGORY_HOENN] = _("{JPN}ホウエン"),
    [FRONTIER_MUSIC_CATEGORY_ORRE] = _("{JPN}オーレ"),
    [FRONTIER_MUSIC_CATEGORY_DUNGEON] = _("{JPN}ダンジョン"),
    [FRONTIER_MUSIC_CATEGORY_ORIGINAL] = _("{JPN}オリジナル"),
};
static const u8 sText_Default[] = _("{JPN}もとの BGM");
static const u8 sText_Random[] = _("{JPN}かいほうずみから ランダム");
static const u8 sText_Price[] = _("{ENG}5BP");
static const u8 sText_Unlocked[] = _("{JPN}ずみ");
static const u8 sText_Selected[] = _("{JPN}せってい");
static const u8 sText_PreviewControls[] = _("{JPN}SELECT:しちょう A:けってい B:もどる");
static const u8 sText_StopPreviewControls[] = _("{JPN}SELECT:ていし A:けってい B:もどる");
static const u8 sText_CancelControls[] = _("{JPN}A:もどる B:もどる");
static const u8 sText_CategoryControls[] = _("{JPN}A:ひらく B:もどる");

static EWRAM_DATA bool8 sPreviewActive = FALSE;
static EWRAM_DATA u16 sPreviewSong = 0;
static EWRAM_DATA u16 sPreviewReturnSong = 0;

struct FrontierMusicCatalog
{
    struct ListMenuItem items[FRONTIER_MUSIC_TRACK_COUNT + 1];
    u16 scroll;
    u16 row;
    u16 categoryScroll;
    u16 categoryRow;
    u8 category;
    u8 windowId;
    u8 headerWindowId;
    u8 footerWindowId;
    u8 frameWindowId;
    u8 listTaskId;
    u8 arrowsTaskId;
    const u8 *controlsText;
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
    save->earned &= FRONTIER_MUSIC_EARNED_RELEASE | FRONTIER_MUSIC_EARNED_FINAL;
    if (save->mode > FRONTIER_MUSIC_RANDOM
     || (save->mode == FRONTIER_MUSIC_FIXED
      && (index >= ARRAY_COUNT(sTracks) || !(save->unlocked & (1u << index))))
     || (save->mode == FRONTIER_MUSIC_RANDOM && save->unlocked == 0))
        save->mode = FRONTIER_MUSIC_DEFAULT;
    if (save->mode != FRONTIER_MUSIC_FIXED)
        save->selectedSong = MUS_DUMMY;
    UpdateChecksum();
}

// The story tracks are offered only after their battle is won. Saves from before the
// earned bits fall back on the story flags those wins lead to.
bool8 FrontierMusic_IsAvailable(u16 song)
{
    u32 i;
    u16 earned = gSaveBlock3Ptr->frontierMusic.earned;

    switch (song)
    {
    case MUS_POKEVIRUS_RELEASE:
        if ((earned & FRONTIER_MUSIC_EARNED_RELEASE) || FlagGet(FLAG_STORY_CURE_FOUND))
            return TRUE;
        for (i = 0; i < ARRAY_COUNT(sRescuedFlags); i++)
            if (FlagGet(sRescuedFlags[i]))
                return TRUE;
        return FALSE;
    case MUS_POKEVIRUS_FINAL:
        return (earned & FRONTIER_MUSIC_EARNED_FINAL) || FlagGet(FLAG_STORY_POKEVIRUS_FADED);
    default:
        return GetTrackIndex(song) < ARRAY_COUNT(sTracks);
    }
}

// Called when a release battle (or the final battle) is won.
void FrontierMusic_Earn(bool32 finalBattle)
{
    FrontierMusic_EnsureSaveData();
    gSaveBlock3Ptr->frontierMusic.earned |= FRONTIER_MUSIC_EARNED_RELEASE;
    if (finalBattle)
        gSaveBlock3Ptr->frontierMusic.earned |= FRONTIER_MUSIC_EARNED_FINAL;
}

u16 FrontierMusic_GetSong(u32 index)
{
    return index < ARRAY_COUNT(sTracks) ? sTracks[index].song : MUS_DUMMY;
}

u8 FrontierMusic_GetCategory(u16 song)
{
    u32 index = GetTrackIndex(song);
    return index < ARRAY_COUNT(sTracks) ? sTracks[index].category : FRONTIER_MUSIC_CATEGORY_COUNT;
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
    if (index >= ARRAY_COUNT(sTracks) || !FrontierMusic_IsAvailable(song))
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

    if (defaultSong == MUS_POKEVIRUS_RELEASE || defaultSong == MUS_POKEVIRUS_FINAL)
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

bool8 FrontierMusic_Preview(u16 song)
{
    if (!FrontierMusic_IsAvailable(song))
        return FALSE;
    if (!sPreviewActive)
    {
        sPreviewReturnSong = GetCurrentMapMusic();
        sPreviewActive = TRUE;
    }
    sPreviewSong = song;
    // Leave the map music state and the saved battle preference untouched.
    PlayBGM(song);
    return TRUE;
}

void FrontierMusic_StopPreview(void)
{
    if (!sPreviewActive)
        return;
    PlayBGM(sPreviewReturnSong);
    sPreviewActive = FALSE;
    sPreviewSong = MUS_DUMMY;
    sPreviewReturnSong = MUS_DUMMY;
}

static void PrintCatalogControls(s32 index)
{
    const u8 *text = sCatalog->category == FRONTIER_MUSIC_CATEGORY_COUNT
        ? (index == LIST_CANCEL ? sText_CancelControls : sText_CategoryControls)
        : index < 0 || index >= ARRAY_COUNT(sTracks) ? sText_CancelControls
        : sPreviewActive && sPreviewSong == sTracks[index].song ? sText_StopPreviewControls : sText_PreviewControls;
    if (sCatalog->controlsText == text)
        return;
    sCatalog->controlsText = text;
    FillWindowPixelBuffer(sCatalog->footerWindowId, PIXEL_FILL(1));
    AddTextPrinterParameterized(sCatalog->footerWindowId, FONT_SMALL, text, 4, 2, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sCatalog->footerWindowId, COPYWIN_GFX);
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
    PrintCatalogControls(index);
}

static void DestroyCatalogList(void)
{
    if (sCatalog->listTaskId != TASK_NONE)
        DestroyListMenuTask(sCatalog->listTaskId, NULL, NULL);
    if (sCatalog->arrowsTaskId != TASK_NONE)
        RemoveScrollIndicatorArrowPair(sCatalog->arrowsTaskId);
    sCatalog->listTaskId = sCatalog->arrowsTaskId = TASK_NONE;
}

static void RemoveCatalogWindows(void)
{
    const u8 windows[] = {sCatalog->windowId, sCatalog->headerWindowId,
                          sCatalog->footerWindowId, sCatalog->frameWindowId};
    for (u32 i = 0; i < ARRAY_COUNT(windows); i++)
        if (windows[i] != WINDOW_NONE)
            RemoveWindow(windows[i]);
}

static void BuildCatalogList(u8 category, u16 scroll, u16 row)
{
    struct ListMenuTemplate list = {0};
    u32 count = 0;
    DestroyCatalogList();
    sCatalog->category = category;
    sCatalog->scroll = scroll;
    sCatalog->row = row;
    if (category == FRONTIER_MUSIC_CATEGORY_COUNT)
    {
        for (u32 i = 0; i < FRONTIER_MUSIC_CATEGORY_COUNT; i++)
            sCatalog->items[count++] = (struct ListMenuItem){sCategoryNames[i], i};
    }
    else
    {
        for (u32 i = 0; i < ARRAY_COUNT(sTracks); i++)
            if (sTracks[i].category == category && FrontierMusic_IsAvailable(sTracks[i].song))
                sCatalog->items[count++] = (struct ListMenuItem){sTracks[i].name, i};
    }
    sCatalog->items[count++] = (struct ListMenuItem){category == FRONTIER_MUSIC_CATEGORY_COUNT ? sText_Cancel : sText_Back, LIST_CANCEL};
    FillWindowPixelBuffer(sCatalog->headerWindowId, PIXEL_FILL(1));
    AddTextPrinterParameterized(sCatalog->headerWindowId, FONT_SMALL,
        category == FRONTIER_MUSIC_CATEGORY_COUNT ? sText_CatalogTitle : sCategoryNames[category], 4, 2, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sCatalog->headerWindowId, COPYWIN_GFX);
    FillWindowPixelBuffer(sCatalog->windowId, PIXEL_FILL(1));
    list.items = sCatalog->items;
    list.moveCursorFunc = CatalogMoveCursor;
    list.itemPrintFunc = category == FRONTIER_MUSIC_CATEGORY_COUNT ? NULL : PrintTrackStatus;
    list.totalItems = count;
    list.maxShowed = min(count, 4);
    list.windowId = sCatalog->windowId;
    list.item_X = 8;
    list.upText_Y = 0;
    list.cursorPal = 2;
    list.fillValue = 1;
    list.cursorShadowPal = 3;
    list.fontId = FONT_NORMAL;
    list.scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD;
    sCatalog->listTaskId = ListMenuInit(&list, scroll, row);
    if (count > list.maxShowed)
        sCatalog->arrowsTaskId = AddScrollIndicatorArrowPairParameterized(
            SCROLL_ARROW_UP, 228, 56, 104,
            count - list.maxShowed, 0xFCE0, 0xFCE0, &sCatalog->scroll);
}

static void Task_Catalog(u8 taskId)
{
    if (JOY_NEW(SELECT_BUTTON))
    {
        if (sCatalog->category == FRONTIER_MUSIC_CATEGORY_COUNT)
            return;
        u16 item;
        ListMenuGetCurrentItemArrayId(sCatalog->listTaskId, &item);
        s32 selected = sCatalog->items[item].id;
        if (selected >= 0 && selected < ARRAY_COUNT(sTracks))
        {
            if (sPreviewActive && sPreviewSong == sTracks[selected].song)
                FrontierMusic_StopPreview();
            else
                FrontierMusic_Preview(sTracks[selected].song);
            PlaySE(SE_SELECT);
            PrintCatalogControls(selected);
        }
        return;
    }
    s32 index = ListMenu_ProcessInput(sCatalog->listTaskId);
    ListMenuGetScrollAndRow(sCatalog->listTaskId, &sCatalog->scroll, &sCatalog->row);
    if (index == LIST_NOTHING_CHOSEN)
        return;
    PlaySE(SE_SELECT);
    if (sCatalog->category == FRONTIER_MUSIC_CATEGORY_COUNT && index >= 0 && index < FRONTIER_MUSIC_CATEGORY_COUNT)
    {
        sCatalog->categoryScroll = sCatalog->scroll;
        sCatalog->categoryRow = sCatalog->row;
        BuildCatalogList(index, 0, 0);
        return;
    }
    if (sCatalog->category < FRONTIER_MUSIC_CATEGORY_COUNT && index == LIST_CANCEL)
    {
        FrontierMusic_StopPreview();
        BuildCatalogList(FRONTIER_MUSIC_CATEGORY_COUNT, sCatalog->categoryScroll, sCatalog->categoryRow);
        return;
    }
    gSpecialVar_Result = index >= 0 && index < ARRAY_COUNT(sTracks);
    if (gSpecialVar_Result)
    {
        gSpecialVar_0x8005 = sTracks[index].song;
        StringCopy(gStringVar1, sTracks[index].name);
    }
    FrontierMusic_StopPreview();
    DestroyCatalogList();
    FillBgTilemapBufferRect(0, 0, 1, 3, 28, 14, 0);
    CopyBgTilemapBufferToVram(0);
    RemoveCatalogWindows();
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
    // Three non-overlapping buffers use the same 312 tiles as the former single window.
    const struct WindowTemplate header = {0, 2, 4, 26, 2, 15, 64};
    const struct WindowTemplate window = {0, 2, 6, 26, 8, 15, 116};
    const struct WindowTemplate footer = {0, 2, 14, 26, 2, 15, 324};
    const struct WindowTemplate frame = {0, 2, 4, 26, 12, 15, 64};
    if (sCatalog != NULL)
        return;
    u8 taskId = CreateTask(Task_ResumeScript, 8);
    gSpecialVar_Result = FALSE;
    if (taskId == TASK_NONE)
        return;
    sCatalog = AllocZeroed(sizeof(*sCatalog));
    if (sCatalog == NULL)
        return;
    sCatalog->windowId = sCatalog->headerWindowId = sCatalog->footerWindowId = sCatalog->frameWindowId = WINDOW_NONE;
    sCatalog->listTaskId = sCatalog->arrowsTaskId = TASK_NONE;
    FrontierMusic_StopPreview();
    FrontierMusic_EnsureSaveData();
    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    sCatalog->frameWindowId = AddWindowWithoutTileMap(&frame);
    sCatalog->headerWindowId = AddWindow(&header);
    sCatalog->windowId = AddWindow(&window);
    sCatalog->footerWindowId = AddWindow(&footer);
    if (sCatalog->windowId == WINDOW_NONE || sCatalog->headerWindowId == WINDOW_NONE
     || sCatalog->footerWindowId == WINDOW_NONE || sCatalog->frameWindowId == WINDOW_NONE)
    {
        RemoveCatalogWindows();
        Free(sCatalog);
        sCatalog = NULL;
        return;
    }
    DrawTextBorderOuter(sCatalog->frameWindowId, STD_WINDOW_BASE_TILE_NUM, STD_WINDOW_PALETTE_NUM);
    PutWindowTilemap(sCatalog->headerWindowId);
    PutWindowTilemap(sCatalog->windowId);
    PutWindowTilemap(sCatalog->footerWindowId);
    BuildCatalogList(FRONTIER_MUSIC_CATEGORY_COUNT, 0, 0);
    CopyBgTilemapBufferToVram(0);
    gTasks[taskId].func = Task_Catalog;
}
