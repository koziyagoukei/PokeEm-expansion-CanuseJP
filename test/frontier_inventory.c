#include "global.h"
#include "berry.h"
#include "frontier_inventory.h"
#include "item.h"
#include "item_use.h"
#include "load_save.h"
#include "test/test.h"
#include "constants/frontier_bag.h"
#include "constants/hold_effects.h"
#include "data/frontier_bag_items.h"

static const enum Item sFusionItems[] = {
    ITEM_DNA_SPLICERS, ITEM_N_SOLARIZER, ITEM_N_LUNARIZER, ITEM_REINS_OF_UNITY,
};

TEST("Bag redesign: obtainable item catalog is independent of the saved ordinary pocket capacity")
{
    ClearBag();
    EXPECT_EQ(BAG_ITEMS_COUNT, 206);
    EXPECT_EQ(FRONTIER_OBTAINABLE_ITEMS_COUNT, ARRAY_COUNT(sFrontierObtainableItems));
    EXPECT_GE(FRONTIER_OBTAINABLE_ITEMS_COUNT, BAG_ITEMS_COUNT);
    for (u32 i = 0; i < ARRAY_COUNT(sFrontierObtainableItems); i++)
    {
        EXPECT_EQ(GetItemPocket(sFrontierObtainableItems[i]), POCKET_ITEMS);
        EXPECT_EQ(AddBagItem(sFrontierObtainableItems[i], 1), i < BAG_ITEMS_COUNT);
    }
    for (u32 i = 0; i < ARRAY_COUNT(sFrontierObtainableItems); i++)
        EXPECT_EQ(CheckBagHasItem(sFrontierObtainableItems[i], 1), i < BAG_ITEMS_COUNT);
}

TEST("Bag redesign: form-changing tools fit without displacing the starter keys")
{
    static const struct
    {
        enum Item item;
        ItemUseFunc use;
        bool32 important;
    } tools[] = {
        { ITEM_REVEAL_GLASS, ItemUseOutOfBattle_FormChange, TRUE },
        { ITEM_ROTOM_CATALOG, ItemUseOutOfBattle_RotomCatalog, TRUE },
        { ITEM_GRACIDEA, ItemUseOutOfBattle_FormChange, TRUE },
        { ITEM_PRISON_BOTTLE, ItemUseOutOfBattle_FormChange, TRUE },
        { ITEM_ZYGARDE_CUBE, ItemUseOutOfBattle_ZygardeCube, TRUE },
        { ITEM_RED_NECTAR, ItemUseOutOfBattle_FormChange_ConsumedOnUse, FALSE },
        { ITEM_YELLOW_NECTAR, ItemUseOutOfBattle_FormChange_ConsumedOnUse, FALSE },
        { ITEM_PINK_NECTAR, ItemUseOutOfBattle_FormChange_ConsumedOnUse, FALSE },
        { ITEM_PURPLE_NECTAR, ItemUseOutOfBattle_FormChange_ConsumedOnUse, FALSE },
    };
    ClearBag();
    GiveFrontierStarterInventory();
    struct ItemSlot keys[BAG_KEYITEMS_COUNT];
    memcpy(keys, gSaveBlock1Ptr->bag.keyItems, sizeof(keys));
    for (u32 i = 0; i < ARRAY_COUNT(tools); i++)
    {
        EXPECT_EQ(GetItemPocket(tools[i].item), POCKET_ITEMS);
        EXPECT_EQ(GetItemImportance(tools[i].item), tools[i].important);
        EXPECT(GetItemFieldFunc(tools[i].item) == tools[i].use);
        EXPECT(AddBagItem(tools[i].item, 1));
        EXPECT_EQ(CountTotalItemQuantityInBag(tools[i].item), 1);
    }
    EXPECT_EQ(memcmp(keys, gSaveBlock1Ptr->bag.keyItems, sizeof(keys)), 0);
}

TEST("Bag redesign: starter inventory has every berry eight utility keys and four fusion keys")
{
    static const enum Item keys[] = {
        ITEM_MEGA_RING, ITEM_Z_POWER_RING, ITEM_DYNAMAX_BAND, ITEM_TERA_ORB,
        ITEM_MACH_BIKE, ITEM_POKEBLOCK_CASE, ITEM_WAILMER_PAIL,
        ITEM_POWDER_JAR,
        ITEM_DNA_SPLICERS, ITEM_N_SOLARIZER, ITEM_N_LUNARIZER, ITEM_REINS_OF_UNITY,
    };
    ClearBag();
    GiveFrontierStarterInventory();
    GiveFrontierStarterInventory();
    for (u32 i = 0; i < ARRAY_COUNT(keys); i++)
        EXPECT_EQ(CountTotalItemQuantityInBag(keys[i]), 1);
    u32 used = 0;
    for (u32 i = 0; i < BAG_KEYITEMS_COUNT; i++)
        if (GetBagItemId(POCKET_KEY_ITEMS, i) != ITEM_NONE)
            used++;
    EXPECT_EQ(used, ARRAY_COUNT(keys));
    EXPECT(!CheckBagHasItem(ITEM_SS_TICKET, 1));
    EXPECT(!CheckBagHasItem(ITEM_DEVON_GOODS, 1));
    EXPECT(!CheckBagHasItem(ITEM_ENIGMA_BERRY_E_READER, 1));
    for (enum BerryId berry = 1; berry <= BAG_BERRIES_COUNT; berry++)
        EXPECT_EQ(CountTotalItemQuantityInBag(BerryTypeToItemId(berry)), MAX_BAG_ITEM_CAPACITY);
}

