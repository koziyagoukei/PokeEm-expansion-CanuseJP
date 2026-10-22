#include "global.h"
#include "contest_shop.h"
#include "event_data.h"
#include "fieldmap.h"
#include "item.h"
#include "overworld.h"
#include "pokeblock.h"
#include "string_util.h"
#include "test/test.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/map_event_ids.h"
#include "constants/maps.h"

extern const u8 LilycoveCity_ContestLobby_EventScript_BpShop[];

static const enum Item sScarves[] =
{
    ITEM_RED_SCARF, ITEM_BLUE_SCARF, ITEM_PINK_SCARF, ITEM_GREEN_SCARF, ITEM_YELLOW_SCARF,
};

static void SetUpShop(u32 product)
{
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_LILYCOVE_CITY_CONTEST_LOBBY), MAP_NUM(MAP_LILYCOVE_CITY_CONTEST_LOBBY));
    FlagClear(FLAG_STORING_ITEMS_IN_PYRAMID_BAG);
    SetBagItemsPointers();
    ClearBag();
    ClearPokeblocks();
    gSaveBlock2Ptr->frontier.battlePoints = 100;
    gSpecialVar_0x8004 = product;
}

TEST("Contest shop: all five scarves cost ten BP and use the existing item names")
{
    u32 scarf = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sScarves); i++)
        PARAMETRIZE { scarf = i; }
    SetUpShop(scarf + CONTEST_SHOP_RED_SCARF);
    EXPECT(ContestShop_BufferProduct());
    EXPECT_EQ(StringCompare(gStringVar1, GetItemName(sScarves[scarf])), 0);
    EXPECT_EQ(ContestShop_Purchase(), CONTEST_SHOP_SUCCESS);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 90);
    EXPECT(CheckBagHasItem(sScarves[scarf], 1));
    EXPECT(!CheckBagHasItem(sScarves[scarf], 2));
    EXPECT_EQ(GetFirstFreePokeblockSlot(), 0);
}

TEST("Contest shop: Rainbow Pokeblock has five fifty-point flavors and twenty feel")
{
    SetUpShop(CONTEST_SHOP_RAINBOW_POKEBLOCK);
    EXPECT(AddBagItem(ITEM_POKEBLOCK_CASE, 1));
    EXPECT(ContestShop_BufferProduct());
    EXPECT_EQ(StringCompare(gStringVar1, gPokeblockNames[PBLOCK_CLR_RAINBOW]), 0);
    EXPECT_EQ(ContestShop_Purchase(), CONTEST_SHOP_SUCCESS);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 90);
    const struct Pokeblock *block = &gSaveBlock1Ptr->pokeblocks[0];
    EXPECT_EQ(block->color, PBLOCK_CLR_RAINBOW);
    for (u32 flavor = PBLOCK_SPICY; flavor <= PBLOCK_SOUR; flavor++)
        EXPECT_EQ(GetPokeblockData(block, flavor), 50);
    EXPECT_EQ(GetPokeblocksFeel(block), 20);
    EXPECT_EQ(GetFirstFreePokeblockSlot(), 1);
    // Equal flavors cancel nature preference, so the existing feed logic grants exactly 50.
    for (u32 nature = 0; nature < NUM_NATURES; nature++)
        EXPECT_EQ(PokeblockGetGain(nature, block), 0);
    EXPECT(TryClearPokeblock(0));
    EXPECT_EQ(GetFirstFreePokeblockSlot(), 0);
}

TEST("Contest shop: insufficient BP never adds a product or deducts BP")
{
    u32 product = 0;
    for (u32 i = 0; i < CONTEST_SHOP_PRODUCT_COUNT; i++)
        PARAMETRIZE { product = i; }
    SetUpShop(product);
    EXPECT(AddBagItem(ITEM_POKEBLOCK_CASE, 1));
    gSaveBlock2Ptr->frontier.battlePoints = 9;
    EXPECT_EQ(ContestShop_Purchase(), CONTEST_SHOP_NOT_ENOUGH_BP);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 9);
    EXPECT_EQ(GetFirstFreePokeblockSlot(), 0);
    for (u32 i = 0; i < ARRAY_COUNT(sScarves); i++)
        EXPECT(!CheckBagHasItem(sScarves[i], 1));
}

