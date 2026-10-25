#include "global.h"
#include "berry.h"
#include "frontier_inventory.h"
#include "item.h"
#include "load_save.h"
#include "test/test.h"
#include "constants/hold_effects.h"
#include "data/frontier_bag_items.h"

TEST("Bag redesign: every normally obtainable ordinary item fits at once")
{
    ClearBag();
    EXPECT_EQ(BAG_ITEMS_COUNT, ARRAY_COUNT(sFrontierObtainableItems));
    for (u32 i = 0; i < ARRAY_COUNT(sFrontierObtainableItems); i++)
    {
        EXPECT_EQ(GetItemPocket(sFrontierObtainableItems[i]), POCKET_ITEMS);
        EXPECT(AddBagItem(sFrontierObtainableItems[i], 1));
    }
    for (u32 i = 0; i < ARRAY_COUNT(sFrontierObtainableItems); i++)
        EXPECT(CheckBagHasItem(sFrontierObtainableItems[i], 1));
}

TEST("Bag redesign: starter inventory has every berry and only eight utility key items")
{
    static const enum Item keys[] = {
        ITEM_MEGA_RING, ITEM_Z_POWER_RING, ITEM_DYNAMAX_BAND, ITEM_TERA_ORB,
        ITEM_MACH_BIKE, ITEM_POKEBLOCK_CASE, ITEM_WAILMER_PAIL,
        ITEM_POWDER_JAR,
    };
    ClearBag();
    GiveFrontierStarterInventory();
    GiveFrontierStarterInventory();
    for (u32 i = 0; i < ARRAY_COUNT(keys); i++)
        EXPECT_EQ(CountTotalItemQuantityInBag(keys[i]), 1);
    EXPECT_EQ(GetBagItemId(POCKET_KEY_ITEMS, ARRAY_COUNT(keys)), ITEM_NONE);
    EXPECT(!CheckBagHasItem(ITEM_SS_TICKET, 1));
    EXPECT(!CheckBagHasItem(ITEM_DEVON_GOODS, 1));
    EXPECT(!CheckBagHasItem(ITEM_ENIGMA_BERRY_E_READER, 1));
    for (enum BerryId berry = 1; berry <= BAG_BERRIES_COUNT; berry++)
        EXPECT_EQ(CountTotalItemQuantityInBag(BerryTypeToItemId(berry)), MAX_BAG_ITEM_CAPACITY);
}

TEST("Bag redesign: all Mega Stones and Z-Crystals route to the stone pocket")
{
    for (enum Item item = ITEM_NONE; item < ITEMS_COUNT; item++)
    {
        enum HoldEffect effect = GetItemHoldEffect(item);
        EXPECT_EQ(GetItemPocket(item) == POCKET_STONES,
            effect == HOLD_EFFECT_MEGA_STONE || effect == HOLD_EFFECT_Z_CRYSTAL);
    }
    EXPECT_EQ(GetItemPocket(ITEM_TM01), POCKET_ITEMS);
    EXPECT_EQ(GetItemPocket(ITEM_HM01), POCKET_ITEMS);
    EXPECT_NE(GetItemTMHMMoveId(ITEM_TM01), MOVE_NONE);
}

TEST("Bag redesign: stone pocket rejects a thirty-first type and recovers after removal")
{
    enum Item first = ITEM_NONE, rejected = ITEM_NONE;
    u32 count = 0;
    ClearBag();
    for (enum Item item = ITEM_NONE + 1; item < ITEMS_COUNT; item++)
    {
        if (GetItemPocket(item) != POCKET_STONES)
            continue;
        if (first == ITEM_NONE)
            first = item;
        if (count++ < BAG_STONES_COUNT)
            EXPECT(AddBagItem(item, 1));
        else
        {
            rejected = item;
            break;
        }
    }
    EXPECT_NE(rejected, ITEM_NONE);
    struct Bag before = gSaveBlock1Ptr->bag;
    EXPECT(!CheckBagHasSpace(rejected, 1));
    EXPECT(!AddBagItem(rejected, 1));
    EXPECT_EQ(memcmp(&before, &gSaveBlock1Ptr->bag, sizeof(before)), 0);
    EXPECT(RemoveBagItem(first, 1));
    EXPECT(AddBagItem(rejected, 1));
}

TEST("Bag redesign: stones and berries keep a single stack capped at 999")
{
    enum Item item = ITEM_GENGARITE;
    PARAMETRIZE { item = ITEM_GENGARITE; }
    PARAMETRIZE { item = ITEM_CHERI_BERRY; }
    ClearBag();
    EXPECT(AddBagItem(item, 500));
    EXPECT_EQ(GetFreeSpaceForItemInBag(item), 499);
    EXPECT(!CheckBagHasSpace(item, 500));
    EXPECT(!AddBagItem(item, 500));
    EXPECT(AddBagItem(item, 499));
    EXPECT(!CheckBagHasSpace(item, 1));
    EXPECT(!AddBagItem(item, 1));
    EXPECT_EQ(CountTotalItemQuantityInBag(item), MAX_BAG_ITEM_CAPACITY);
}

TEST("Bag redesign: new pockets survive temporary bag backup and key rotation")
{
    ClearBag();
    EXPECT(AddBagItem(ITEM_GENGARITE, 2));
    EXPECT(AddBagItem(ITEM_NORMALIUM_Z, 3));
    EXPECT(AddBagItem(ITEM_POTION, 4));
    EXPECT(AddBagItem(ITEM_CHERI_BERRY, 5));
    LoadPlayerBag();
    ClearBag();
    gSaveBlock2Ptr->encryptionKey = 0x12345678;
    SavePlayerBag();
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_GENGARITE), 2);
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_NORMALIUM_Z), 3);
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_POTION), 4);
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_CHERI_BERRY), 5);
}
