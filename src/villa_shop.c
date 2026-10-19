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
#include "menu_helpers.h"
#include "pokeball.h"
#include "pokemon.h"
#include "script.h"
#include "sound.h"
#include "special_doll.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "villa_shop.h"
#include "constants/decorations.h"
#include "constants/songs.h"

static const u8 sText_Cancel[] = _("{JPN}やめる");

static const u8 *const sCategoryNames[VILLA_SHOP_CATEGORY_COUNT] =
{
    gText_Desk, gText_Chair, gText_Plant, gText_Ornament,
    gText_Mat, gText_Poster, gText_Doll, gText_Cushion, gText_SpecialDolls,
};

STATIC_ASSERT(DECORCAT_COUNT == VILLA_SHOP_SPECIAL_DOLL_CATEGORY, VillaShopDecorationCategories);

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
    u16 category;
    u16 scroll;
    u16 row;
    u8 windowId;
    u8 listTaskId;
    u8 arrowsTaskId;
};

static EWRAM_DATA struct VillaShopCatalog *sCatalog = NULL;
static EWRAM_DATA u16 sLastScroll = 0;
static EWRAM_DATA u16 sLastRow = 0;
static EWRAM_DATA u16 sLastCategory = 0;

static void DrawCatalog(void);

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

u16 VillaShop_BuySpecialDoll(u16 doll)
{
    if (SpecialDoll_GetDecoration(doll) == NULL)
        return VILLA_SHOP_INVALID;
    if (gSaveBlock2Ptr->frontier.battlePoints < VILLA_SHOP_DECOR_PRICE)
        return VILLA_SHOP_NOT_ENOUGH_BP;
    if (!SpecialDoll_Add(doll))
        return VILLA_SHOP_STORAGE_FULL;
    gSaveBlock2Ptr->frontier.battlePoints -= VILLA_SHOP_DECOR_PRICE;
    return VILLA_SHOP_OK;
}

bool8 VillaShop_IsInCategory(u16 category, u16 item)
{
    if (category == VILLA_SHOP_SPECIAL_DOLL_CATEGORY)
        return SpecialDoll_GetDecoration(item) != NULL;
    return category < DECORCAT_COUNT && item != DECOR_NONE && item <= NUM_DECORATIONS
        && gDecorations[item].category == category;
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
    else if (gSpecialVar_0x8006 == VILLA_SHOP_SPECIAL_DOLLS)
        gSpecialVar_Result = VillaShop_BuySpecialDoll(gSpecialVar_0x8005);
    else
        gSpecialVar_Result = VILLA_SHOP_INVALID;
}

