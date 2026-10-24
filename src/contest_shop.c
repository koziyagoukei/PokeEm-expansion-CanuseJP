#include "global.h"
#include "contest_shop.h"
#include "event_data.h"
#include "item.h"
#include "pokeblock.h"
#include "string_util.h"

static const enum Item sScarfItems[] =
{
    ITEM_RED_SCARF, ITEM_BLUE_SCARF, ITEM_PINK_SCARF, ITEM_GREEN_SCARF, ITEM_YELLOW_SCARF,
};

static const struct Pokeblock sRainbowPokeblock =
{
    .color = PBLOCK_CLR_RAINBOW,
    .spicy = 50,
    .dry = 50,
    .sweet = 50,
    .bitter = 50,
    .sour = 50,
    .feel = 20,
};

STATIC_ASSERT(ARRAY_COUNT(sScarfItems) + 1 == CONTEST_SHOP_PRODUCT_COUNT, ContestShopProductCount);

bool32 ContestShop_BufferProduct(void)
{
    u32 product = gSpecialVar_0x8004;

    if (product >= CONTEST_SHOP_PRODUCT_COUNT)
        return FALSE;
    if (product == CONTEST_SHOP_RAINBOW_POKEBLOCK)
        StringCopy(gStringVar1, gPokeblockNames[PBLOCK_CLR_RAINBOW]);
    else
        StringCopy(gStringVar1, GetItemName(sScarfItems[product - 1]));
    return TRUE;
}

u16 ContestShop_Purchase(void)
{
    u32 product = gSpecialVar_0x8004;

    if (product >= CONTEST_SHOP_PRODUCT_COUNT)
        return CONTEST_SHOP_INVALID;
    if (gSaveBlock2Ptr->frontier.battlePoints < CONTEST_SHOP_PRICE)
        return CONTEST_SHOP_NOT_ENOUGH_BP;

    if (product == CONTEST_SHOP_RAINBOW_POKEBLOCK)
    {
        if (!CheckBagHasItem(ITEM_POKEBLOCK_CASE, 1))
            return CONTEST_SHOP_NO_CASE;
        if (!AddPokeblock(&sRainbowPokeblock))
            return CONTEST_SHOP_CASE_FULL;
    }
    else if (!AddBagItem(sScarfItems[product - 1], 1))
        return CONTEST_SHOP_BAG_FULL;

    // Charge only after the product has been stored successfully.
    gSaveBlock2Ptr->frontier.battlePoints -= CONTEST_SHOP_PRICE;
    return CONTEST_SHOP_SUCCESS;
}
