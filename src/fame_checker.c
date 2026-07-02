#include "global.h"
#include "bg.h"
#include "constants/songs.h"
#include "event_data.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "battle.h"
#include "battle_setup.h"
#include "menu.h"
#include "trainer_pokemon_sprites.h"
#include "scanline_effect.h"
#include "item_menu.h"
#include "list_menu.h"
#include "malloc.h"
#include "overworld.h"
#include "palette.h"
#include "sound.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "event_object_movement.h"
#include "text_window.h"
#include "fame_checker.h"
#include "strings.h"
#include "ultra_help.h"
#include "window.h"
#include "constants/event_objects.h"

#define SPRITETAG_SELECTOR_CURSOR 1000
#define SPRITETAG_QUESTION_MARK 1001
#define SPRITETAG_SPINNING_POKEBALL 1002
#define SPRITETAG_SCROLL_INDICATORS 1004
#define SPRITETAG_DAISY 1006 // TODO: Investigate, seems to be used for other NPCs (e.g. Fan Club Chairman)
#define SPRITETAG_FUJI 1007
#define SPRITETAG_OAK 1008
#define SPRITETAG_BILL 1009

#define FC_NONTRAINER_START 0xFE00
#define FAMECHECKER_HELP_ENTRY 0xFE
#define FAMECHECKER_CANCEL_ENTRY 0xFF
#define FC_FLAVOR_TEXT_COUNT 6
#define FC_FLAVOR_TEXT_LENGTH 64
#define FC_LIST_ROW_HEIGHT 14
#define FC_LIST_ROW_TEXT_Y(row) (FC_LIST_ROW_HEIGHT * (row) + 4)
#define FC_ICON_GRAPHICS_NONE 0xFFFF

#define tExitToUltraHelp data[15]

struct FameCheckerData
{
    MainCallback savedCallback;
    u16 listMenuTopIdx;
    u8 scrollIndicatorPairTaskId;
    u8 personHasUnlockedPanels:1;
    u8 inPickMode:1;
    u8 numUnlockedPersons:6;
    u8 listMenuTaskId;
    u8 listMenuCurIdx;
    u8 listMenuTopIdx2;
    u8 listMenuDrawnSelIdx;
    u8 unlockedPersons[NUM_FAMECHECKER_PERSONS + 2];
    u8 spriteIds[6];
    u16 spriteGraphicsIds[6];
    u8 viewingFlavorText:1;
    u8 unk_23_1:1; // unused
    u8 pickModeOverCancel:1;
};

static EWRAM_DATA u16 * sBg3TilemapBuffer = NULL;
static EWRAM_DATA u16 * sBg1TilemapBuffer = NULL;
static EWRAM_DATA u16 * sBg2TilemapBuffer = NULL;
static EWRAM_DATA struct FameCheckerData * sFameCheckerData = NULL;
static EWRAM_DATA struct ListMenuItem * sListMenuItems = NULL;
static EWRAM_DATA s32 sLastMenuIdx = 0;
static EWRAM_DATA MainCallback sFameCheckerReturnCallback = NULL;
static struct SpriteTemplate sFameCheckerObjectSpriteTemplates[FC_FLAVOR_TEXT_COUNT];

COMMON_DATA struct ListMenuTemplate gFameChecker_ListMenuTemplate = {0};
COMMON_DATA u8 gIconDescriptionBoxIsOpen = 0;

static void MainCB2_LoadFameChecker(void);
static void LoadUISpriteSheetsAndPalettes(void);
static void Task_WaitFadeOnInit(u8 taskId);
static void Task_TopMenuHandleInput(u8 taskId);
static bool8 TryExitPickMode(u8 taskId);
static void MessageBoxPrintEmptyText(void);
static void Task_EnterPickMode(u8 taskId);
static void Task_ExitPickMode(u8 taskId);
static void Task_FlavorTextDisplayHandleInput(u8 taskId);
static void FC_MoveSelectorCursor(u8 taskId, s8 dx, s8 dy);
static void GetPickModeText(void);
static void PrintSelectedNameInBrightGreen(u8 taskId);
static void WipeMsgBoxAndTransfer(void);
static void Setup_DrawMsgAndListBoxes(void);
static void FC_PutWindowTilemapAndCopyWindowToVramMode3(u8 windowId);
static bool8 SetMessageSelectorIconObjMode(u8 taskId, u8 objMode);
static void Task_StartToCloseFameChecker(u8 taskId);
static void Task_StartUltraHelpFromFameChecker(u8 taskId);
static void Task_DestroyAssetsAndCloseFameChecker(u8 taskId);
static void FC_DestroyWindow(u8 windowId);
static void PrintUIHelp(u8 state);
static bool8 CreateAllFlavorTextIcons(u8 who);
static u8 CreateFameCheckerObject(u16 graphicsId, u8 localId, s16 x, s16 y);
static void FreeFameCheckerObjectPalette(u16 graphicsId);
static void FCSetup_ClearVideoRegisters(void);
static void FCSetup_ResetTasksAndSpriteResources(void);
static void FCSetup_TurnOnDisplay(void);
static void FCSetup_ResetBGCoords(void);
static bool8 HasUnlockedAllFlavorTextsForCurrentPerson(void);
static void FreeSelectionCursorSpriteResources(void);
static u8 CreateFlavorTextIconSelectorCursorSprite(s16 where);
static void SpriteCB_DestroyFlavorTextIconSelectorCursor(struct Sprite *sprite);
static void FreeQuestionMarkSpriteResources(void);
static u8 PlaceQuestionMarkTile(u8 x, u8 y);
static void FreeSpinningPokeballSpriteResources(void);
static u8 CreateSpinningPokeballSprite(void);
static void SpriteCB_DestroySpinningPokeball(struct Sprite *sprite);
static void FreeNonTrainerPicTiles(void);
static u8 CreatePersonPicSprite(u8 fcPersonIdx);
static void DestroyPersonPicSprite(u8 taskId, u16 who);
static void UpdateIconDescriptionBox(u8 whichText);
static void UpdateIconDescriptionBoxOff(void);
static void FC_CreateListMenu(void);
static void SpriteCB_FCSpinningPokeball(struct Sprite *sprite);
static void InitListMenuTemplate(void);
static void FC_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu * list);
static void Task_SwitchToPickMode(u8 taskId);
static void PrintCancelDescription(void);
static void PrintHelpDescription(void);
static void FC_DoMoveCursor(s32 itemIndex, bool8 onInit);
static u8 FC_PopulateListMenu(void);
static void FC_PutWindowTilemapAndCopyWindowToVramMode3_2(u8 windowId);
static void FC_CreateScrollIndicatorArrowPair(void);
static void FreeListMenuSelectorArrowPairResources(void);
static u16 FameCheckerGetCursorY(void);
static void HandleFlavorTextModeSwitch(bool8 state);
static void Task_FCOpenOrCloseInfoBox(u8 taskId);
static void UpdateInfoBoxTilemap(u8 bg, s16 state);
static void PlaceListMenuCursor(bool8 isActive);
static void NormalizeFameCheckerSaveData(void);
static void UnlockAllFameCheckerPeople(void);
static void CB2_ReturnToFameCheckerFromUltraHelp(void);

static const u32 gFameCheckerBgTiles[] = INCGFX_U32("graphics/fame_checker/bg.png", ".4bpp");
static const u16 gFameCheckerBgPals[] = INCGFX_U16("graphics/fame_checker/bg.png", ".gbapal");
static const u16 gFameCheckerBg3Tilemap[] = INCBIN_U16("graphics/fame_checker/tilemap3.bin");
static const u16 gFameCheckerBg2Tilemap[] = INCBIN_U16("graphics/fame_checker/tilemap2.bin");
static const u16 sFameCheckerTilemap[] = INCBIN_U16("graphics/fame_checker/tilemap1.bin");
static const u8 sQuestionMarkSpriteGfx[] = INCBIN_U8("graphics/fame_checker/question_mark.4bpp");
static const u8 sSpinningPokeballSpriteGfx[] = INCBIN_U8("graphics/fame_checker/spinning_pokeball.4bpp");
static const u16 sSpinningPokeballSpritePalette[] = INCBIN_U16("graphics/fame_checker/spinning_pokeball.gbapal");
static const u8 sSelectorCursorSpriteGfx[] = INCBIN_U8("graphics/fame_checker/cursor.4bpp");
static const u16 sSelectorCursorSpritePalette[] = INCBIN_U16("graphics/fame_checker/cursor.gbapal");
static const u8 sFujiSpriteGfx[] = INCBIN_U8("graphics/fame_checker/mr_fuji.4bpp");
static const u16 sFujiSpritePalette[] = INCBIN_U16("graphics/fame_checker/mr_fuji.gbapal");
static const u8 sBillSpriteGfx[] = INCBIN_U8("graphics/fame_checker/bill.4bpp");
static const u16 sBillSpritePalette[] = INCBIN_U16("graphics/fame_checker/bill.gbapal");
static const u8 sDaisySpriteGfx[] = INCBIN_U8("graphics/fame_checker/daisy.4bpp");
static const u16 sDaisySpritePalette[] = INCBIN_U16("graphics/fame_checker/daisy.gbapal");
static const u8 sOakSpriteGfx[] = INCBIN_U8("graphics/fame_checker/prof_oak.4bpp");
static const u16 sOakSpritePalette[] = INCBIN_U16("graphics/fame_checker/prof_oak.gbapal");
static const u16 sSilhouettePalette[] = INCBIN_U16("graphics/fame_checker/silhouette.gbapal");

static const u8 sTextColor_White[3]  = {0, 1, 2};
static const u8 sTextColor_DkGrey[3] = {0, 2, 3};
static const u8 sTextColor_Green[3]  = {0, 6, 7};