static void PrintCatalogPrice(u8 windowId, u32 id, u8 y)
{
    u8 text[16];
    u16 price;
    if (id == 0)
        return;
    price = sCatalog->mode == VILLA_SHOP_BALLS ? VillaShop_GetBallPrice(id) : VILLA_SHOP_DECOR_PRICE;
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
    if (sCatalog->mode != VILLA_SHOP_BALLS && sCatalog->category == VILLA_SHOP_CATEGORY_NONE && id > 0)
    {
        DestroyListMenuTask(sCatalog->listTaskId, NULL, NULL);
        RemoveScrollIndicatorArrowPair(sCatalog->arrowsTaskId);
        sCatalog->category = id - 1;
        sCatalog->mode = sCatalog->category == VILLA_SHOP_SPECIAL_DOLL_CATEGORY
            ? VILLA_SHOP_SPECIAL_DOLLS : VILLA_SHOP_DECORATIONS;
        sCatalog->scroll = 0;
        sCatalog->row = 0;
        DrawCatalog();
        return;
    }
    if (sCatalog->mode != VILLA_SHOP_BALLS && sCatalog->category != VILLA_SHOP_CATEGORY_NONE && id <= 0)
    {
        u16 category = sCatalog->category;
        DestroyListMenuTask(sCatalog->listTaskId, NULL, NULL);
        RemoveScrollIndicatorArrowPair(sCatalog->arrowsTaskId);
        sCatalog->category = VILLA_SHOP_CATEGORY_NONE;
        sCatalog->mode = VILLA_SHOP_DECORATIONS;
        sCatalog->row = min(category, 5);
        sCatalog->scroll = category - sCatalog->row;
        DrawCatalog();
        return;
    }
    gSpecialVar_Result = id > 0;
    if (id > 0)
    {
        gSpecialVar_0x8005 = id;
        gSpecialVar_0x8006 = sCatalog->mode;
        if (sCatalog->mode == VILLA_SHOP_SPECIAL_DOLLS)
            StringCopy(gStringVar1, SpecialDoll_GetDecoration(id)->name);
        else
            StringCopy(gStringVar1, sCatalog->mode == VILLA_SHOP_DECORATIONS
                ? gDecorations[id].name : GetItemName(gPokeBalls[id].itemId));
        ConvertIntToDecimalStringN(gStringVar2, sCatalog->mode == VILLA_SHOP_BALLS
            ? VillaShop_GetBallPrice(id) : VILLA_SHOP_DECOR_PRICE, STR_CONV_MODE_LEFT_ALIGN, 3);
    }
    sLastCategory = sCatalog->category == VILLA_SHOP_CATEGORY_NONE ? 0 : sCatalog->category + 1;
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

static void DrawCatalog(void)
{
    struct ListMenuTemplate list = {0};
    u32 id, count = 0;
    bool8 categories = sCatalog->mode != VILLA_SHOP_BALLS && sCatalog->category == VILLA_SHOP_CATEGORY_NONE;
    if (categories)
    {
        for (id = 0; id < VILLA_SHOP_CATEGORY_COUNT; id++)
            sCatalog->items[count++] = (struct ListMenuItem){sCategoryNames[id], id + 1};
    }
    else if (sCatalog->mode == VILLA_SHOP_DECORATIONS)
    {
        for (id = 1; id <= NUM_DECORATIONS; id++)
            if (VillaShop_IsInCategory(sCatalog->category, id))
                sCatalog->items[count++] = (struct ListMenuItem){gDecorations[id].name, id};
    }
    else if (sCatalog->mode == VILLA_SHOP_SPECIAL_DOLLS)
    {
        for (id = 1; id < SPECIAL_DOLL_COUNT; id++)
            sCatalog->items[count++] = (struct ListMenuItem){SpecialDoll_GetDecoration(id)->name, id};
    }
    else
    {
        for (id = 1; id < POKEBALL_COUNT; id++)
            if (VillaShop_GetBallPrice(id) != 0)
                sCatalog->items[count++] = (struct ListMenuItem){GetItemName(gPokeBalls[id].itemId), id};
    }
    sCatalog->items[count++] = (struct ListMenuItem){sText_Cancel, 0};
    FillWindowPixelBuffer(sCatalog->windowId, PIXEL_FILL(1));
    SetStandardWindowBorderStyle(sCatalog->windowId, FALSE);
    list.items = sCatalog->items;
    list.moveCursorFunc = CatalogMoveCursor;
    list.itemPrintFunc = categories ? NULL : PrintCatalogPrice;
    list.totalItems = count;
    list.maxShowed = min(6, count);
    list.windowId = sCatalog->windowId;
    list.item_X = 8;
    list.upText_Y = 1;
    list.cursorPal = 2;
    list.fillValue = 1;
    list.cursorShadowPal = 3;
    list.fontId = FONT_NORMAL;
    list.scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD;
    SetCursorWithinListBounds(&sCatalog->scroll, &sCatalog->row, list.maxShowed, count);
    sCatalog->listTaskId = ListMenuInit(&list, sCatalog->scroll, sCatalog->row);
    sCatalog->arrowsTaskId = AddScrollIndicatorArrowPairParameterized(
        SCROLL_ARROW_UP, 228, 32, 132, count - list.maxShowed, 0xFCE0, 0xFCE0, &sCatalog->scroll);
    CopyWindowToVram(sCatalog->windowId, COPYWIN_FULL);
}

void VillaShop_OpenCatalog(void)
{
    const struct WindowTemplate window = {0, 2, 4, 26, 12, 15, 64};
    u8 taskId = CreateTask(Task_VillaShopResume, 8);

    gSpecialVar_Result = FALSE;
    if (sCatalog != NULL || gSpecialVar_0x8006 > VILLA_SHOP_SPECIAL_DOLLS)
        return;
    sCatalog = AllocZeroed(sizeof(*sCatalog));
    if (sCatalog == NULL)
        return;
    if (gSpecialVar_0x8008)
    {
        sLastScroll = 0;
        sLastRow = 0;
        sLastCategory = 0;
        gSpecialVar_0x8008 = FALSE;
    }
    sCatalog->category = sLastCategory == 0 ? VILLA_SHOP_CATEGORY_NONE : sLastCategory - 1;
    sCatalog->mode = gSpecialVar_0x8006 == VILLA_SHOP_BALLS ? VILLA_SHOP_BALLS
        : sCatalog->category == VILLA_SHOP_SPECIAL_DOLL_CATEGORY ? VILLA_SHOP_SPECIAL_DOLLS : VILLA_SHOP_DECORATIONS;
    sCatalog->scroll = sLastScroll;
    sCatalog->row = sLastRow;
    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    sCatalog->windowId = AddWindow(&window);
    if (sCatalog->windowId == WINDOW_NONE)
    {
        Free(sCatalog);
        sCatalog = NULL;
        return;
    }
    DrawCatalog();
    gTasks[taskId].func = Task_VillaShopCatalog;
}