TEST("Bag redesign: fusion tools use existing key slots without changing saved pocket offsets")
{
    struct PreviousBagLayout
    {
        struct ItemSlot items[206];
        struct ItemSlot keyItems[12];
        struct ItemSlot pokeBalls[5];
        struct ItemSlot stones[30];
        struct ItemSlot berries[BAG_BERRIES_COUNT];
    };
    EXPECT_EQ(sizeof(struct Bag), sizeof(struct PreviousBagLayout));
    EXPECT_EQ(offsetof(struct Bag, items), offsetof(struct PreviousBagLayout, items));
    EXPECT_EQ(offsetof(struct Bag, keyItems), offsetof(struct PreviousBagLayout, keyItems));
    EXPECT_EQ(offsetof(struct Bag, pokeBalls), offsetof(struct PreviousBagLayout, pokeBalls));
    EXPECT_EQ(offsetof(struct Bag, stones), offsetof(struct PreviousBagLayout, stones));
    EXPECT_EQ(offsetof(struct Bag, berries), offsetof(struct PreviousBagLayout, berries));
    for (u32 i = 0; i < ARRAY_COUNT(sFusionItems); i++)
    {
        EXPECT_EQ(GetItemPocket(sFusionItems[i]), POCKET_KEY_ITEMS);
        EXPECT_NE(GetItemImportance(sFusionItems[i]), 0);
        EXPECT(GetItemFieldFunc(sFusionItems[i]) == ItemUseOutOfBattle_Fusion);
    }
}

TEST("Bag redesign: old eight-key inventory gains fusion tools without changing a full ordinary pocket")
{
    static const enum Item oldKeys[] = {
        ITEM_MEGA_RING, ITEM_Z_POWER_RING, ITEM_DYNAMAX_BAND, ITEM_TERA_ORB,
        ITEM_MACH_BIKE, ITEM_POKEBLOCK_CASE, ITEM_WAILMER_PAIL, ITEM_POWDER_JAR,
    };
    ClearBag();
    for (u32 i = 0; i < ARRAY_COUNT(oldKeys); i++)
        EXPECT(AddBagItem(oldKeys[i], 1));
    for (u32 i = 0; i < BAG_ITEMS_COUNT; i++)
        EXPECT(AddBagItem(sFrontierObtainableItems[i], 1 + i));
    struct ItemSlot ordinary[206];
    memcpy(ordinary, gSaveBlock1Ptr->bag.items, sizeof(ordinary));
    GiveFrontierStarterInventory();
    for (u32 i = 0; i < ARRAY_COUNT(sFusionItems); i++)
        EXPECT_EQ(CountTotalItemQuantityInBag(sFusionItems[i]), 1);
    EXPECT_EQ(memcmp(ordinary, gSaveBlock1Ptr->bag.items, sizeof(ordinary)), 0);
    struct Bag before = gSaveBlock1Ptr->bag;
    GiveFrontierStarterInventory();
    EXPECT_EQ(memcmp(&before, &gSaveBlock1Ptr->bag, sizeof(before)), 0);
}

TEST("Bag redesign: full key pocket never discards owned items and retries missing gifts after space is freed")
{
    static const enum Item oldKeys[] = {
        ITEM_MEGA_RING, ITEM_Z_POWER_RING, ITEM_DYNAMAX_BAND, ITEM_TERA_ORB,
        ITEM_MACH_BIKE, ITEM_POKEBLOCK_CASE, ITEM_WAILMER_PAIL, ITEM_POWDER_JAR,
        ITEM_SS_TICKET, ITEM_DEVON_GOODS, ITEM_GO_GOGGLES, ITEM_DEVON_SCOPE,
    };
    ClearBag();
    for (u32 i = 0; i < ARRAY_COUNT(oldKeys); i++)
        EXPECT(AddBagItem(oldKeys[i], 1));
    struct ItemSlot before[BAG_KEYITEMS_COUNT];
    memcpy(before, gSaveBlock1Ptr->bag.keyItems, sizeof(before));
    GiveFrontierStarterInventory();
    EXPECT_EQ(memcmp(before, gSaveBlock1Ptr->bag.keyItems, sizeof(before)), 0);
    for (u32 i = 0; i < ARRAY_COUNT(sFusionItems); i++)
        EXPECT(!CheckBagHasItem(sFusionItems[i], 1));
    EXPECT(RemoveBagItem(ITEM_SS_TICKET, 1));
    GiveFrontierStarterInventory();
    EXPECT_EQ(CountTotalItemQuantityInBag(ITEM_DNA_SPLICERS), 1);
    for (u32 i = 1; i < ARRAY_COUNT(sFusionItems); i++)
        EXPECT(!CheckBagHasItem(sFusionItems[i], 1));
    for (u32 i = 0; i < ARRAY_COUNT(oldKeys); i++)
        if (oldKeys[i] != ITEM_SS_TICKET)
            EXPECT_EQ(CountTotalItemQuantityInBag(oldKeys[i]), 1);
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
    for (u32 i = 0; i < ARRAY_COUNT(sFusionItems); i++)
        EXPECT(AddBagItem(sFusionItems[i], 1));
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
    for (u32 i = 0; i < ARRAY_COUNT(sFusionItems); i++)
        EXPECT_EQ(CountTotalItemQuantityInBag(sFusionItems[i]), 1);
}
