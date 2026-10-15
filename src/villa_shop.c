#include "global.h"
#include "decoration.h"
#include "decoration_inventory.h"
#include "event_data.h"
#include "item.h"
#include "international_string_util.h"
#include "list_menu.h"
#include "malloc.h"
#include "map_name_popup.h"
#include "menu.h"
#include "pokeball.h"
#include "pokemon.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "villa_shop.h"
#include "constants/decorations.h"
#include "constants/songs.h"

static const u8 sText_Cancel[] = _("{JPN}やめる");

static const u8 sBallPrices[POKEBALL_COUNT] =
{
    [BALL_POKE] = 1,
    [BALL_GREAT] = 2,
    [BALL_HEAL] = 2,
    [BALL_ULTRA] = 3,
    [BALL_PREMIER] = 3,
    [BALL_NEST] = 3,
    [BALL_TIMER] = 5,
    [BALL_QUICK] = 5,
    [BALL_REPEAT] = 5,
    [BALL_NET] = 5,
    [BALL_DIVE] = 5,
    [BALL_DUSK] = 5,
    [BALL_LUXURY] = 10,
    [BALL_LEVEL] = 20,
    [BALL_LURE] = 20,
    [BALL_MOON] = 20,
    [BALL_FRIEND] = 20,
    [BALL_LOVE] = 20,
    [BALL_FAST] = 20,
    [BALL_HEAVY] = 20,
    [BALL_DREAM] = 30,
    [BALL_BEAST] = 30,
    [BALL_SPORT] = 30,
    [BALL_SAFARI] = 30,
    [BALL_PARK] = 30,
    [BALL_MASTER] = 50,
    [BALL_CHERISH] = 100,
};

struct VillaShopCatalog
{
    struct ListMenuItem items[NUM_DECORATIONS + 1];
    u16 mode;
    u16 scroll;
    u16 row;
    u8 windowId;
    u8 listTaskId;
    u8 arrowsTaskId;
};

static EWRAM_DATA struct VillaShopCatalog *sCatalog = NULL;
static EWRAM_DATA u16 sLastScroll = 0;
static EWRAM_DATA u16 sLastRow = 0;

u16 VillaShop_GetBallPrice(u16 ball)
{
    if (ball >= ARRAY_COUNT(sBallPrices))
        return 0;
    return sBallPrices[ball];
}

u16 VillaShop_BuyDecoration(u16 decoration)
{
    if (decoration == DECOR_NONE || decoration > NUM_DECORATIONS)
        return VILLA_SHOP_INVALID;
    if (gSaveBlock2Ptr->frontier.battlePoints < VILLA_SHOP_DECOR_PRICE)
        return VILLA_SHOP_NOT_ENOUGH_BP;
    if (!DecorationAdd(decoration))
        return VILLA_SHOP_STORAGE_FULL;
    gSaveBlock2Ptr->frontier.battlePoints -= VILLA_SHOP_DECOR_PRICE;
    return VILLA_SHOP_OK;
}

u16 VillaShop_ChangeBall(u16 partySlot, u16 ball)
{
    u16 price = VillaShop_GetBallPrice(ball);
    struct Pokemon *mon;

    if (partySlot >= PARTY_SIZE || price == 0)
        return VILLA_SHOP_INVALID;
    mon = &gParties[B_TRAINER_PLAYER][partySlot];
    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
        return VILLA_SHOP_INVALID;
    if (GetMonData(mon, MON_DATA_POKEBALL) == ball)
        return VILLA_SHOP_SAME_BALL;
    if (gSaveBlock2Ptr->frontier.battlePoints < price)
        return VILLA_SHOP_NOT_ENOUGH_BP;
    SetMonData(mon, MON_DATA_POKEBALL, &ball);
    gSaveBlock2Ptr->frontier.battlePoints -= price;
    return VILLA_SHOP_OK;
}

// Script inputs: 8004 = party slot, 8005 = catalog id, 8006 = service.
void VillaShop_Purchase(void)
{
    if (gSpecialVar_0x8006 == VILLA_SHOP_DECORATIONS)
        gSpecialVar_Result = VillaShop_BuyDecoration(gSpecialVar_0x8005);
    else if (gSpecialVar_0x8006 == VILLA_SHOP_BALLS)
        gSpecialVar_Result = VillaShop_ChangeBall(gSpecialVar_0x8004, gSpecialVar_0x8005);
    else
        gSpecialVar_Result = VILLA_SHOP_INVALID;
}