TEST("Contest shop: Rainbow Pokeblock requires a case and never charges without one")
{
    SetUpShop(CONTEST_SHOP_RAINBOW_POKEBLOCK);
    EXPECT_EQ(ContestShop_Purchase(), CONTEST_SHOP_NO_CASE);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    EXPECT_EQ(GetFirstFreePokeblockSlot(), 0);
}

TEST("Contest shop: a full case preserves every Pokeblock and all BP")
{
    SetUpShop(CONTEST_SHOP_RAINBOW_POKEBLOCK);
    EXPECT(AddBagItem(ITEM_POKEBLOCK_CASE, 1));
    const struct Pokeblock block = {PBLOCK_CLR_RED, 20, 0, 0, 0, 0, 20};
    for (u32 i = 0; i < POKEBLOCKS_COUNT; i++)
        EXPECT(AddPokeblock(&block));
    EXPECT_EQ(ContestShop_Purchase(), CONTEST_SHOP_CASE_FULL);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    for (u32 i = 0; i < POKEBLOCKS_COUNT; i++)
        EXPECT_EQ(memcmp(&gSaveBlock1Ptr->pokeblocks[i], &block, sizeof(block)), 0);
}

TEST("Contest shop: a full item pocket does not consume BP")
{
    SetUpShop(CONTEST_SHOP_RED_SCARF);
    struct BagPocket *pocket = &gBagPockets[GetItemPocket(ITEM_RED_SCARF)];
    for (u32 i = 0; i < pocket->capacity; i++)
        BagPocket_SetSlotItemIdAndCount(pocket, i, ITEM_BLUE_SCARF, MAX_BAG_ITEM_CAPACITY);
    EXPECT_EQ(ContestShop_Purchase(), CONTEST_SHOP_BAG_FULL);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    EXPECT(!CheckBagHasItem(ITEM_RED_SCARF, 1));
    for (u32 i = 0; i < pocket->capacity; i++)
        EXPECT_EQ(BagPocket_GetSlotData(pocket, i).quantity, MAX_BAG_ITEM_CAPACITY);
}

TEST("Contest shop: exact BP balance permits one purchase, not a second")
{
    SetUpShop(CONTEST_SHOP_RED_SCARF);
    gSaveBlock2Ptr->frontier.battlePoints = CONTEST_SHOP_PRICE;
    EXPECT_EQ(ContestShop_Purchase(), CONTEST_SHOP_SUCCESS);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(ContestShop_Purchase(), CONTEST_SHOP_NOT_ENOUGH_BP);
    EXPECT(!CheckBagHasItem(ITEM_RED_SCARF, 2));
}

TEST("Contest shop: invalid and cancelled selections cannot buy anything")
{
    u32 product = CONTEST_SHOP_PRODUCT_COUNT;
    PARAMETRIZE { product = CONTEST_SHOP_PRODUCT_COUNT; }
    PARAMETRIZE { product = 127; }
    PARAMETRIZE { product = 0xFFFF; }
    SetUpShop(product);
    EXPECT(!ContestShop_BufferProduct());
    EXPECT_EQ(ContestShop_Purchase(), CONTEST_SHOP_INVALID);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    EXPECT_EQ(GetFirstFreePokeblockSlot(), 0);
}

TEST("Contest shop: lobby clerk has a reachable tile and a permanent script")
{
    SetUpShop(CONTEST_SHOP_RAINBOW_POKEBLOCK);
    EXPECT_GE(gMapHeader.events->objectEventCount, LOCALID_CONTEST_LOBBY_BP_SHOP);
    const struct ObjectEventTemplate *clerk = &gMapHeader.events->objectEvents[LOCALID_CONTEST_LOBBY_BP_SHOP - 1];
    EXPECT_EQ(clerk->localId, LOCALID_CONTEST_LOBBY_BP_SHOP);
    EXPECT_EQ(clerk->graphicsId, OBJ_EVENT_GFX_MART_EMPLOYEE);
    EXPECT(clerk->script == LilycoveCity_ContestLobby_EventScript_BpShop);
    EXPECT_EQ(clerk->flagId, 0);
    InitMap();
    EXPECT_EQ(MapGridGetCollisionAt(clerk->x + MAP_OFFSET, clerk->y + MAP_OFFSET), 0);
    EXPECT_EQ(MapGridGetCollisionAt(clerk->x + MAP_OFFSET, clerk->y + MAP_OFFSET + 1), 0);
}