/* FRLG Fame Checker text labels are defined in data/text/fame_checker_frlg.inc. */
extern const u8 gFameCheckerFlavorTextOriginLocation_Agatha0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Agatha1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Agatha2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Agatha3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Agatha4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Agatha5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bill0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bill1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bill2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bill3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bill4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bill5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Blaine0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Blaine1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Blaine2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Blaine3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Blaine4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Blaine5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Brock0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Brock1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Brock2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Brock3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Brock4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Brock5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bruno0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bruno1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bruno2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bruno3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bruno4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Bruno5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Daisy0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Daisy1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Daisy2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Daisy3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Daisy4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Daisy5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Erika0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Erika1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Erika2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Erika3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Erika4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Erika5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Giovanni0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Giovanni1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Giovanni2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Giovanni3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Giovanni4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Giovanni5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Koga0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Koga1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Koga2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Koga3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Koga4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Koga5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lance0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lance1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lance2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lance3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lance4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lance5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lorelei0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lorelei1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lorelei2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lorelei3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lorelei4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Lorelei5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_LtSurge0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_LtSurge1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_LtSurge2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_LtSurge3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_LtSurge4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_LtSurge5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Misty0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Misty1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Misty2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Misty3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Misty4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Misty5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_MrFuji0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_MrFuji1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_MrFuji2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_MrFuji3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_MrFuji4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_MrFuji5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_ProfOak0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_ProfOak1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_ProfOak2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_ProfOak3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_ProfOak4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_ProfOak5[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Sabrina0[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Sabrina1[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Sabrina2[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Sabrina3[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Sabrina4[];
extern const u8 gFameCheckerFlavorTextOriginLocation_Sabrina5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Agatha0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Agatha1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Agatha2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Agatha3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Agatha4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Agatha5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bill0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bill1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bill2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bill3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bill4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bill5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Blaine0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Blaine1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Blaine2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Blaine3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Blaine4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Blaine5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Brock0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Brock1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Brock2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Brock3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Brock4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Brock5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bruno0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bruno1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bruno2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bruno3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bruno4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Bruno5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Daisy0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Daisy1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Daisy2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Daisy3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Daisy4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Daisy5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Erika0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Erika1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Erika2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Erika3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Erika4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Erika5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Giovanni0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Giovanni1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Giovanni2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Giovanni3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Giovanni4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Giovanni5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Koga0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Koga1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Koga2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Koga3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Koga4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Koga5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lance0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lance1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lance2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lance3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lance4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lance5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lorelei0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lorelei1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lorelei2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lorelei3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lorelei4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Lorelei5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_LtSurge0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_LtSurge1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_LtSurge2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_LtSurge3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_LtSurge4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_LtSurge5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Misty0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Misty1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Misty2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Misty3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Misty4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Misty5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_MrFuji0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_MrFuji1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_MrFuji2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_MrFuji3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_MrFuji4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_MrFuji5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_ProfOak0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_ProfOak1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_ProfOak2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_ProfOak3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_ProfOak4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_ProfOak5[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Sabrina0[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Sabrina1[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Sabrina2[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Sabrina3[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Sabrina4[];
extern const u8 gFameCheckerFlavorTextOriginObjectName_Sabrina5[];
extern const u8 gFameCheckerFlavorText_Agatha0[];
extern const u8 gFameCheckerFlavorText_Agatha1[];
extern const u8 gFameCheckerFlavorText_Agatha2[];
extern const u8 gFameCheckerFlavorText_Agatha3[];
extern const u8 gFameCheckerFlavorText_Agatha4[];
extern const u8 gFameCheckerFlavorText_Agatha5[];
extern const u8 gFameCheckerFlavorText_Bill0[];
extern const u8 gFameCheckerFlavorText_Bill1[];
extern const u8 gFameCheckerFlavorText_Bill2[];
extern const u8 gFameCheckerFlavorText_Bill3[];
extern const u8 gFameCheckerFlavorText_Bill4[];
extern const u8 gFameCheckerFlavorText_Bill5[];
extern const u8 gFameCheckerFlavorText_Blaine0[];
extern const u8 gFameCheckerFlavorText_Blaine1[];
extern const u8 gFameCheckerFlavorText_Blaine2[];
extern const u8 gFameCheckerFlavorText_Blaine3[];
extern const u8 gFameCheckerFlavorText_Blaine4[];
extern const u8 gFameCheckerFlavorText_Blaine5[];
extern const u8 gFameCheckerFlavorText_Brock0[];
extern const u8 gFameCheckerFlavorText_Brock1[];
extern const u8 gFameCheckerFlavorText_Brock2[];
extern const u8 gFameCheckerFlavorText_Brock3[];
extern const u8 gFameCheckerFlavorText_Brock4[];
extern const u8 gFameCheckerFlavorText_Brock5[];
extern const u8 gFameCheckerFlavorText_Bruno0[];
extern const u8 gFameCheckerFlavorText_Bruno1[];
extern const u8 gFameCheckerFlavorText_Bruno2[];
extern const u8 gFameCheckerFlavorText_Bruno3[];
extern const u8 gFameCheckerFlavorText_Bruno4[];
extern const u8 gFameCheckerFlavorText_Bruno5[];
extern const u8 gFameCheckerFlavorText_Daisy0[];
extern const u8 gFameCheckerFlavorText_Daisy1[];
extern const u8 gFameCheckerFlavorText_Daisy2[];
extern const u8 gFameCheckerFlavorText_Daisy3[];
extern const u8 gFameCheckerFlavorText_Daisy4[];
extern const u8 gFameCheckerFlavorText_Daisy5[];
extern const u8 gFameCheckerFlavorText_Erika0[];
extern const u8 gFameCheckerFlavorText_Erika1[];
extern const u8 gFameCheckerFlavorText_Erika2[];
extern const u8 gFameCheckerFlavorText_Erika3[];
extern const u8 gFameCheckerFlavorText_Erika4[];
extern const u8 gFameCheckerFlavorText_Erika5[];
extern const u8 gFameCheckerFlavorText_Giovanni0[];
extern const u8 gFameCheckerFlavorText_Giovanni1[];
extern const u8 gFameCheckerFlavorText_Giovanni2[];
extern const u8 gFameCheckerFlavorText_Giovanni3[];
extern const u8 gFameCheckerFlavorText_Giovanni4[];
extern const u8 gFameCheckerFlavorText_Giovanni5[];
extern const u8 gFameCheckerFlavorText_Koga0[];
extern const u8 gFameCheckerFlavorText_Koga1[];
extern const u8 gFameCheckerFlavorText_Koga2[];
extern const u8 gFameCheckerFlavorText_Koga3[];
extern const u8 gFameCheckerFlavorText_Koga4[];
extern const u8 gFameCheckerFlavorText_Koga5[];
extern const u8 gFameCheckerFlavorText_Lance0[];
extern const u8 gFameCheckerFlavorText_Lance1[];
extern const u8 gFameCheckerFlavorText_Lance2[];
extern const u8 gFameCheckerFlavorText_Lance3[];
extern const u8 gFameCheckerFlavorText_Lance4[];
extern const u8 gFameCheckerFlavorText_Lance5[];
extern const u8 gFameCheckerFlavorText_Lorelei0[];
extern const u8 gFameCheckerFlavorText_Lorelei1[];
extern const u8 gFameCheckerFlavorText_Lorelei2[];
extern const u8 gFameCheckerFlavorText_Lorelei3[];
extern const u8 gFameCheckerFlavorText_Lorelei4[];
extern const u8 gFameCheckerFlavorText_Lorelei5[];
extern const u8 gFameCheckerFlavorText_LtSurge0[];
extern const u8 gFameCheckerFlavorText_LtSurge1[];
extern const u8 gFameCheckerFlavorText_LtSurge2[];
extern const u8 gFameCheckerFlavorText_LtSurge3[];
extern const u8 gFameCheckerFlavorText_LtSurge4[];
extern const u8 gFameCheckerFlavorText_LtSurge5[];
extern const u8 gFameCheckerFlavorText_Misty0[];
extern const u8 gFameCheckerFlavorText_Misty1[];
extern const u8 gFameCheckerFlavorText_Misty2[];
extern const u8 gFameCheckerFlavorText_Misty3[];
extern const u8 gFameCheckerFlavorText_Misty4[];
extern const u8 gFameCheckerFlavorText_Misty5[];
extern const u8 gFameCheckerFlavorText_MrFuji0[];
extern const u8 gFameCheckerFlavorText_MrFuji1[];
extern const u8 gFameCheckerFlavorText_MrFuji2[];
extern const u8 gFameCheckerFlavorText_MrFuji3[];
extern const u8 gFameCheckerFlavorText_MrFuji4[];
extern const u8 gFameCheckerFlavorText_MrFuji5[];
extern const u8 gFameCheckerFlavorText_ProfOak0[];
extern const u8 gFameCheckerFlavorText_ProfOak1[];
extern const u8 gFameCheckerFlavorText_ProfOak2[];
extern const u8 gFameCheckerFlavorText_ProfOak3[];
extern const u8 gFameCheckerFlavorText_ProfOak4[];
extern const u8 gFameCheckerFlavorText_ProfOak5[];
extern const u8 gFameCheckerFlavorText_Sabrina0[];
extern const u8 gFameCheckerFlavorText_Sabrina1[];
extern const u8 gFameCheckerFlavorText_Sabrina2[];
extern const u8 gFameCheckerFlavorText_Sabrina3[];
extern const u8 gFameCheckerFlavorText_Sabrina4[];
extern const u8 gFameCheckerFlavorText_Sabrina5[];
extern const u8 gFameCheckerPersonName_Agatha[];
extern const u8 gFameCheckerPersonName_Bill[];
extern const u8 gFameCheckerPersonName_Blaine[];
extern const u8 gFameCheckerPersonName_Brock[];
extern const u8 gFameCheckerPersonName_Bruno[];
extern const u8 gFameCheckerPersonName_Daisy[];
extern const u8 gFameCheckerPersonName_Erika[];
extern const u8 gFameCheckerPersonName_Giovanni[];
extern const u8 gFameCheckerPersonName_Koga[];
extern const u8 gFameCheckerPersonName_Lance[];
extern const u8 gFameCheckerPersonName_Lorelei[];
extern const u8 gFameCheckerPersonName_LtSurge[];
extern const u8 gFameCheckerPersonName_Misty[];
extern const u8 gFameCheckerPersonName_MrFuji[];
extern const u8 gFameCheckerPersonName_ProfOak[];
extern const u8 gFameCheckerPersonName_Sabrina[];
extern const u8 gFameCheckerPersonQuote_Agatha[];
extern const u8 gFameCheckerPersonQuote_Bill[];
extern const u8 gFameCheckerPersonQuote_Blaine[];
extern const u8 gFameCheckerPersonQuote_Brock[];
extern const u8 gFameCheckerPersonQuote_Bruno[];
extern const u8 gFameCheckerPersonQuote_Daisy[];
extern const u8 gFameCheckerPersonQuote_Erika[];
extern const u8 gFameCheckerPersonQuote_Giovanni[];
extern const u8 gFameCheckerPersonQuote_Koga[];
extern const u8 gFameCheckerPersonQuote_Lance[];
extern const u8 gFameCheckerPersonQuote_Lorelei[];
extern const u8 gFameCheckerPersonQuote_LtSurge[];
extern const u8 gFameCheckerPersonQuote_Misty[];
extern const u8 gFameCheckerPersonQuote_MrFuji[];
extern const u8 gFameCheckerPersonQuote_ProfOak[];
extern const u8 gFameCheckerPersonQuote_Sabrina[];

static const u8 gFameCheckerText_ClearTextbox[] = _("{JPN}");
static const u8 gFameCheckerText_MainScreenUI[] = _("{JPN}みる くわしく やめる");
static const u8 gFameCheckerText_FlavorTextUI[] = _("{JPN}きく もどる");
static const u8 gFameCheckerText_PickScreenUI[] = _("{JPN}もどる");
static const u8 gFameCheckerText_FameCheckerWillBeClosed[] = _("{JPN}ボイスチェッカーを\nとじます");
static const u8 gFameCheckerText_Cancel[] = _("{JPN}もどる");
static const u8 sText_FameCheckerHelp[] = _("{JPN}ヘルプ");
static const u8 sText_FameCheckerHelpDesc[] = _("{JPN}フロンティアの\nせつめいを みます");

static const u8 sText_FameCheckerName_Birch[] = _("{JPN}オダマキ");
static const u8 sText_FameCheckerName_Rival[] = _("{JPN}ライバル");
static const u8 sText_FameCheckerName_Steven[] = _("{JPN}ダイゴ");
static const u8 sText_FameCheckerName_Maxie[] = _("{JPN}マツブサ");
static const u8 sText_FameCheckerName_Archie[] = _("{JPN}アオギリ");
static const u8 sText_FameCheckerName_Roxanne[] = _("{JPN}ツツジ");
static const u8 sText_FameCheckerName_Brawly[] = _("{JPN}トウキ");
static const u8 sText_FameCheckerName_Wattson[] = _("{JPN}テッセン");
static const u8 sText_FameCheckerName_Flannery[] = _("{JPN}アスナ");
static const u8 sText_FameCheckerName_Norman[] = _("{JPN}センリ");
static const u8 sText_FameCheckerName_Winona[] = _("{JPN}ナギ");
static const u8 sText_FameCheckerName_TateLiza[] = _("{JPN}フウとラン");
static const u8 sText_FameCheckerName_Juan[] = _("{JPN}アダン");
static const u8 sText_FameCheckerName_Wallace[] = _("{JPN}ミクリ");
static const u8 sText_FameCheckerName_Wally[] = _("{JPN}ミツル");
static const u8 sText_FameCheckerName_Scott[] = _("{JPN}エニシダ");

static const u8 sText_FameCheckerQuote_Birch[] = _("{JPN}ポケモンと くらす せかいを\nしらべている");
static const u8 sText_FameCheckerQuote_Rival[] = _("{JPN}いつか {PLAYER}を\nこえてみせる");
static const u8 sText_FameCheckerQuote_Steven[] = _("{JPN}いしと はがねを あいする\nすごうで トレーナー");
static const u8 sText_FameCheckerQuote_Maxie[] = _("{JPN}だいちの ちからを\nもとめる マグマだんの ボス");
static const u8 sText_FameCheckerQuote_Archie[] = _("{JPN}うみの ちからを\nもとめる アクアだんの ボス");
static const u8 sText_FameCheckerQuote_Roxanne[] = _("{JPN}いわを あやつる\nカナズミの ジムリーダー");
static const u8 sText_FameCheckerQuote_Brawly[] = _("{JPN}かくとうを きわめる\nムロの ジムリーダー");
static const u8 sText_FameCheckerQuote_Wattson[] = _("{JPN}でんきと メカに くわしい\nキンセツの ジムリーダー");
static const u8 sText_FameCheckerQuote_Flannery[] = _("{JPN}ほのおの こころを もつ\nフエンの ジムリーダー");
static const u8 sText_FameCheckerQuote_Norman[] = _("{JPN}{PLAYER}の ちちであり\nトウカの ジムリーダー");
static const u8 sText_FameCheckerQuote_Winona[] = _("{JPN}そらを まう\nヒワマキの ジムリーダー");
static const u8 sText_FameCheckerQuote_TateLiza[] = _("{JPN}ふたごで いきを あわせる\nトクサネの ジムリーダー");
static const u8 sText_FameCheckerQuote_Juan[] = _("{JPN}みずを あやつる\nルネの ジムリーダー");
static const u8 sText_FameCheckerQuote_Wallace[] = _("{JPN}みずと うつくしさを\nたいせつにする トレーナー");
static const u8 sText_FameCheckerQuote_Wally[] = _("{JPN}つよくなりたいと ねがう\nやさしい トレーナー");
static const u8 sText_FameCheckerQuote_Scott[] = _("{JPN}バトルフロンティアを\nみまもる おとこ");

static const u8 *const sFameCheckerPersonNamePointers[NUM_FAMECHECKER_PERSONS] = {
    [FAMECHECKER_BIRCH]     = sText_FameCheckerName_Birch,
    [FAMECHECKER_RIVAL]     = sText_FameCheckerName_Rival,
    [FAMECHECKER_STEVEN]    = sText_FameCheckerName_Steven,
    [FAMECHECKER_MAXIE]     = sText_FameCheckerName_Maxie,
    [FAMECHECKER_ARCHIE]    = sText_FameCheckerName_Archie,
    [FAMECHECKER_ROXANNE]   = sText_FameCheckerName_Roxanne,
    [FAMECHECKER_BRAWLY]    = sText_FameCheckerName_Brawly,
    [FAMECHECKER_WATTSON]   = sText_FameCheckerName_Wattson,
    [FAMECHECKER_FLANNERY]  = sText_FameCheckerName_Flannery,
    [FAMECHECKER_NORMAN]    = sText_FameCheckerName_Norman,
    [FAMECHECKER_WINONA]    = sText_FameCheckerName_Winona,
    [FAMECHECKER_TATE_LIZA] = sText_FameCheckerName_TateLiza,
    [FAMECHECKER_JUAN]      = sText_FameCheckerName_Juan,
    [FAMECHECKER_WALLACE]   = sText_FameCheckerName_Wallace,
    [FAMECHECKER_WALLY]     = sText_FameCheckerName_Wally,
    [FAMECHECKER_SCOTT]     = sText_FameCheckerName_Scott,
};

static const u16 sFameCheckerTrainerPicIdxs[] = {
    [FAMECHECKER_BIRCH]     = TRAINER_PIC_PROFESSOR_OAK_FRLG, // Birch is hidden for now.
    [FAMECHECKER_RIVAL]     = TRAINER_PIC_MAY,
    [FAMECHECKER_STEVEN]    = TRAINER_PIC_STEVEN,
    [FAMECHECKER_MAXIE]     = TRAINER_PIC_MAGMA_LEADER_MAXIE,
    [FAMECHECKER_ARCHIE]    = TRAINER_PIC_AQUA_LEADER_ARCHIE,
    [FAMECHECKER_ROXANNE]   = TRAINER_PIC_LEADER_ROXANNE,
    [FAMECHECKER_BRAWLY]    = TRAINER_PIC_LEADER_BRAWLY,
    [FAMECHECKER_WATTSON]   = TRAINER_PIC_LEADER_WATTSON,
    [FAMECHECKER_FLANNERY]  = TRAINER_PIC_LEADER_FLANNERY,
    [FAMECHECKER_NORMAN]    = TRAINER_PIC_LEADER_NORMAN,
    [FAMECHECKER_WINONA]    = TRAINER_PIC_LEADER_WINONA,
    [FAMECHECKER_TATE_LIZA] = TRAINER_PIC_LEADER_TATE_AND_LIZA,
    [FAMECHECKER_JUAN]      = TRAINER_PIC_LEADER_JUAN,
    [FAMECHECKER_WALLACE]   = TRAINER_PIC_CHAMPION_WALLACE,
    [FAMECHECKER_WALLY]     = TRAINER_PIC_WALLY,
    [FAMECHECKER_SCOTT]     = TRAINER_PIC_POKEDUDE,
};

static const u8 sFameCheckerTrainerGenders_Unused[] = {
    [FAMECHECKER_OAK]      = MALE,
    [FAMECHECKER_DAISY]    = FEMALE,
    [FAMECHECKER_BROCK]    = MALE,
    [FAMECHECKER_MISTY]    = FEMALE,
    [FAMECHECKER_LTSURGE]  = MALE,
    [FAMECHECKER_ERIKA]    = FEMALE,
    [FAMECHECKER_KOGA]     = MALE,
    [FAMECHECKER_SABRINA]  = FEMALE,
    [FAMECHECKER_BLAINE]   = MALE,
    [FAMECHECKER_LORELEI]  = FEMALE,
    [FAMECHECKER_BRUNO]    = MALE,
    [FAMECHECKER_AGATHA]   = FEMALE,
    [FAMECHECKER_LANCE]    = MALE,
    [FAMECHECKER_BILL]     = MALE,
    [FAMECHECKER_MRFUJI]   = MALE,
    [FAMECHECKER_GIOVANNI] = MALE,
};

static const u8 *const sFameCheckerNameAndQuotesPointers[2 * NUM_FAMECHECKER_PERSONS] =
{
    sText_FameCheckerName_Birch,
    sText_FameCheckerName_Rival,
    sText_FameCheckerName_Steven,
    sText_FameCheckerName_Maxie,
    sText_FameCheckerName_Archie,
    sText_FameCheckerName_Roxanne,
    sText_FameCheckerName_Brawly,
    sText_FameCheckerName_Wattson,
    sText_FameCheckerName_Flannery,
    sText_FameCheckerName_Norman,
    sText_FameCheckerName_Winona,
    sText_FameCheckerName_TateLiza,
    sText_FameCheckerName_Juan,
    sText_FameCheckerName_Wallace,
    sText_FameCheckerName_Wally,
    sText_FameCheckerName_Scott,

    sText_FameCheckerQuote_Birch,
    sText_FameCheckerQuote_Rival,
    sText_FameCheckerQuote_Steven,
    sText_FameCheckerQuote_Maxie,
    sText_FameCheckerQuote_Archie,
    sText_FameCheckerQuote_Roxanne,
    sText_FameCheckerQuote_Brawly,
    sText_FameCheckerQuote_Wattson,
    sText_FameCheckerQuote_Flannery,
    sText_FameCheckerQuote_Norman,
    sText_FameCheckerQuote_Winona,
    sText_FameCheckerQuote_TateLiza,
    sText_FameCheckerQuote_Juan,
    sText_FameCheckerQuote_Wallace,
    sText_FameCheckerQuote_Wally,
    sText_FameCheckerQuote_Scott
};

static const u8 sFameCheckerFlavorTexts[NUM_FAMECHECKER_PERSONS][FC_FLAVOR_TEXT_COUNT][FC_FLAVOR_TEXT_LENGTH] = {
    [FAMECHECKER_BIRCH] = {
        _("{JPN}ホウエンの\nポケモンはかせ"),
        _("{JPN}ミシロタウンで\nけんきゅうしている"),
        _("{JPN}やせいの すがたを\nしらべるのが すき"),
        _("{JPN}フィールドワークを\nたいせつにする"),
        _("{JPN}{PLAYER}に ポケモンを\nたくした"),
        _("{JPN}いつも げんきに\nそとへ とびだす"),
    },
    [FAMECHECKER_RIVAL] = {
        _("{JPN}オダマキの こどもで\n{PLAYER}の ライバル"),
        _("{JPN}103ばんどうろで\nはじめて たたかう"),
        _("{JPN}{PLAYER}と ともに\nせいちょうする"),
        _("{JPN}ポケモンずかんを\nたすけてくれる"),
        _("{JPN}たびの さきざきで\nしょうぶする"),
        _("{JPN}まじめで\nせわずきな トレーナー"),
    },
    [FAMECHECKER_STEVEN] = {
        _("{JPN}いしを あいする\nすごうで トレーナー"),
        _("{JPN}ダイゴへの てがみを\nとどける"),
        _("{JPN}はがねタイプを\nつかいこなす"),
        _("{JPN}めずらしい いしを\nさがしている"),
        _("{JPN}ホウエンを しずかに\nみまもる"),
        _("{JPN}チャンピオンの\nふうかくを もつ"),
    },
    [FAMECHECKER_MAXIE] = {
        _("{JPN}マグマだんを\nひきいる おとこ"),
        _("{JPN}だいちを ひろげる\nゆめを もつ"),
        _("{JPN}えんとつやまで\nたくらむ"),
        _("{JPN}グラードンを\nねらっている"),
        _("{JPN}れいせいだが\nしんねんは あつい"),
        _("{JPN}ホウエンを めぐる\nそうどうの かなめ"),
    },
    [FAMECHECKER_ARCHIE] = {
        _("{JPN}アクアだんを\nひきいる おとこ"),
        _("{JPN}うみを ひろげる\nゆめを もつ"),
        _("{JPN}カイナや ミナモで\nうごきまわる"),
        _("{JPN}カイオーガを\nねらっている"),
        _("{JPN}ごうかいで\nなかまおもい"),
        _("{JPN}ホウエンを めぐる\nそうどうの かなめ"),
    },
    [FAMECHECKER_ROXANNE] = {
        _("{JPN}カナズミシティの\nジムリーダー"),
        _("{JPN}いわタイプを\nつかいこなす"),
        _("{JPN}トレーナーズスクールで\nまなんだ"),
        _("{JPN}きほんを たいせつに\nたたかう"),
        _("{JPN}ノズパスが\nじまんの あいて"),
        _("{JPN}まじめで\nけんきゅうねっしん"),
    },
    [FAMECHECKER_BRAWLY] = {
        _("{JPN}ムロタウンの\nジムリーダー"),
        _("{JPN}かくとうタイプを\nつかいこなす"),
        _("{JPN}くらい ジムで\nうでを みがく"),
        _("{JPN}サーフィンも\nとくい"),
        _("{JPN}マクノシタと\nともに たたかう"),
        _("{JPN}おおらかで\nからだを きたえる"),
    },
    [FAMECHECKER_WATTSON] = {
        _("{JPN}キンセツシティの\nジムリーダー"),
        _("{JPN}でんきタイプを\nつかいこなす"),
        _("{JPN}メカと しかけが\nだいすき"),
        _("{JPN}ジムを ゆかいな\nしかけにした"),
        _("{JPN}レアコイルが\nじまんの あいて"),
        _("{JPN}わらいごえが\nとても おおきい"),
    },
    [FAMECHECKER_FLANNERY] = {
        _("{JPN}フエンタウンの\nジムリーダー"),
        _("{JPN}ほのおタイプを\nつかいこなす"),
        _("{JPN}ジムリーダーに\nなったばかり"),
        _("{JPN}あつい きもちで\nしょうぶする"),
        _("{JPN}コータスが\nじまんの あいて"),
        _("{JPN}せいいっぱい\nがんばっている"),
    },
    [FAMECHECKER_NORMAN] = {
        _("{JPN}トウカシティの\nジムリーダー"),
        _("{JPN}ノーマルタイプを\nつかいこなす"),
        _("{JPN}{PLAYER}の ちちで\nつよい トレーナー"),
        _("{JPN}せいどうどうと\nしょうぶする"),
        _("{JPN}ケッキングが\nじまんの あいて"),
        _("{JPN}きびしくも\nやさしい ちち"),
    },
    [FAMECHECKER_WINONA] = {
        _("{JPN}ヒワマキシティの\nジムリーダー"),
        _("{JPN}ひこうタイプを\nつかいこなす"),
        _("{JPN}そらを まうように\nたたかう"),
        _("{JPN}ジムには かぜの\nしかけが ある"),
        _("{JPN}チルタリスが\nじまんの あいて"),
        _("{JPN}すがすがしい\nこころを もつ"),
    },
    [FAMECHECKER_TATE_LIZA] = {
        _("{JPN}トクサネシティの\nジムリーダー"),
        _("{JPN}エスパータイプを\nつかいこなす"),
        _("{JPN}ふたごで いきを\nあわせて たたかう"),
        _("{JPN}うちゅうセンターの\nまちを まもる"),
        _("{JPN}ルナトーンと\nソルロックが じまん"),
        _("{JPN}こころを あわせる\nしょうぶが とくい"),
    },
    [FAMECHECKER_JUAN] = {
        _("{JPN}ルネシティの\nジムリーダー"),
        _("{JPN}みずタイプを\nつかいこなす"),
        _("{JPN}うつくしい たたかいを\nたいせつにする"),
        _("{JPN}ミクリの ししょうと\nよばれる"),
        _("{JPN}キングドラが\nじまんの あいて"),
        _("{JPN}おだやかで\nきひんが ある"),
    },
    [FAMECHECKER_WALLACE] = {
        _("{JPN}ルネに ゆかりある\nみずの トレーナー"),
        _("{JPN}うつくしさを\nたいせつにする"),
        _("{JPN}コンテストにも\nふかく かかわる"),
        _("{JPN}ミロカロスが\nじまんの あいて"),
        _("{JPN}ホウエンを\nみちびく そんざい"),
        _("{JPN}はなやかな\nふるまいが めだつ"),
    },
    [FAMECHECKER_WALLY] = {
        _("{JPN}トウカで であう\nやさしい しょうねん"),
        _("{JPN}ポケモンを つかまえる\nれんしゅうをした"),
        _("{JPN}ラルトスと ともに\nつよくなる"),
        _("{JPN}からだは よわいが\nこころは つよい"),
        _("{JPN}チャンピオンロードで\nまっている"),
        _("{JPN}{PLAYER}を こえるため\nどりょくする"),
    },
    [FAMECHECKER_SCOTT] = {
        _("{JPN}フロンティアを\nみまもる おとこ"),
        _("{JPN}つよい トレーナーを\nさがしている"),
        _("{JPN}ホウエンを あるき\nさいのうを みる"),
        _("{JPN}バトルの うでを\nたしかめる"),
        _("{JPN}{PLAYER}に きたいを\nよせている"),
        _("{JPN}バトルフロンティアへ\nみちびく"),
    },
};

static const u8 sText_FameCheckerOrigin_Profile[] = _("{JPN}しょうかい");
static const u8 sText_FameCheckerOrigin_Place[] = _("{JPN}ばしょ");
static const u8 sText_FameCheckerOrigin_Battle[] = _("{JPN}バトル");
static const u8 sText_FameCheckerOrigin_Rumor[] = _("{JPN}うわさ");
static const u8 sText_FameCheckerOrigin_Partner[] = _("{JPN}なかま");
static const u8 sText_FameCheckerOrigin_Word[] = _("{JPN}ひとこと");

static const u8 *const sFameCheckerFlavorOriginLocationTexts[FC_FLAVOR_TEXT_COUNT] = {
    sText_FameCheckerOrigin_Profile,
    sText_FameCheckerOrigin_Place,
    sText_FameCheckerOrigin_Battle,
    sText_FameCheckerOrigin_Rumor,
    sText_FameCheckerOrigin_Partner,
    sText_FameCheckerOrigin_Word,
};

static const u8 *const sFameCheckerFlavorOriginObjectNameTexts[FC_FLAVOR_TEXT_COUNT] = {
    sText_FameCheckerOrigin_Profile,
    sText_FameCheckerOrigin_Place,
    sText_FameCheckerOrigin_Battle,
    sText_FameCheckerOrigin_Rumor,
    sText_FameCheckerOrigin_Partner,
    sText_FameCheckerOrigin_Word,
};

static const u16 sFameCheckerArrayNpcGraphicsIds[] = {
    // BIRCH
    OBJ_EVENT_GFX_PROF_BIRCH,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // RIVAL
    OBJ_EVENT_GFX_MAY_NORMAL,
    OBJ_EVENT_GFX_BRENDAN_NORMAL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // STEVEN
    OBJ_EVENT_GFX_STEVEN,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // MAXIE
    OBJ_EVENT_GFX_MAXIE,
    OBJ_EVENT_GFX_MAGMA_MEMBER_M,
    OBJ_EVENT_GFX_MAGMA_MEMBER_F,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // ARCHIE
    OBJ_EVENT_GFX_ARCHIE,
    OBJ_EVENT_GFX_AQUA_MEMBER_M,
    OBJ_EVENT_GFX_AQUA_MEMBER_F,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // ROXANNE
    OBJ_EVENT_GFX_ROXANNE,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // BRAWLY
    OBJ_EVENT_GFX_BRAWLY,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // WATTSON
    OBJ_EVENT_GFX_WATTSON,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // FLANNERY
    OBJ_EVENT_GFX_FLANNERY,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // NORMAN
    OBJ_EVENT_GFX_NORMAN,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // WINONA
    OBJ_EVENT_GFX_WINONA,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // TATE_LIZA
    OBJ_EVENT_GFX_TATE,
    OBJ_EVENT_GFX_LIZA,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // JUAN
    OBJ_EVENT_GFX_JUAN,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // WALLACE
    OBJ_EVENT_GFX_WALLACE,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // WALLY
    OBJ_EVENT_GFX_WALLY,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    // SCOTT
    OBJ_EVENT_GFX_SCOTT,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
    OBJ_EVENT_GFX_POKE_BALL,
};

static const u8 *const sFlavorTextOriginLocationTexts[] = {
    gFameCheckerFlavorTextOriginLocation_ProfOak0, gFameCheckerFlavorTextOriginLocation_ProfOak1, gFameCheckerFlavorTextOriginLocation_ProfOak2, gFameCheckerFlavorTextOriginLocation_ProfOak3, gFameCheckerFlavorTextOriginLocation_ProfOak4, gFameCheckerFlavorTextOriginLocation_ProfOak5,
    gFameCheckerFlavorTextOriginLocation_Daisy0, gFameCheckerFlavorTextOriginLocation_Daisy1, gFameCheckerFlavorTextOriginLocation_Daisy2, gFameCheckerFlavorTextOriginLocation_Daisy3, gFameCheckerFlavorTextOriginLocation_Daisy4, gFameCheckerFlavorTextOriginLocation_Daisy5,
    gFameCheckerFlavorTextOriginLocation_Brock0, gFameCheckerFlavorTextOriginLocation_Brock1, gFameCheckerFlavorTextOriginLocation_Brock2, gFameCheckerFlavorTextOriginLocation_Brock3, gFameCheckerFlavorTextOriginLocation_Brock4, gFameCheckerFlavorTextOriginLocation_Brock5,
    gFameCheckerFlavorTextOriginLocation_Misty0, gFameCheckerFlavorTextOriginLocation_Misty1, gFameCheckerFlavorTextOriginLocation_Misty2, gFameCheckerFlavorTextOriginLocation_Misty3, gFameCheckerFlavorTextOriginLocation_Misty4, gFameCheckerFlavorTextOriginLocation_Misty5,
    gFameCheckerFlavorTextOriginLocation_LtSurge0, gFameCheckerFlavorTextOriginLocation_LtSurge1, gFameCheckerFlavorTextOriginLocation_LtSurge2, gFameCheckerFlavorTextOriginLocation_LtSurge3, gFameCheckerFlavorTextOriginLocation_LtSurge4, gFameCheckerFlavorTextOriginLocation_LtSurge5,
    gFameCheckerFlavorTextOriginLocation_Erika0, gFameCheckerFlavorTextOriginLocation_Erika1, gFameCheckerFlavorTextOriginLocation_Erika2, gFameCheckerFlavorTextOriginLocation_Erika3, gFameCheckerFlavorTextOriginLocation_Erika4, gFameCheckerFlavorTextOriginLocation_Erika5,
    gFameCheckerFlavorTextOriginLocation_Koga0, gFameCheckerFlavorTextOriginLocation_Koga1, gFameCheckerFlavorTextOriginLocation_Koga2, gFameCheckerFlavorTextOriginLocation_Koga3, gFameCheckerFlavorTextOriginLocation_Koga4, gFameCheckerFlavorTextOriginLocation_Koga5,
    gFameCheckerFlavorTextOriginLocation_Sabrina0, gFameCheckerFlavorTextOriginLocation_Sabrina1, gFameCheckerFlavorTextOriginLocation_Sabrina2, gFameCheckerFlavorTextOriginLocation_Sabrina3, gFameCheckerFlavorTextOriginLocation_Sabrina4, gFameCheckerFlavorTextOriginLocation_Sabrina5,
    gFameCheckerFlavorTextOriginLocation_Blaine0, gFameCheckerFlavorTextOriginLocation_Blaine1, gFameCheckerFlavorTextOriginLocation_Blaine2, gFameCheckerFlavorTextOriginLocation_Blaine3, gFameCheckerFlavorTextOriginLocation_Blaine4, gFameCheckerFlavorTextOriginLocation_Blaine5,
    gFameCheckerFlavorTextOriginLocation_Lorelei0, gFameCheckerFlavorTextOriginLocation_Lorelei1, gFameCheckerFlavorTextOriginLocation_Lorelei2, gFameCheckerFlavorTextOriginLocation_Lorelei3, gFameCheckerFlavorTextOriginLocation_Lorelei4, gFameCheckerFlavorTextOriginLocation_Lorelei5,
    gFameCheckerFlavorTextOriginLocation_Bruno0, gFameCheckerFlavorTextOriginLocation_Bruno1, gFameCheckerFlavorTextOriginLocation_Bruno2, gFameCheckerFlavorTextOriginLocation_Bruno3, gFameCheckerFlavorTextOriginLocation_Bruno4, gFameCheckerFlavorTextOriginLocation_Bruno5,
    gFameCheckerFlavorTextOriginLocation_Agatha0, gFameCheckerFlavorTextOriginLocation_Agatha1, gFameCheckerFlavorTextOriginLocation_Agatha2, gFameCheckerFlavorTextOriginLocation_Agatha3, gFameCheckerFlavorTextOriginLocation_Agatha4, gFameCheckerFlavorTextOriginLocation_Agatha5,
    gFameCheckerFlavorTextOriginLocation_Lance0, gFameCheckerFlavorTextOriginLocation_Lance1, gFameCheckerFlavorTextOriginLocation_Lance2, gFameCheckerFlavorTextOriginLocation_Lance3, gFameCheckerFlavorTextOriginLocation_Lance4, gFameCheckerFlavorTextOriginLocation_Lance5,
    gFameCheckerFlavorTextOriginLocation_Bill0, gFameCheckerFlavorTextOriginLocation_Bill1, gFameCheckerFlavorTextOriginLocation_Bill2, gFameCheckerFlavorTextOriginLocation_Bill3, gFameCheckerFlavorTextOriginLocation_Bill4, gFameCheckerFlavorTextOriginLocation_Bill5,
    gFameCheckerFlavorTextOriginLocation_MrFuji0, gFameCheckerFlavorTextOriginLocation_MrFuji1, gFameCheckerFlavorTextOriginLocation_MrFuji2, gFameCheckerFlavorTextOriginLocation_MrFuji3, gFameCheckerFlavorTextOriginLocation_MrFuji4, gFameCheckerFlavorTextOriginLocation_MrFuji5,
    gFameCheckerFlavorTextOriginLocation_Giovanni0, gFameCheckerFlavorTextOriginLocation_Giovanni1, gFameCheckerFlavorTextOriginLocation_Giovanni2, gFameCheckerFlavorTextOriginLocation_Giovanni3, gFameCheckerFlavorTextOriginLocation_Giovanni4, gFameCheckerFlavorTextOriginLocation_Giovanni5
};

static const u8 *const sFlavorTextOriginObjectNameTexts[] = {
    gFameCheckerFlavorTextOriginObjectName_ProfOak0, gFameCheckerFlavorTextOriginObjectName_ProfOak1, gFameCheckerFlavorTextOriginObjectName_ProfOak2, gFameCheckerFlavorTextOriginObjectName_ProfOak3, gFameCheckerFlavorTextOriginObjectName_ProfOak4, gFameCheckerFlavorTextOriginObjectName_ProfOak5,
    gFameCheckerFlavorTextOriginObjectName_Daisy0, gFameCheckerFlavorTextOriginObjectName_Daisy1, gFameCheckerFlavorTextOriginObjectName_Daisy2, gFameCheckerFlavorTextOriginObjectName_Daisy3, gFameCheckerFlavorTextOriginObjectName_Daisy4, gFameCheckerFlavorTextOriginObjectName_Daisy5,
    gFameCheckerFlavorTextOriginObjectName_Brock0, gFameCheckerFlavorTextOriginObjectName_Brock1, gFameCheckerFlavorTextOriginObjectName_Brock2, gFameCheckerFlavorTextOriginObjectName_Brock3, gFameCheckerFlavorTextOriginObjectName_Brock4, gFameCheckerFlavorTextOriginObjectName_Brock5,
    gFameCheckerFlavorTextOriginObjectName_Misty0, gFameCheckerFlavorTextOriginObjectName_Misty1, gFameCheckerFlavorTextOriginObjectName_Misty2, gFameCheckerFlavorTextOriginObjectName_Misty3, gFameCheckerFlavorTextOriginObjectName_Misty4, gFameCheckerFlavorTextOriginObjectName_Misty5,
    gFameCheckerFlavorTextOriginObjectName_LtSurge0, gFameCheckerFlavorTextOriginObjectName_LtSurge1, gFameCheckerFlavorTextOriginObjectName_LtSurge2, gFameCheckerFlavorTextOriginObjectName_LtSurge3, gFameCheckerFlavorTextOriginObjectName_LtSurge4, gFameCheckerFlavorTextOriginObjectName_LtSurge5,
    gFameCheckerFlavorTextOriginObjectName_Erika0, gFameCheckerFlavorTextOriginObjectName_Erika1, gFameCheckerFlavorTextOriginObjectName_Erika2, gFameCheckerFlavorTextOriginObjectName_Erika3, gFameCheckerFlavorTextOriginObjectName_Erika4, gFameCheckerFlavorTextOriginObjectName_Erika5,
    gFameCheckerFlavorTextOriginObjectName_Koga0, gFameCheckerFlavorTextOriginObjectName_Koga1, gFameCheckerFlavorTextOriginObjectName_Koga2, gFameCheckerFlavorTextOriginObjectName_Koga3, gFameCheckerFlavorTextOriginObjectName_Koga4, gFameCheckerFlavorTextOriginObjectName_Koga5,
    gFameCheckerFlavorTextOriginObjectName_Sabrina0, gFameCheckerFlavorTextOriginObjectName_Sabrina1, gFameCheckerFlavorTextOriginObjectName_Sabrina2, gFameCheckerFlavorTextOriginObjectName_Sabrina3, gFameCheckerFlavorTextOriginObjectName_Sabrina4, gFameCheckerFlavorTextOriginObjectName_Sabrina5,
    gFameCheckerFlavorTextOriginObjectName_Blaine0, gFameCheckerFlavorTextOriginObjectName_Blaine1, gFameCheckerFlavorTextOriginObjectName_Blaine2, gFameCheckerFlavorTextOriginObjectName_Blaine3, gFameCheckerFlavorTextOriginObjectName_Blaine4, gFameCheckerFlavorTextOriginObjectName_Blaine5,
    gFameCheckerFlavorTextOriginObjectName_Lorelei0, gFameCheckerFlavorTextOriginObjectName_Lorelei1, gFameCheckerFlavorTextOriginObjectName_Lorelei2, gFameCheckerFlavorTextOriginObjectName_Lorelei3, gFameCheckerFlavorTextOriginObjectName_Lorelei4, gFameCheckerFlavorTextOriginObjectName_Lorelei5,
    gFameCheckerFlavorTextOriginObjectName_Bruno0, gFameCheckerFlavorTextOriginObjectName_Bruno1, gFameCheckerFlavorTextOriginObjectName_Bruno2, gFameCheckerFlavorTextOriginObjectName_Bruno3, gFameCheckerFlavorTextOriginObjectName_Bruno4, gFameCheckerFlavorTextOriginObjectName_Bruno5,
    gFameCheckerFlavorTextOriginObjectName_Agatha0, gFameCheckerFlavorTextOriginObjectName_Agatha1, gFameCheckerFlavorTextOriginObjectName_Agatha2, gFameCheckerFlavorTextOriginObjectName_Agatha3, gFameCheckerFlavorTextOriginObjectName_Agatha4, gFameCheckerFlavorTextOriginObjectName_Agatha5,
    gFameCheckerFlavorTextOriginObjectName_Lance0, gFameCheckerFlavorTextOriginObjectName_Lance1, gFameCheckerFlavorTextOriginObjectName_Lance2, gFameCheckerFlavorTextOriginObjectName_Lance3, gFameCheckerFlavorTextOriginObjectName_Lance4, gFameCheckerFlavorTextOriginObjectName_Lance5,
    gFameCheckerFlavorTextOriginObjectName_Bill0, gFameCheckerFlavorTextOriginObjectName_Bill1, gFameCheckerFlavorTextOriginObjectName_Bill2, gFameCheckerFlavorTextOriginObjectName_Bill3, gFameCheckerFlavorTextOriginObjectName_Bill4, gFameCheckerFlavorTextOriginObjectName_Bill5,
    gFameCheckerFlavorTextOriginObjectName_MrFuji0, gFameCheckerFlavorTextOriginObjectName_MrFuji1, gFameCheckerFlavorTextOriginObjectName_MrFuji2, gFameCheckerFlavorTextOriginObjectName_MrFuji3, gFameCheckerFlavorTextOriginObjectName_MrFuji4, gFameCheckerFlavorTextOriginObjectName_MrFuji5,
    gFameCheckerFlavorTextOriginObjectName_Giovanni0, gFameCheckerFlavorTextOriginObjectName_Giovanni1, gFameCheckerFlavorTextOriginObjectName_Giovanni2, gFameCheckerFlavorTextOriginObjectName_Giovanni3, gFameCheckerFlavorTextOriginObjectName_Giovanni4, gFameCheckerFlavorTextOriginObjectName_Giovanni5
};

static const struct SpriteSheet sUISpriteSheets[] = {
    {sSelectorCursorSpriteGfx,   0x400, SPRITETAG_SELECTOR_CURSOR},
    {sQuestionMarkSpriteGfx,     0x100, SPRITETAG_QUESTION_MARK},
    {sSpinningPokeballSpriteGfx, 0x1e0, SPRITETAG_SPINNING_POKEBALL},
    {sDaisySpriteGfx,            0x800, SPRITETAG_DAISY},
    {sFujiSpriteGfx,             0x800, SPRITETAG_FUJI},
    {sOakSpriteGfx,              0x800, SPRITETAG_OAK},
    {sBillSpriteGfx,             0x800, SPRITETAG_BILL},
    {}
};

static const struct SpritePalette sUISpritePalettes[] = {
    {sSelectorCursorSpritePalette, SPRITETAG_SELECTOR_CURSOR},
    {sSpinningPokeballSpritePalette, SPRITETAG_SPINNING_POKEBALL},
    {}
};

static const struct BgTemplate sUIBgTemplates[4] = {
    {
        .bg = 3,
        .charBaseIndex = 3,
        .mapBaseIndex =  30,
        .screenSize = 0,
        .paletteMode = FALSE,
        .priority = 3,
        .baseTile = 0x000
    },
    {
        .bg = 2,
        .charBaseIndex = 3,
        .mapBaseIndex =  27,
        .screenSize = 0,
        .paletteMode = FALSE,
        .priority = 2,
        .baseTile = 0x000
    },
    {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex =  28,
        .screenSize = 1,
        .paletteMode = FALSE,
        .priority = 0,
        .baseTile = 0x000
    },
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex =  31,
        .screenSize = 0,
        .paletteMode = FALSE,
        .priority = 2,
        .baseTile = 0x000
    },
};

static const struct WindowTemplate sUIWindowTemplates[] = {
    [FCWINDOWID_LIST] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 3,
        .width = 8,
        .height = 10,
        .paletteNum = 15,
        .baseBlock = 20
    },
    [FCWINDOWID_UIHELP] = {
        .bg = 0,
        .tilemapLeft = 6,
        .tilemapTop = 0,
        .width = 24,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 20 + 8 * 10
    },
    [FCWINDOWID_MSGBOX] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 20 + 8 * 10 + 24 * 2
    },
    [FCWINDOWID_ICONDESC] = {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 10,
        .width = 11,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 20 + 8 * 10 + 24 * 2 + 26 * 4
    },
    DUMMY_WIN_TEMPLATE
};

static const union AnimCmd sSelectorCursorAnim0[] = {
    ANIMCMD_FRAME( 0, 15),
    ANIMCMD_FRAME(16, 15),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sSelectorCursorAnims[] = {
    sSelectorCursorAnim0
};

static const struct OamData sSelectorCursorOamData = {
    .size = 2,
    .priority = 2
};

static const struct SpriteTemplate sSpriteTemplate_SelectorCursor = {
    SPRITETAG_SELECTOR_CURSOR, SPRITETAG_SELECTOR_CURSOR, &sSelectorCursorOamData, sSelectorCursorAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static const u8 sUnused[8] = {}; // ???

static const struct OamData sQuestionMarkTileOamData = {
    .shape = ST_OAM_V_RECTANGLE,
    .size = 2,
    .priority = 2
};

static const union AnimCmd sQuestionMarkTileAnim0[] = {
    ANIMCMD_FRAME( 0, 10),
    ANIMCMD_END
};

static const union AnimCmd *const sQuestionMarkTileAnims[] = {
    sQuestionMarkTileAnim0
};

static const struct SpriteTemplate sQuestionMarkTileSpriteTemplate = {
    SPRITETAG_QUESTION_MARK, 0xffff, &sQuestionMarkTileOamData, sQuestionMarkTileAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static const union AnimCmd sSpinningPokeballAnim0[] = {
    ANIMCMD_FRAME( 0, 10),
    ANIMCMD_END
};

static const union AnimCmd *const sSpinningPokeballAnims[] = {
    sSpinningPokeballAnim0
};

static const struct OamData sSpinningPokeballOamData = {
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .size = 2
};

static const union AffineAnimCmd sSpinningPokeballAffineAnim0[] = {
    AFFINEANIMCMD_FRAME(0, 0, 4, 20),
    AFFINEANIMCMD_JUMP(0)
};

static const union AffineAnimCmd *const sSpinningPokeballAffineAnims[] = {
    sSpinningPokeballAffineAnim0
};

static const struct SpriteTemplate sSpinningPokeballSpriteTemplate = {
    SPRITETAG_SPINNING_POKEBALL, SPRITETAG_SPINNING_POKEBALL, &sSpinningPokeballOamData, sSpinningPokeballAnims, NULL, sSpinningPokeballAffineAnims, SpriteCB_FCSpinningPokeball
};

static const union AnimCmd sDaisyFujiOakBillAnim0[] = {
    ANIMCMD_FRAME( 0, 15),
    ANIMCMD_END
};

static const union AnimCmd *const sDaisyFujiOakBillAnims[] = {
    sDaisyFujiOakBillAnim0
};

static const struct OamData sDaisyFujiOakBillOamData = {
    .size = 3
};

static const struct SpriteTemplate sDaisySpriteTemplate = {
    SPRITETAG_DAISY, 0xffff, &sDaisyFujiOakBillOamData, sDaisyFujiOakBillAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static const struct SpriteTemplate sFujiSpriteTemplate = {
    SPRITETAG_FUJI, 0xffff, &sDaisyFujiOakBillOamData, sDaisyFujiOakBillAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static const struct SpriteTemplate sOakSpriteTemplate = {
    SPRITETAG_OAK, 0xffff, &sDaisyFujiOakBillOamData, sDaisyFujiOakBillAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static const struct SpriteTemplate sBillSpriteTemplate = {
    SPRITETAG_BILL, 0xffff, &sDaisyFujiOakBillOamData, sDaisyFujiOakBillAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static void FC_VBlankCallback(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void MainCB2_FameCheckerMain(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void UseFameChecker(MainCallback savedCallback)
{
    NormalizeFameCheckerSaveData();
    SetVBlankCallback(NULL);
    sFameCheckerData = AllocZeroed(sizeof(struct FameCheckerData));
    sFameCheckerData->savedCallback = savedCallback;
    sFameCheckerData->listMenuCurIdx = 0;
    sFameCheckerData->listMenuTopIdx2 = 0;
    sFameCheckerData->listMenuDrawnSelIdx = 0;
    sFameCheckerData->viewingFlavorText = FALSE;
    PlaySE(SE_M_SWIFT);
    SetMainCallback2(MainCB2_LoadFameChecker);
}

static void NormalizeFameCheckerSaveData(void)
{
    u8 i;

    for (i = 0; i < NUM_FAMECHECKER_PERSONS; i++)
    {
        if (gSaveBlock1Ptr->fameChecker[i].pickState > FCPICKSTATE_COLORED)
            gSaveBlock1Ptr->fameChecker[i].pickState = FCPICKSTATE_NO_DRAW;
        gSaveBlock1Ptr->fameChecker[i].flavorTextFlags &= 0x3F;
    }

    UnlockAllFameCheckerPeople();
}

static void MainCB2_LoadFameChecker(void)
{
    switch (gMain.state)
    {
        case 0:
            SetVBlankCallback(NULL);
            FCSetup_ClearVideoRegisters();
            gMain.state++;
            break;
        case 1:
            FCSetup_ResetTasksAndSpriteResources();
            gMain.state++;
            break;
        case 2:
            sBg3TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);     // 256x256
            sBg1TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE * 2); // 512x256
            sBg2TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);     // 256x256
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sUIBgTemplates, NELEMS(sUIBgTemplates));
            SetBgTilemapBuffer(3, sBg3TilemapBuffer);
            SetBgTilemapBuffer(2, sBg2TilemapBuffer);
            SetBgTilemapBuffer(1, sBg1TilemapBuffer);
            FCSetup_ResetBGCoords();
            gMain.state++;
            break;
        case 3:
            LoadBgTiles(3, gFameCheckerBgTiles, sizeof(gFameCheckerBgTiles), 0);
            CopyToBgTilemapBufferRect(3, gFameCheckerBg3Tilemap, 0, 0, 32, 32);
            LoadPalette(&gFameCheckerBgPals[0], BG_PLTT_ID(0), 2 * PLTT_SIZE_4BPP);
            LoadPalette(&gFameCheckerBgPals[16], BG_PLTT_ID(1), PLTT_SIZE_4BPP);
            CopyToBgTilemapBufferRect(2, gFameCheckerBg2Tilemap, 0, 0, 32, 32);
            CopyToBgTilemapBufferRect_ChangePalette(1, sFameCheckerTilemap, 30, 0, 32, 32, 0x11);
            LoadPalette(GetTextWindowPalette(2), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
            gMain.state++;
            break;
        case 4:
            if (IsDma3ManagerBusyWithBgCopy() != TRUE)
            {
                ShowBg(0);
                ShowBg(1);
                ShowBg(2);
                ShowBg(3);
                CopyBgTilemapBufferToVram(3);
                CopyBgTilemapBufferToVram(2);
                CopyBgTilemapBufferToVram(1);
                gMain.state++;
            }
            break;
        case 5:
            InitWindows(sUIWindowTemplates);
            DeactivateAllTextPrinters();
            Setup_DrawMsgAndListBoxes();
            sListMenuItems = AllocZeroed((NUM_FAMECHECKER_PERSONS + 2) * sizeof(struct ListMenuItem));
            FC_CreateListMenu();
            gMain.state++;
            break;
        case 6:
            LoadUISpriteSheetsAndPalettes();
            CreateAllFlavorTextIcons(FAMECHECKER_OAK);
            WipeMsgBoxAndTransfer();
            BeginNormalPaletteFade(PALETTES_ALL,0, 16, 0, 0);
            gMain.state++;
            break;
        case 7:
            FCSetup_TurnOnDisplay();
            SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD);
            SetGpuReg(REG_OFFSET_BLDALPHA, 0x07);
            SetGpuReg(REG_OFFSET_BLDY, 0x08);
            SetVBlankCallback(FC_VBlankCallback);
            sFameCheckerData->listMenuTopIdx = 0;
            FC_CreateScrollIndicatorArrowPair();
            UpdateInfoBoxTilemap(1, 4);
            CreateTask(Task_WaitFadeOnInit, 0x08);
            SetMainCallback2(MainCB2_FameCheckerMain);
            gMain.state = 0;
            break;
    }
}

static void LoadUISpriteSheetsAndPalettes(void)
{
    LoadSpriteSheets(sUISpriteSheets);
    LoadSpritePalettes(sUISpritePalettes);
}

static void Task_WaitFadeOnInit(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_TopMenuHandleInput;
}

static void Task_TopMenuHandleInput(u8 taskId)
{
    u16 cursorPos;
    u8 i;
    struct Task *task = &gTasks[taskId];
    s16 * data = gTasks[taskId].data;
    if (FindTaskIdByFunc(Task_FCOpenOrCloseInfoBox) == 0xFF)
    {
        RunTextPrinters();
        if ((JOY_NEW(SELECT_BUTTON)) && !sFameCheckerData->inPickMode && sFameCheckerData->savedCallback != CB2_BagMenuFromStartMenu)
            task->func = Task_StartToCloseFameChecker;
        else if (JOY_NEW(START_BUTTON))
        {
            cursorPos = FameCheckerGetCursorY();
            if (TryExitPickMode(taskId) == TRUE)
            {
                PlaySE(SE_M_LOCK_ON);
            }
            else if (sFameCheckerData->unlockedPersons[cursorPos] == FAMECHECKER_HELP_ENTRY)
            {
                task->func = Task_StartUltraHelpFromFameChecker;
            }
            else if (sFameCheckerData->unlockedPersons[cursorPos] < NUM_FAMECHECKER_PERSONS)
            {
                PlaySE(SE_M_LOCK_ON);
                FillWindowPixelRect(FCWINDOWID_ICONDESC, PIXEL_FILL(0), 0, 0, 88, 32);
                FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_ICONDESC);
                UpdateInfoBoxTilemap(2, 4);
                UpdateInfoBoxTilemap(1, 5);
                PrintUIHelp(1);
                task->data[2] = CreatePersonPicSprite(sFameCheckerData->unlockedPersons[cursorPos]);
                gSprites[task->data[2]].x2 = 0xF0;
                gSprites[task->data[2]].data[0] = 1;
                task->data[3] = CreateSpinningPokeballSprite();
                gSprites[task->data[3]].x2 = 0xF0;
                gSprites[task->data[3]].data[0] = 1;
                task->func = Task_EnterPickMode;
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            cursorPos = ListMenu_ProcessInput(0);
            if (sFameCheckerData->unlockedPersons[cursorPos] == FAMECHECKER_CANCEL_ENTRY)
                task->func = Task_StartToCloseFameChecker;
            else if (sFameCheckerData->unlockedPersons[cursorPos] == FAMECHECKER_HELP_ENTRY)
                task->func = Task_StartUltraHelpFromFameChecker;
            else if (sFameCheckerData->inPickMode)
            {
                if (!IsTextPrinterActiveOnWindow(FCWINDOWID_MSGBOX) && HasUnlockedAllFlavorTextsForCurrentPerson() == TRUE)
                    GetPickModeText();
            }
            else if (sFameCheckerData->personHasUnlockedPanels)
            {
                PlaySE(SE_SELECT);
                task->data[0] = CreateFlavorTextIconSelectorCursorSprite(task->data[1]);
                for (i = 0; i < 6; i++)
                {
                    if (i != task->data[1])
                        SetMessageSelectorIconObjMode(sFameCheckerData->spriteIds[i], ST_OAM_OBJ_BLEND);
                }
                gIconDescriptionBoxIsOpen = 0xFF;
                PlaceListMenuCursor(FALSE);
                PrintUIHelp(2);
                if (sFameCheckerData->spriteIds[task->data[1]] < MAX_SPRITES
                 && gSprites[sFameCheckerData->spriteIds[task->data[1]]].data[1] != 0xFF) // not a ? tile
                {
                    PrintSelectedNameInBrightGreen(taskId);
                    UpdateIconDescriptionBox(data[1]);
                }
                FreeListMenuSelectorArrowPairResources();
                task->func = Task_FlavorTextDisplayHandleInput;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            if (TryExitPickMode(taskId) != TRUE)
                task->func = Task_StartToCloseFameChecker;
        }
        else
            ListMenu_ProcessInput(0);
    }
}

static bool8 TryExitPickMode(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (sFameCheckerData->inPickMode)
    {
        gSprites[task->data[2]].data[0] = 2;
        gSprites[task->data[2]].x2 += 10;
        gSprites[task->data[3]].data[0] = 2;
        gSprites[task->data[3]].x2 += 10;
        WipeMsgBoxAndTransfer();
        task->func = Task_ExitPickMode;
        MessageBoxPrintEmptyText();
        sFameCheckerData->pickModeOverCancel = FALSE;
        return TRUE;
    }
    return FALSE;
}

static void MessageBoxPrintEmptyText(void)
{
    AddTextPrinterParameterized2(FCWINDOWID_MSGBOX, FONT_NORMAL, gFameCheckerText_ClearTextbox, 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
}

static void Task_EnterPickMode(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (gSprites[task->data[2]].data[0] == 0)
    {
        GetPickModeText();
        sFameCheckerData->inPickMode = TRUE;
        task->func = Task_TopMenuHandleInput;
    }
    else
        ChangeBgX(1, 0xA00, 1);
}

static void Task_ExitPickMode(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (GetBgX(1) != 0)
        ChangeBgX(1, 0xA00, 2);
    else
        ChangeBgX(1, 0x000, 0);
    if (gSprites[task->data[2]].data[0] == 0)
    {
        if (sFameCheckerData->personHasUnlockedPanels)
            PrintUIHelp(0);
        UpdateInfoBoxTilemap(1, 4);
        UpdateInfoBoxTilemap(2, 2);
        sFameCheckerData->inPickMode = FALSE;
        DestroyPersonPicSprite(taskId, FameCheckerGetCursorY());
        task->func = Task_TopMenuHandleInput;
        gSprites[task->data[3]].callback = SpriteCB_DestroySpinningPokeball;
    }
}

static void Task_FlavorTextDisplayHandleInput(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    s16 *data = gTasks[taskId].data;

    RunTextPrinters();
    if (JOY_NEW(A_BUTTON) && !IsTextPrinterActiveOnWindow(FCWINDOWID_MSGBOX))
    {
        u8 spriteId = sFameCheckerData->spriteIds[data[1]];
        if (gSprites[spriteId].data[1] != 0xFF)
            PrintSelectedNameInBrightGreen(taskId);
    }
    if (JOY_NEW(B_BUTTON))
    {
        u8 i;
        PlaySE(SE_SELECT);
        for (i = 0; i < 6; i++)
            SetMessageSelectorIconObjMode(sFameCheckerData->spriteIds[i], ST_OAM_OBJ_NORMAL);
        WipeMsgBoxAndTransfer();
        gSprites[task->data[0]].callback = SpriteCB_DestroyFlavorTextIconSelectorCursor;
        if (gIconDescriptionBoxIsOpen != 0xFF)
            UpdateIconDescriptionBoxOff();
        PlaceListMenuCursor(TRUE);
        PrintUIHelp(0);
        FC_CreateScrollIndicatorArrowPair();
        MessageBoxPrintEmptyText();
        task->func = Task_TopMenuHandleInput;
    }
    else if (JOY_NEW(DPAD_UP) || JOY_NEW(DPAD_DOWN))
    {
        if (task->data[1] >= 3)
        {
            task->data[1] -= 3;
            FC_MoveSelectorCursor(taskId, 0, -0x1b);
        }
        else
        {
            task->data[1] += 3;
            FC_MoveSelectorCursor(taskId, 0, +0x1b);
        }
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (task->data[1] == 0 || task->data[1] % 3 == 0)
        {
            task->data[1] += 2;
            FC_MoveSelectorCursor(taskId, +0x5e, 0);
        }
        else
        {
            task->data[1]--;
            FC_MoveSelectorCursor(taskId, -0x2f, 0);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        if ((task->data[1] + 1) % 3 == 0)
        {
            task->data[1] -= 2;
            FC_MoveSelectorCursor(taskId, -0x5e, 0);
        }
        else
        {
            task->data[1]++;
            FC_MoveSelectorCursor(taskId, +0x2f, 0);
        }
    }
}

static void FC_MoveSelectorCursor(u8 taskId, s8 dx, s8 dy)
{
    u8 i;
    s16 *data = gTasks[taskId].data;
    PlaySE(SE_M_SWAGGER2);
    gSprites[data[0]].x += dx;
    gSprites[data[0]].y += dy;
    for (i = 0; i < 6; i++)
        SetMessageSelectorIconObjMode(sFameCheckerData->spriteIds[i], ST_OAM_OBJ_BLEND);
    FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
    MessageBoxPrintEmptyText();
    if (SetMessageSelectorIconObjMode(sFameCheckerData->spriteIds[data[1]], ST_OAM_OBJ_NORMAL) == TRUE)
    {
        PrintSelectedNameInBrightGreen(taskId);
        UpdateIconDescriptionBox(data[1]);
    }
    else if (gIconDescriptionBoxIsOpen != 0xFF)
        UpdateIconDescriptionBoxOff();
}

static void GetPickModeText(void)
{
    s32 whichText = 0;
    u16 who = FameCheckerGetCursorY();
    if (gSaveBlock1Ptr->fameChecker[sFameCheckerData->unlockedPersons[who]].pickState != FCPICKSTATE_COLORED)
    {
        WipeMsgBoxAndTransfer();
        MessageBoxPrintEmptyText();
    }
    else
    {
        FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
        if (HasUnlockedAllFlavorTextsForCurrentPerson() == TRUE)
            whichText = NUM_FAMECHECKER_PERSONS;
        StringExpandPlaceholders(gStringVar4, sFameCheckerNameAndQuotesPointers[sFameCheckerData->unlockedPersons[who] + whichText]);
        AddTextPrinterParameterized2(FCWINDOWID_MSGBOX, FONT_NORMAL, gStringVar4, GetPlayerTextSpeedDelay(), NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
        FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
    }
}

static void PrintSelectedNameInBrightGreen(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 cursorPos = FameCheckerGetCursorY();
    FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
    StringExpandPlaceholders(gStringVar4, sFameCheckerFlavorTexts[sFameCheckerData->unlockedPersons[cursorPos]][data[1]]);
    AddTextPrinterParameterized2(FCWINDOWID_MSGBOX, FONT_NORMAL, gStringVar4, GetPlayerTextSpeedDelay(), NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
}

static void WipeMsgBoxAndTransfer(void)
{
    FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
    FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
}

static void Setup_DrawMsgAndListBoxes(void)
{
    LoadMessageBoxAndBorderGfx();
    DrawDialogueFrame(FCWINDOWID_MSGBOX, TRUE);
    FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
    FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_LIST);
}

static void FC_PutWindowTilemapAndCopyWindowToVramMode3(u8 windowId)
{
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_FULL);
}

static bool8 SetMessageSelectorIconObjMode(u8 spriteId, u8 objMode)
{
    if (spriteId >= MAX_SPRITES)
        return FALSE;

    if (gSprites[spriteId].data[1] != 0xFF)
    {
        gSprites[spriteId].oam.objMode = objMode;
        return TRUE;
    }
    return FALSE;
}

static void Task_StartToCloseFameChecker(u8 taskId)
{
    PlaySE(SE_M_SWIFT);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, 0);
    gTasks[taskId].func = Task_DestroyAssetsAndCloseFameChecker;
}

static void Task_StartUltraHelpFromFameChecker(u8 taskId)
{
    PlaySE(SE_SELECT);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, 0);
    gTasks[taskId].tExitToUltraHelp = TRUE;
    gTasks[taskId].func = Task_DestroyAssetsAndCloseFameChecker;
}

static void Task_DestroyAssetsAndCloseFameChecker(u8 taskId)
{
    u8 i;

    if (!gPaletteFade.active)
    {
        bool8 exitToUltraHelp = gTasks[taskId].tExitToUltraHelp;
        MainCallback savedCallback = sFameCheckerData->savedCallback;

        if (sFameCheckerData->inPickMode)
        {
            DestroyPersonPicSprite(taskId, FameCheckerGetCursorY());
            FreeSpriteOamMatrix(&gSprites[gTasks[taskId].data[3]]);
            DestroySprite(&gSprites[gTasks[taskId].data[3]]);
        }
        for (i = 0; i < 6; i++)
        {
            if (sFameCheckerData->spriteIds[i] < MAX_SPRITES)
                DestroySprite(&gSprites[sFameCheckerData->spriteIds[i]]);
            FreeFameCheckerObjectPalette(sFameCheckerData->spriteGraphicsIds[i]);
        }
        FreeNonTrainerPicTiles();
        FreeSpinningPokeballSpriteResources();
        FreeSelectionCursorSpriteResources();
        FreeQuestionMarkSpriteResources();
        FreeListMenuSelectorArrowPairResources();
        if (exitToUltraHelp)
            sFameCheckerReturnCallback = savedCallback;
        else
            SetMainCallback2(savedCallback);
        DestroyListMenuTask(sFameCheckerData->listMenuTaskId, NULL, NULL);
        Free(sBg3TilemapBuffer);
        Free(sBg1TilemapBuffer);
        Free(sBg2TilemapBuffer);
        Free(sFameCheckerData);
        Free(sListMenuItems);
        FC_DestroyWindow(FCWINDOWID_LIST);
        FC_DestroyWindow(FCWINDOWID_UIHELP);
        FC_DestroyWindow(FCWINDOWID_MSGBOX);
        FC_DestroyWindow(FCWINDOWID_ICONDESC);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        if (exitToUltraHelp)
            StartUltraHelp(ULTRA_HELP_TOPIC_FRONTIER_RULES, CB2_ReturnToFameCheckerFromUltraHelp);
    }
}

static void CB2_ReturnToFameCheckerFromUltraHelp(void)
{
    MainCallback savedCallback = sFameCheckerReturnCallback;

    if (savedCallback == NULL)
        savedCallback = CB2_ReturnToField;
    sFameCheckerReturnCallback = NULL;
    UseFameChecker(savedCallback);
}

static void FC_DestroyWindow(u8 windowId)
{
    FillWindowPixelBuffer(windowId, 0);
    ClearWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_GFX);
    RemoveWindow(windowId);
}

static u8 AdjustGiovanniIndexIfBeatenInGym(u8 a0)
{
    return a0;
}

static void PrintUIHelp(u8 state)
{
    s32 width;
    const u8 * src = gFameCheckerText_MainScreenUI;
    if (state != 0)
    {
        src = gFameCheckerText_FlavorTextUI;
        if (state == 1)
            src = gFameCheckerText_PickScreenUI;
    }
    width = GetStringWidth(FONT_SMALL, src, 0);
    FillWindowPixelRect(FCWINDOWID_UIHELP, PIXEL_FILL(0), 0, 0, 0xc0, 0x10);
    AddTextPrinterParameterized4(FCWINDOWID_UIHELP, FONT_SMALL, 188 - width, 0, 0, 2, sTextColor_White, -1, src);
    FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_UIHELP);
}

static void DestroyAllFlavorTextIcons(void)
{
    u8 i;
    for (i = 0; i < 6; i++)
    {
        if (sFameCheckerData->spriteIds[i] < MAX_SPRITES)
            DestroySprite(&gSprites[sFameCheckerData->spriteIds[i]]);
        FreeFameCheckerObjectPalette(sFameCheckerData->spriteGraphicsIds[i]);
        sFameCheckerData->spriteGraphicsIds[i] = FC_ICON_GRAPHICS_NONE;
        sFameCheckerData->spriteIds[i] = MAX_SPRITES;
    }
}

static u8 CreateFameCheckerObject(u16 graphicsId, u8 localId, s16 x, s16 y)
{
    u8 spriteId;
    u8 paletteNum;
    struct Sprite *sprite;
    struct SpriteTemplate *spriteTemplate;
    const struct SubspriteTable *subspriteTables;
    const struct ObjectEventGraphicsInfo *graphicsInfo;

    graphicsInfo = GetObjectEventGraphicsInfo(graphicsId);
    spriteTemplate = &sFameCheckerObjectSpriteTemplates[localId];
    CopyObjectGraphicsInfoToSpriteTemplate(graphicsId, SpriteCallbackDummy, spriteTemplate, &subspriteTables);

    // Fame Checker creates and destroys these small icons frequently. Keep the
    // template owned by this file and explicitly free the object palette on swap.
    paletteNum = LoadObjectEventPalette(spriteTemplate->paletteTag);
    if (spriteTemplate->paletteTag != TAG_NONE && paletteNum == 0xFF)
        return MAX_SPRITES;

    spriteId = CreateSprite(spriteTemplate, x, y, 0);
    if (spriteId != MAX_SPRITES)
    {
        sprite = &gSprites[spriteId];
        if (spriteTemplate->paletteTag != TAG_NONE)
            sprite->oam.paletteNum = paletteNum;
        if (subspriteTables != NULL)
        {
            SetSubspriteTables(sprite, subspriteTables);
            sprite->subspriteMode = SUBSPRITES_IGNORE_PRIORITY;
        }
        sprite->data[0] = localId;
        sprite->data[1] = 0;
        sprite->oam.priority = 2;
        sprite->centerToCornerVecY = -(graphicsInfo->height >> 1);
    }

    return spriteId;
}

static void FreeFameCheckerObjectPalette(u16 graphicsId)
{
    const struct ObjectEventGraphicsInfo *graphicsInfo;

    if (graphicsId == FC_ICON_GRAPHICS_NONE)
        return;

    graphicsInfo = GetObjectEventGraphicsInfo(graphicsId);
    if (graphicsInfo->paletteTag != TAG_NONE)
        FreeSpritePaletteByTag(graphicsInfo->paletteTag);
}

static bool8 CreateAllFlavorTextIcons(u8 who)
{
    bool8 result = FALSE;
    u8 i;

    if (sFameCheckerData->unlockedPersons[who] == FAMECHECKER_BIRCH)
    {
        for (i = 0; i < 6; i++)
        {
            sFameCheckerData->spriteIds[i] = PlaceQuestionMarkTile(
                47 * (i % 3) + 0x72,
                27 * (i / 3) + 0x1F
            );
            sFameCheckerData->spriteGraphicsIds[i] = FC_ICON_GRAPHICS_NONE;
            if (sFameCheckerData->spriteIds[i] < MAX_SPRITES)
                gSprites[sFameCheckerData->spriteIds[i]].data[1] = 0xFF;
        }
        sFameCheckerData->personHasUnlockedPanels = FALSE;
        PrintUIHelp(1);
        return FALSE;
    }

    for (i = 0; i < 6; i++)
    {
        if ((gSaveBlock1Ptr->fameChecker[sFameCheckerData->unlockedPersons[who]].flavorTextFlags >> i) & 1)
        {
            u16 graphicsId = sFameCheckerArrayNpcGraphicsIds[sFameCheckerData->unlockedPersons[who] * 6 + i];

            sFameCheckerData->spriteIds[i] = CreateFameCheckerObject(
                graphicsId,
                i,
                47 * (i % 3) + 0x72,
                27 * (i / 3) + 0x1F
            );
            if (sFameCheckerData->spriteIds[i] == MAX_SPRITES)
            {
                sFameCheckerData->spriteGraphicsIds[i] = FC_ICON_GRAPHICS_NONE;
                sFameCheckerData->spriteIds[i] = PlaceQuestionMarkTile(
                    47 * (i % 3) + 0x72,
                    27 * (i / 3) + 0x1F
                );
                if (sFameCheckerData->spriteIds[i] < MAX_SPRITES)
                    gSprites[sFameCheckerData->spriteIds[i]].data[1] = 0xFF;
            }
            else
            {
                sFameCheckerData->spriteGraphicsIds[i] = graphicsId;
                result = TRUE;
            }
        }
        else
        {
            sFameCheckerData->spriteIds[i] = PlaceQuestionMarkTile(
                47 * (i % 3) + 0x72,
                27 * (i / 3) + 0x1F
            );
            sFameCheckerData->spriteGraphicsIds[i] = FC_ICON_GRAPHICS_NONE;
            if (sFameCheckerData->spriteIds[i] < MAX_SPRITES)
                gSprites[sFameCheckerData->spriteIds[i]].data[1] = 0xFF;
        }
    }
    if (result == TRUE)
    {
        sFameCheckerData->personHasUnlockedPanels = TRUE;
        if (sFameCheckerData->inPickMode)
            PrintUIHelp(1);
        else
            PrintUIHelp(0);
    }
    else
    {
        sFameCheckerData->personHasUnlockedPanels = FALSE;
        PrintUIHelp(1);
    }
    return result;
}

void ResetFameChecker(void)
{
    u8 i;
    for (i = 0; i < NUM_FAMECHECKER_PERSONS; i++)
    {
        gSaveBlock1Ptr->fameChecker[i].pickState = FCPICKSTATE_NO_DRAW;
        gSaveBlock1Ptr->fameChecker[i].flavorTextFlags = 0;
        gSaveBlock1Ptr->fameChecker[i].unk_0_E = 0;
    }
    UnlockAllFameCheckerPeople();
}

static void UnlockAllFameCheckerPeople(void)
{
    u8 i, j;
    for (i = 0; i < NUM_FAMECHECKER_PERSONS; i++)
    {
        gSaveBlock1Ptr->fameChecker[i].pickState = FCPICKSTATE_COLORED;
        for (j = 0; j < 6; j++)
        {
            gSaveBlock1Ptr->fameChecker[i].flavorTextFlags |= (1 << j);
        }
    }
}

void FullyUnlockFameChecker(void)
{
    UnlockAllFameCheckerPeople();
}

static void FCSetup_ClearVideoRegisters(void)
{
    void *vram = (void *)VRAM;
    DmaClearLarge16(3, vram, VRAM_SIZE, 0x1000);
    DmaClear32(3, OAM, OAM_SIZE);
    DmaClear16(3, PLTT, PLTT_SIZE);
    SetGpuReg(REG_OFFSET_DISPCNT,  0);
    SetGpuReg(REG_OFFSET_BG0CNT,   0);
    SetGpuReg(REG_OFFSET_BG0HOFS,  0);
    SetGpuReg(REG_OFFSET_BG0VOFS,  0);
    SetGpuReg(REG_OFFSET_BG1CNT,   0);
    SetGpuReg(REG_OFFSET_BG1HOFS,  0);
    SetGpuReg(REG_OFFSET_BG1VOFS,  0);
    SetGpuReg(REG_OFFSET_BG2CNT,   0);
    SetGpuReg(REG_OFFSET_BG2HOFS,  0);
    SetGpuReg(REG_OFFSET_BG2VOFS,  0);
    SetGpuReg(REG_OFFSET_BG3CNT,   0);
    SetGpuReg(REG_OFFSET_BG3HOFS,  0);
    SetGpuReg(REG_OFFSET_BG3VOFS,  0);
    SetGpuReg(REG_OFFSET_WIN0H,    0);
    SetGpuReg(REG_OFFSET_WIN0V,    0);
    SetGpuReg(REG_OFFSET_WININ,    0);
    SetGpuReg(REG_OFFSET_WINOUT,   0);
    SetGpuReg(REG_OFFSET_BLDCNT,   0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY,     0);
}

static void FCSetup_ResetTasksAndSpriteResources(void)
{
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetAllPicSprites();
    ResetPaletteFade();
    InitObjectEventPalettes(0);
    gReservedSpritePaletteCount = 7;

}

static void FCSetup_TurnOnDisplay(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
}

static void FCSetup_ResetBGCoords(void)
{
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);
}

void SetFlavorTextFlagFromSpecialVars(void)
{
    NormalizeFameCheckerSaveData();
    if (gSpecialVar_0x8004 < NUM_FAMECHECKER_PERSONS && gSpecialVar_0x8005 < 6)
    {
        gSaveBlock1Ptr->fameChecker[gSpecialVar_0x8004].flavorTextFlags |= (1 << gSpecialVar_0x8005);
        gSpecialVar_0x8005 = FCPICKSTATE_SILHOUETTE;
        UpdatePickStateFromSpecialVar8005();
    }
}

void UpdatePickStateFromSpecialVar8005(void)
{
    NormalizeFameCheckerSaveData();
    if (gSpecialVar_0x8004 < NUM_FAMECHECKER_PERSONS && gSpecialVar_0x8005 < 3)
    {
        if (gSpecialVar_0x8005 == FCPICKSTATE_NO_DRAW)
            return;
        if (   gSpecialVar_0x8005 == FCPICKSTATE_SILHOUETTE
            && gSaveBlock1Ptr->fameChecker[gSpecialVar_0x8004].pickState == FCPICKSTATE_COLORED
           )
            return;
        gSaveBlock1Ptr->fameChecker[gSpecialVar_0x8004].pickState = gSpecialVar_0x8005;
    }
}

static bool8 HasUnlockedAllFlavorTextsForCurrentPerson(void)
{
    u8 i;
    u8 who = sFameCheckerData->unlockedPersons[FameCheckerGetCursorY()];
    for (i = 0; i < 6; i++)
    {
        if (!((gSaveBlock1Ptr->fameChecker[who].flavorTextFlags >> i) & 1))
            return FALSE;
    }
    return TRUE;
}

static void FreeSelectionCursorSpriteResources(void)
{
    FreeSpriteTilesByTag(SPRITETAG_SELECTOR_CURSOR);
    FreeSpritePaletteByTag(SPRITETAG_SELECTOR_CURSOR);
}

static u8 CreateFlavorTextIconSelectorCursorSprite(s16 where)
{
    s16 y =  34 + 27 * (where >= 3);
    s16 x = 114 + 47 * (where %  3);
    return CreateSprite(&sSpriteTemplate_SelectorCursor, x, y, 0);
}

static void SpriteCB_DestroyFlavorTextIconSelectorCursor(struct Sprite *sprite)
{
    DestroySprite(sprite);
}

static void FreeQuestionMarkSpriteResources(void)
{
    FreeSpriteTilesByTag(SPRITETAG_QUESTION_MARK);
}

static u8 PlaceQuestionMarkTile(u8 x, u8 y)
{
    u8 spriteId = CreateSprite(&sQuestionMarkTileSpriteTemplate, x, y, 8);
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].oam.priority = 2;
        gSprites[spriteId].oam.paletteNum = 2;
    }
    return spriteId;
}

static void FreeSpinningPokeballSpriteResources(void)
{
    FreeSpriteTilesByTag(SPRITETAG_SPINNING_POKEBALL);
    FreeSpritePaletteByTag(SPRITETAG_SPINNING_POKEBALL);
}

static u8 CreateSpinningPokeballSprite(void)
{
    return CreateSprite(&sSpinningPokeballSpriteTemplate, 0xe2, 0x42, 0);
}

static void SpriteCB_DestroySpinningPokeball(struct Sprite *sprite)
{
    FreeSpriteOamMatrix(sprite);
    DestroySprite(sprite);
}

static void FreeNonTrainerPicTiles(void)
{
    FreeSpriteTilesByTag(SPRITETAG_DAISY);
    FreeSpriteTilesByTag(SPRITETAG_FUJI);
    FreeSpriteTilesByTag(SPRITETAG_OAK);
    FreeSpriteTilesByTag(SPRITETAG_BILL);
}

static void SpriteCB_FCSpinningPokeball(struct Sprite *sprite)
{
    if (sprite->data[0] == 1)
    {
        if (sprite->x2 - 10 < 0)
        {
            sprite->x2 = 0;
            sprite->data[0] = 0;
        }
        else
            sprite->x2 -= 10;
    }
    else if (sprite->data[0] == 2)
    {
        if (sprite->x2 > 240)
        {
            sprite->x2 = 240;
            sprite->data[0] = 0;
        }
        else
            sprite->x2 += 10;
    }
}

#define PERSON_PAL_NUM 6
#define PERSON_X  148
#define PERSON_Y   66

static u8 CreatePersonPicSprite(u8 fcPersonIdx)
{
    u8 spriteId;
    u16 trainerPic = sFameCheckerTrainerPicIdxs[fcPersonIdx];

    if (fcPersonIdx == FAMECHECKER_BIRCH)
    {
        return CreateInvisibleSprite(SpriteCB_FCSpinningPokeball);
    }

    if (fcPersonIdx == FAMECHECKER_RIVAL)
        trainerPic = (gSaveBlock2Ptr->playerGender == MALE) ? TRAINER_PIC_MAY : TRAINER_PIC_BRENDAN;

    spriteId = CreateTrainerPicSprite(trainerPic, TRUE, PERSON_X, PERSON_Y, PERSON_PAL_NUM, TAG_NONE);
    gSprites[spriteId].callback = SpriteCB_FCSpinningPokeball;
    if (gSaveBlock1Ptr->fameChecker[fcPersonIdx].pickState == FCPICKSTATE_SILHOUETTE)
        LoadPalette(sSilhouettePalette, OBJ_PLTT_ID(PERSON_PAL_NUM), sizeof(sSilhouettePalette));
    return spriteId;
}

static void DestroyPersonPicSprite(u8 taskId, u16 who)
{
    s16 * data = gTasks[taskId].data;
    u16 who_copy = who;
    u8 fcPersonIdx;
    if (who_copy >= sFameCheckerData->numUnlockedPersons
        || sFameCheckerData->unlockedPersons[who_copy] >= NUM_FAMECHECKER_PERSONS)
        return;

    fcPersonIdx = sFameCheckerData->unlockedPersons[who_copy];
    if (fcPersonIdx == FAMECHECKER_BIRCH)
        DestroySprite(&gSprites[data[2]]);
    else
        FreeAndDestroyTrainerPicSprite(data[2]);
}

static void UpdateIconDescriptionBox(u8 whichText)
{
    s32 width;
    HandleFlavorTextModeSwitch(TRUE);
    gIconDescriptionBoxIsOpen = 1;
    FillWindowPixelRect(FCWINDOWID_ICONDESC, PIXEL_FILL(0), 0, 0, 0x58, 0x20);
    width = (0x54 - GetStringWidth(FONT_SMALL, sFameCheckerFlavorOriginLocationTexts[whichText], 0)) / 2;
    AddTextPrinterParameterized4(FCWINDOWID_ICONDESC, FONT_SMALL, width, 0, 0, 2, sTextColor_DkGrey, -1, sFameCheckerFlavorOriginLocationTexts[whichText]);
    StringExpandPlaceholders(gStringVar1, sFameCheckerFlavorOriginObjectNameTexts[whichText]);
    width = (0x54 - GetStringWidth(FONT_SMALL, gStringVar1, 0)) / 2;
    AddTextPrinterParameterized4(FCWINDOWID_ICONDESC, FONT_SMALL, width, 10, 0, 2, sTextColor_DkGrey, -1, gStringVar1);
    FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_ICONDESC);
}

static void UpdateIconDescriptionBoxOff(void)
{
    HandleFlavorTextModeSwitch(FALSE);
    gIconDescriptionBoxIsOpen = 0xFF;
}

static void FC_CreateListMenu(void)
{
    InitListMenuTemplate();
    sFameCheckerData->numUnlockedPersons = FC_PopulateListMenu();
    sFameCheckerData->listMenuTaskId = ListMenuInit(&gFameChecker_ListMenuTemplate, 0, 0);
    FC_PutWindowTilemapAndCopyWindowToVramMode3_2(FCWINDOWID_LIST);
}

static void InitListMenuTemplate(void)
{
    gFameChecker_ListMenuTemplate.items = sListMenuItems;
    gFameChecker_ListMenuTemplate.moveCursorFunc = FC_MoveCursorFunc;
    gFameChecker_ListMenuTemplate.itemPrintFunc = NULL;
    gFameChecker_ListMenuTemplate.totalItems = 1;
    gFameChecker_ListMenuTemplate.maxShowed = 1;
    gFameChecker_ListMenuTemplate.windowId = FCWINDOWID_LIST;
    gFameChecker_ListMenuTemplate.header_X = 0;
    gFameChecker_ListMenuTemplate.item_X = 8;
    gFameChecker_ListMenuTemplate.cursor_X = 0;
    gFameChecker_ListMenuTemplate.upText_Y = 4;
    gFameChecker_ListMenuTemplate.cursorPal = 2;
    gFameChecker_ListMenuTemplate.fillValue = 0;
    gFameChecker_ListMenuTemplate.cursorShadowPal = 3;
    gFameChecker_ListMenuTemplate.lettersSpacing = 0;
    gFameChecker_ListMenuTemplate.itemVerticalPadding = 0;
    gFameChecker_ListMenuTemplate.scrollMultiple = 0;
    gFameChecker_ListMenuTemplate.fontId = FONT_NORMAL;
    gFameChecker_ListMenuTemplate.cursorKind = 0;
}

static void FC_ClearListMenuRow(u8 row)
{
    FillWindowPixelRect(FCWINDOWID_LIST, PIXEL_FILL(0), 0, FC_LIST_ROW_HEIGHT * row, 8 * 8, FC_LIST_ROW_HEIGHT);
}

static void FC_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    u16 listMenuTopIdx;
    u8 taskId;
    u16 personIdx;
    sLastMenuIdx = 0;
    personIdx = sFameCheckerData->listMenuTopIdx2 + sFameCheckerData->listMenuDrawnSelIdx;
    FC_DoMoveCursor(itemIndex, onInit);
    taskId = FindTaskIdByFunc(Task_TopMenuHandleInput);
    if (taskId != 0xFF)
    {
        struct Task *task = &gTasks[taskId];
        PlaySE(SE_SELECT);
        task->data[1] = 0;
        ListMenuGetScrollAndRow(sFameCheckerData->listMenuTaskId, &listMenuTopIdx, NULL);
        sFameCheckerData->listMenuTopIdx = listMenuTopIdx;
        if (sFameCheckerData->unlockedPersons[itemIndex] < NUM_FAMECHECKER_PERSONS)
        {
            DestroyAllFlavorTextIcons();
            CreateAllFlavorTextIcons(itemIndex);
            if (sFameCheckerData->inPickMode)
            {
                if (!sFameCheckerData->pickModeOverCancel)
                {
                    DestroyPersonPicSprite(taskId, personIdx);
                    sLastMenuIdx = itemIndex;
                    task->func = Task_SwitchToPickMode;
                }
                else
                {
                    gSprites[task->data[2]].invisible = FALSE;
                    sFameCheckerData->pickModeOverCancel = FALSE;
                    gSprites[task->data[2]].data[0] = 0;
                    GetPickModeText();
                }
            }
            else
            {
                FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
                FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
            }
        }
        else
        {
            if (sFameCheckerData->unlockedPersons[itemIndex] == FAMECHECKER_HELP_ENTRY)
                PrintHelpDescription();
            else
                PrintCancelDescription();
            if (sFameCheckerData->inPickMode)
            {
                gSprites[task->data[2]].invisible = TRUE;
                sFameCheckerData->pickModeOverCancel = TRUE;
            }
            else
            {
                u8 i;
                for (i = 0; i < 6; i++)
                {
                    if (sFameCheckerData->spriteIds[i] < MAX_SPRITES)
                        gSprites[sFameCheckerData->spriteIds[i]].invisible = TRUE;
                }
            }
        }
    }
}

static void Task_SwitchToPickMode(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    task->data[2] = CreatePersonPicSprite(sFameCheckerData->unlockedPersons[sLastMenuIdx]);
    gSprites[task->data[2]].data[0] = 0;
    GetPickModeText();
    task->func = Task_TopMenuHandleInput;
}

static void PrintCancelDescription(void)
{
    FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
    AddTextPrinterParameterized2(FCWINDOWID_MSGBOX, FONT_NORMAL, gFameCheckerText_FameCheckerWillBeClosed, 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
}

static void PrintHelpDescription(void)
{
    FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
    AddTextPrinterParameterized2(FCWINDOWID_MSGBOX, FONT_NORMAL, sText_FameCheckerHelpDesc, 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    FC_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
}

static void FC_DoMoveCursor(s32 itemIndex, bool8 onInit)
{
    u16 listY;
    u16 cursorY;
    u16 who;
    ListMenuGetScrollAndRow(sFameCheckerData->listMenuTaskId, &listY, &cursorY);
    who = listY + cursorY;
    if (!onInit)
    {
        if (listY < sFameCheckerData->listMenuTopIdx2)
            sFameCheckerData->listMenuDrawnSelIdx++;
        else if (listY > sFameCheckerData->listMenuTopIdx2 && who != sFameCheckerData->numUnlockedPersons - 1)
            sFameCheckerData->listMenuDrawnSelIdx--;
        FC_ClearListMenuRow(sFameCheckerData->listMenuDrawnSelIdx);
        AddTextPrinterParameterized4(FCWINDOWID_LIST, FONT_NORMAL, 8, FC_LIST_ROW_TEXT_Y(sFameCheckerData->listMenuDrawnSelIdx), 0, 0, sTextColor_DkGrey, 0, sListMenuItems[sFameCheckerData->listMenuCurIdx].name);

    }
    FC_ClearListMenuRow(cursorY);
    AddTextPrinterParameterized4(FCWINDOWID_LIST, FONT_NORMAL, 8, FC_LIST_ROW_TEXT_Y(cursorY), 0, 0, sTextColor_Green, 0, sListMenuItems[itemIndex].name);
    PlaceListMenuCursor(TRUE);
    CopyWindowToVram(FCWINDOWID_LIST, COPYWIN_GFX);
    sFameCheckerData->listMenuCurIdx = itemIndex;
    sFameCheckerData->listMenuDrawnSelIdx = cursorY;
    sFameCheckerData->listMenuTopIdx2 = listY;
}

static u8 FC_PopulateListMenu(void)
{
    u8 nitems = 0;
    u8 i;

    for (i = 0; i < NUM_FAMECHECKER_PERSONS; i++)
    {
        u8 fameCheckerIdx = AdjustGiovanniIndexIfBeatenInGym(i);
        if (gSaveBlock1Ptr->fameChecker[fameCheckerIdx].pickState != FCPICKSTATE_NO_DRAW)
        {
            sListMenuItems[nitems].name = sFameCheckerPersonNamePointers[fameCheckerIdx];
            sListMenuItems[nitems].id = nitems;
            sFameCheckerData->unlockedPersons[nitems] = fameCheckerIdx;
            nitems++;
        }
    }
    sListMenuItems[nitems].name = sText_FameCheckerHelp;
    sListMenuItems[nitems].id = nitems;
    sFameCheckerData->unlockedPersons[nitems] = FAMECHECKER_HELP_ENTRY;
    nitems++;
    sListMenuItems[nitems].name = gFameCheckerText_Cancel;
    sListMenuItems[nitems].id = nitems;
    sFameCheckerData->unlockedPersons[nitems] = FAMECHECKER_CANCEL_ENTRY;
    nitems++;
    gFameChecker_ListMenuTemplate.totalItems = nitems;
    if (nitems < 5)
        gFameChecker_ListMenuTemplate.maxShowed = nitems;
    else
        gFameChecker_ListMenuTemplate.maxShowed = 5;
    return nitems;
}

static void FC_PutWindowTilemapAndCopyWindowToVramMode3_2(u8 windowId)
{
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_FULL);
}

static void FC_CreateScrollIndicatorArrowPair(void)
{
    struct ScrollArrowsTemplate template = {
          2,
          40,
          26,
          3,
          40,
          100,
          0,
          0,
          SPRITETAG_SCROLL_INDICATORS,
          0xFFFF,
          1,
    };

    if (sFameCheckerData->numUnlockedPersons > 5)
    {
        template.fullyUpThreshold = 0;
        template.fullyDownThreshold = sFameCheckerData->numUnlockedPersons - 5;
        sFameCheckerData->scrollIndicatorPairTaskId = AddScrollIndicatorArrowPair(&template, &sFameCheckerData->listMenuTopIdx);
    }
}

static void FreeListMenuSelectorArrowPairResources(void)
{
    if (sFameCheckerData->numUnlockedPersons > 5)
        RemoveScrollIndicatorArrowPair(sFameCheckerData->scrollIndicatorPairTaskId);
}

static u16 FameCheckerGetCursorY(void)
{
    u16 listY, cursorY;
    ListMenuGetScrollAndRow(sFameCheckerData->listMenuTaskId, &listY, &cursorY);
    return listY + cursorY;
}

static void HandleFlavorTextModeSwitch(bool8 state)
{
    if (sFameCheckerData->viewingFlavorText != state)
    {
        u8 taskId = FindTaskIdByFunc(Task_FCOpenOrCloseInfoBox);
        if (taskId == 0xFF)
            taskId = CreateTask(Task_FCOpenOrCloseInfoBox, 8);
        gTasks[taskId].data[0] = 0;
        gTasks[taskId].data[1] = 4;
        if (state == TRUE)
        {
            gTasks[taskId].data[2] = 1;
            sFameCheckerData->viewingFlavorText = TRUE;
        }
        else
        {
            gTasks[taskId].data[2] = 4;
            sFameCheckerData->viewingFlavorText = FALSE;
        }
    }
}

static void Task_FCOpenOrCloseInfoBox(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    switch (task->data[0])
    {
        case 0:
            if (--task->data[1] == 0)
            {
                UpdateInfoBoxTilemap(1, 0);
                task->data[1] = 4;
                task->data[0]++;
            }
            break;
        case 1:
            if (--task->data[1] == 0)
            {
                UpdateInfoBoxTilemap(1, task->data[2]);
                DestroyTask(taskId);
            }
            break;
    }
}

static void UpdateInfoBoxTilemap(u8 bg, s16 state)
{
    if (state == 0 || state == 3)
    {
        FillBgTilemapBufferRect(bg, 0x8C, 14, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0xA1, 15, 10, 10,  1, 1);
        FillBgTilemapBufferRect(bg, 0x8D, 25, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x8E, 26, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x8F, 14, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x00, 15, 11, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x90, 26, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x91, 14, 12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0xA3, 15, 12, 10,  1, 1);
        FillBgTilemapBufferRect(bg, 0x92, 25, 12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x93, 26, 12,  1,  1, 1);
    }
    else if (state == 1)
    {
        FillBgTilemapBufferRect(bg, 0x9B, 14, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x9C, 15, 10, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x96, 26, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x9D, 14, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x00, 15, 11, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x90, 26, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x9E, 14, 12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x9F, 15, 12, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x99, 26, 12,  1,  1, 1);
    }
    else if (state == 2)
    {
        FillBgTilemapBufferRect(bg, 0x94, 14, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x95, 15, 10, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x96, 26, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x8F, 14, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x9A, 15, 11, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x90, 26, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x97, 14, 12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x98, 15, 12, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x99, 26, 12,  1,  1, 1);
    }
    else if (state == 4)
    {
        FillBgTilemapBufferRect(bg, 0x83, 14, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0xA0, 15, 10, 10,  1, 1);
        FillBgTilemapBufferRect(bg, 0x84, 25, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x85, 26, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x86, 14, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0xA2, 15, 11, 10,  1, 1);
        FillBgTilemapBufferRect(bg, 0x87, 25, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x88, 26, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x83, 14, 12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0xA0, 15, 12, 10,  1, 1);
        FillBgTilemapBufferRect(bg, 0x84, 25, 12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x85, 26, 12,  1,  1, 1);
    }
    else if (state == 5)
    {
        FillBgTilemapBufferRect(bg, 0x00, 14, 10, 13,  3, 1);
    }
    CopyBgTilemapBufferToVram(bg);
}

static void PlaceListMenuCursor(bool8 isActive)
{
    u16 cursorY = ListMenuGetYCoordForPrintingArrowCursor(sFameCheckerData->listMenuTaskId);
    if (isActive == TRUE)
        AddTextPrinterParameterized4(FCWINDOWID_LIST, FONT_NORMAL, 0, cursorY, 0, 0, sTextColor_DkGrey, 0, gText_SelectorArrow2);
    else
        AddTextPrinterParameterized4(FCWINDOWID_LIST, FONT_NORMAL, 0, cursorY, 0, 0, sTextColor_White, 0, gText_SelectorArrow2);
}
