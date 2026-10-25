#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "item_menu.h"
#include "pokemon.h"
#include "string_util.h"
#include "test/overworld_script.h"
#include "test/test.h"

static void ExpectBagNameOrder(enum Pocket pocketId, u32 count)
{
    for (u32 i = 0; i < count; i++)
    {
        EXPECT(GetBagItemId(pocketId, i) != ITEM_NONE);
        EXPECT_EQ(GetBagItemQuantity(pocketId, i), 1);
        if (i != 0)
            EXPECT(StringCompare(GetItemName(GetBagItemId(pocketId, i - 1)),
                                 GetItemName(GetBagItemId(pocketId, i))) <= 0);
    }
    EXPECT_EQ(GetBagItemId(pocketId, count), ITEM_NONE);
}

TEST("Bag redesign: stones have their own pocket and sort by item ID")
{
    struct BagPocket *pocket = &gBagPockets[POCKET_STONES];

    RUN_OVERWORLD_SCRIPT(
        additem ITEM_ULTRANECROZIUM_Z;
        additem ITEM_GENGARITE;
        additem ITEM_GRASSIUM_Z;
        additem ITEM_VENUSAURITE;
    );

    SortItemsInBag(pocket, SORT_BY_INDEX);
    for (u32 i = 0; i < 4; i++)
    {
        EXPECT_EQ(GetItemPocket(GetBagItemId(POCKET_STONES, i)), POCKET_STONES);
        EXPECT_EQ(GetBagItemQuantity(POCKET_STONES, i), 1);
        if (i != 0)
            EXPECT(GetBagItemId(POCKET_STONES, i - 1) < GetBagItemId(POCKET_STONES, i));
    }
    EXPECT_EQ(GetBagItemId(POCKET_STONES, 4), ITEM_NONE);
}

TEST("Berries are sorted correctly in the bag")
{
    struct BagPocket *pocket = &gBagPockets[POCKET_BERRIES];

    ASSUME(GetItemPocket(ITEM_POMEG_BERRY) == POCKET_BERRIES);
    ASSUME(GetItemPocket(ITEM_MAGOST_BERRY) == POCKET_BERRIES);
    ASSUME(GetItemPocket(ITEM_KELPSY_BERRY) == POCKET_BERRIES);
    ASSUME(GetItemPocket(ITEM_MICLE_BERRY) == POCKET_BERRIES);
    ASSUME(GetItemPocket(ITEM_CHARTI_BERRY) == POCKET_BERRIES);
    ASSUME(GetItemPocket(ITEM_GANLON_BERRY) == POCKET_BERRIES);
    ASSUME(GetItemPocket(ITEM_ORAN_BERRY) == POCKET_BERRIES);
    ASSUME(GetItemPocket(ITEM_CHERI_BERRY) == POCKET_BERRIES);

    RUN_OVERWORLD_SCRIPT(
        additem ITEM_POMEG_BERRY;
        additem ITEM_MAGOST_BERRY;
        additem ITEM_KELPSY_BERRY;
        additem ITEM_MICLE_BERRY;
        additem ITEM_CHARTI_BERRY;
        additem ITEM_GANLON_BERRY;
        additem ITEM_ORAN_BERRY;
        additem ITEM_CHERI_BERRY;
    );

    SortItemsInBag(&gBagPockets[POCKET_BERRIES], SORT_BY_INDEX);

    EXPECT_EQ(pocket->itemSlots[0].itemId, ITEM_CHERI_BERRY);
    EXPECT_EQ(pocket->itemSlots[1].itemId, ITEM_ORAN_BERRY);
    EXPECT_EQ(pocket->itemSlots[2].itemId, ITEM_POMEG_BERRY);
    EXPECT_EQ(pocket->itemSlots[3].itemId, ITEM_KELPSY_BERRY);
    EXPECT_EQ(pocket->itemSlots[4].itemId, ITEM_MAGOST_BERRY);
    EXPECT_EQ(pocket->itemSlots[5].itemId, ITEM_CHARTI_BERRY);
    EXPECT_EQ(pocket->itemSlots[6].itemId, ITEM_GANLON_BERRY);
    EXPECT_EQ(pocket->itemSlots[7].itemId, ITEM_MICLE_BERRY);
    EXPECT_EQ(pocket->itemSlots[8].itemId, ITEM_NONE);

    SortItemsInBag(&gBagPockets[POCKET_BERRIES], SORT_ALPHABETICALLY);

    // Item names are localized, so an English-only expected order is invalid.
    ExpectBagNameOrder(POCKET_BERRIES, 8);
}