static void PrintCatalogPrice(u8 windowId, u32 id, u8 y)
{
    u8 text[16];
    u16 price;
    if (id == 0)
        return;
    price = sCatalog->mode == VILLA_SHOP_DECORATIONS ? VILLA_SHOP_DECOR_PRICE : VillaShop_GetBallPrice(id);
    StringCopy(ConvertIntToDecimalStringN(text, price, STR_CONV_MODE_LEFT_ALIGN, 3), gText_BP);
    AddTextPrinterParameterized(windowId, FONT_NORMAL, text,
        GetStringRightAlignXOffset(FONT_NORMAL, text, 204), y, TEXT_SKIP_DRAW, NULL);
}

static void CatalogMoveCursor(s32 id, bool8 onInit, struct ListMenu *list)
{
    if (!onInit)
        PlaySE(SE_SELECT);
}

static void Task_VillaShopCatalog(u8 taskId)
{
    s32 id = ListMenu_ProcessInput(sCatalog->listTaskId);
    ListMenuGetScrollAndRow(sCatalog->listTaskId, &sCatalog->scroll, &sCatalog->row);
    if (id == LIST_NOTHING_CHOSEN)
        return;
    PlaySE(SE_SELECT);
    gSpecialVar_Result = id > 0;
    if (id > 0)
    {
        gSpecialVar_0x8005 = id;
        StringCopy(gStringVar1, sCatalog->mode == VILLA_SHOP_DECORATIONS
            ? gDecorations[id].name : GetItemName(gPokeBalls[id].itemId));
        ConvertIntToDecimalStringN(gStringVar2, sCatalog->mode == VILLA_SHOP_DECORATIONS
            ? VILLA_SHOP_DECOR_PRICE : VillaShop_GetBallPrice(id), STR_CONV_MODE_LEFT_ALIGN, 3);
    }
    DestroyListMenuTask(sCatalog->listTaskId, &sLastScroll, &sLastRow);
    RemoveScrollIndicatorArrowPair(sCatalog->arrowsTaskId);
    ClearStdWindowAndFrameToTransparent(sCatalog->windowId, TRUE);
    RemoveWindow(sCatalog->windowId);
    Free(sCatalog);
    sCatalog = NULL;
    DestroyTask(taskId);
    ScriptContext_Enable();
}

static void Task_VillaShopResume(u8 taskId)
{
    DestroyTask(taskId);
    ScriptContext_Enable();
}

void VillaShop_OpenCatalog(void)
{
    const struct WindowTemplate window = {0, 2, 4, 26, 12, 15, 64};
    struct ListMenuTemplate list = {0};
    u32 id, count = 0;
    u8 taskId = CreateTask(Task_VillaShopResume, 8);

    gSpecialVar_Result = FALSE;
    if (sCatalog != NULL || gSpecialVar_0x8006 > VILLA_SHOP_BALLS)
        return;
    sCatalog = AllocZeroed(sizeof(*sCatalog));
    if (sCatalog == NULL)
        return;
    sCatalog->mode = gSpecialVar_0x8006;
    if (sCatalog->mode == VILLA_SHOP_DECORATIONS)
    {
        for (id = 1; id <= NUM_DECORATIONS; id++)
            sCatalog->items[count++] = (struct ListMenuItem){gDecorations[id].name, id};
    }
    else
    {
        for (id = 1; id < POKEBALL_COUNT; id++)
            if (VillaShop_GetBallPrice(id) != 0)
                sCatalog->items[count++] = (struct ListMenuItem){GetItemName(gPokeBalls[id].itemId), id};
    }
    sCatalog->items[count++] = (struct ListMenuItem){sText_Cancel, 0};
    if (gSpecialVar_0x8008)
    {
        sLastScroll = 0;
        sLastRow = 0;
        gSpecialVar_0x8008 = FALSE;
    }
    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    sCatalog->windowId = AddWindow(&window);
    if (sCatalog->windowId == WINDOW_NONE)
    {
        Free(sCatalog);
        sCatalog = NULL;
        return;
    }
    SetStandardWindowBorderStyle(sCatalog->windowId, FALSE);
    list.items = sCatalog->items;
    list.moveCursorFunc = CatalogMoveCursor;
    list.itemPrintFunc = PrintCatalogPrice;
    list.totalItems = count;
    list.maxShowed = 6;
    list.windowId = sCatalog->windowId;
    list.item_X = 8;
    list.upText_Y = 1;
    list.cursorPal = 2;
    list.fillValue = 1;
    list.cursorShadowPal = 3;
    list.fontId = FONT_NORMAL;
    list.scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD;
    sCatalog->scroll = sLastScroll;
    sCatalog->row = sLastRow;
    sCatalog->listTaskId = ListMenuInit(&list, sLastScroll, sLastRow);
    sCatalog->arrowsTaskId = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP, 228, 32, 132, count - list.maxShowed, 0xFCE0, 0xFCE0, &sCatalog->scroll);
    CopyWindowToVram(sCatalog->windowId, COPYWIN_FULL);
    gTasks[taskId].func = Task_VillaShopCatalog;
}
