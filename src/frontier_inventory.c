#include "global.h"
#include "berry.h"
#include "frontier_inventory.h"
#include "item.h"

static const enum Item sStarterKeyItems[] =
{
    ITEM_MEGA_RING,
    ITEM_Z_POWER_RING,
    ITEM_DYNAMAX_BAND,
    ITEM_TERA_ORB,
    ITEM_MACH_BIKE,
    ITEM_POKEBLOCK_CASE,
    ITEM_WAILMER_PAIL,
    ITEM_POWDER_JAR,
};

STATIC_ASSERT(ARRAY_COUNT(sStarterKeyItems) <= BAG_KEYITEMS_COUNT, StarterKeyItemsFit);
STATIC_ASSERT(BERRY_ID_ENGIMA_E_READER == NUM_BERRIES, LegacyBerryLast);

void GiveFrontierStarterInventory(void)
{
    for (u32 i = 0; i < ARRAY_COUNT(sStarterKeyItems); i++)
    {
        if (!CheckBagHasItem(sStarterKeyItems[i], 1))
            AddBagItem(sStarterKeyItems[i], 1);
    }

    for (enum BerryId berry = 1; berry <= BAG_BERRIES_COUNT; berry++)
    {
        enum Item item = BerryTypeToItemId(berry);
        if (!CheckBagHasItem(item, 1))
            AddBagItem(item, MAX_BAG_ITEM_CAPACITY);
    }
}