TEST("Items are correctly sorted and compacted in the bag")
{
    struct BagPocket *pocket = &gBagPockets[POCKET_ITEMS];
    memset(pocket->itemSlots, 0, sizeof(gSaveBlock1Ptr->bag.items));

    ASSUME(GetItemPocket(ITEM_NUGGET) == POCKET_ITEMS);
    ASSUME(GetItemPocket(ITEM_BIG_NUGGET) == POCKET_ITEMS);
    ASSUME(GetItemPocket(ITEM_TINY_MUSHROOM) == POCKET_ITEMS);
    ASSUME(GetItemPocket(ITEM_BIG_MUSHROOM) == POCKET_ITEMS);
    ASSUME(GetItemPocket(ITEM_PEARL) == POCKET_ITEMS);
    ASSUME(GetItemPocket(ITEM_BIG_PEARL) == POCKET_ITEMS);

    RUN_OVERWORLD_SCRIPT(
        additem ITEM_NUGGET;
        additem ITEM_BIG_NUGGET;
        additem ITEM_TINY_MUSHROOM;
        additem ITEM_BIG_MUSHROOM;
        additem ITEM_PEARL;
        additem ITEM_BIG_PEARL;
    );

    EXPECT_EQ(pocket->itemSlots[0].itemId, ITEM_NUGGET);
    EXPECT_EQ(pocket->itemSlots[0].quantity, 1);
    EXPECT_EQ(pocket->itemSlots[1].itemId, ITEM_BIG_NUGGET);
    EXPECT_EQ(pocket->itemSlots[1].quantity, 1);
    EXPECT_EQ(pocket->itemSlots[2].itemId, ITEM_TINY_MUSHROOM);
    EXPECT_EQ(pocket->itemSlots[2].quantity, 1);
    EXPECT_EQ(pocket->itemSlots[3].itemId, ITEM_BIG_MUSHROOM);
    EXPECT_EQ(pocket->itemSlots[3].quantity, 1);
    EXPECT_EQ(pocket->itemSlots[4].itemId, ITEM_PEARL);
    EXPECT_EQ(pocket->itemSlots[4].quantity, 1);
    EXPECT_EQ(pocket->itemSlots[5].itemId, ITEM_BIG_PEARL);
    EXPECT_EQ(pocket->itemSlots[5].quantity, 1);
    EXPECT_EQ(pocket->itemSlots[6].itemId, ITEM_NONE);

    SortItemsInBag(&gBagPockets[POCKET_ITEMS], SORT_ALPHABETICALLY);

    ExpectBagNameOrder(POCKET_ITEMS, 6);

    // Try removing the big items, check that everything is compacted correctly

    RUN_OVERWORLD_SCRIPT(
        removeitem ITEM_BIG_NUGGET;
        removeitem ITEM_BIG_MUSHROOM;
        removeitem ITEM_BIG_PEARL;
    );

    CompactItemsInBagPocket(POCKET_ITEMS);

    ExpectBagNameOrder(POCKET_ITEMS, 3);
    EXPECT(CheckBagHasItem(ITEM_NUGGET, 1));
    EXPECT(CheckBagHasItem(ITEM_PEARL, 1));
    EXPECT(CheckBagHasItem(ITEM_TINY_MUSHROOM, 1));
    EXPECT_EQ(pocket->itemSlots[3].itemId, ITEM_NONE);
    EXPECT_EQ(pocket->itemSlots[4].itemId, ITEM_NONE);
    EXPECT_EQ(pocket->itemSlots[5].itemId, ITEM_NONE);
    EXPECT_EQ(pocket->itemSlots[6].itemId, ITEM_NONE);
}
