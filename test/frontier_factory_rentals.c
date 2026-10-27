#include "global.h"
#include "battle_factory.h"
#include "battle_frontier.h"
#include "event_data.h"
#include "frontier_util.h"
#include "pokemon.h"
#include "random.h"
#include "test/test.h"
#include "constants/battle_factory.h"
#include "constants/battle_frontier.h"
#include "constants/battle_frontier_mons.h"

TEST("Frontier Factory: initial rentals obey the restricted species setting")
{
    u32 unlocked = FALSE, levelMode = FRONTIER_LVL_50;
    PARAMETRIZE { unlocked = FALSE; levelMode = FRONTIER_LVL_50; }
    PARAMETRIZE { unlocked = FALSE; levelMode = FRONTIER_LVL_OPEN; }
    PARAMETRIZE { unlocked = TRUE; levelMode = FRONTIER_LVL_50; }
    PARAMETRIZE { unlocked = TRUE; levelMode = FRONTIER_LVL_OPEN; }

    if (unlocked)
        FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    else
        FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.lvlMode = levelMode;
    gSaveBlock2Ptr->frontier.factoryWinStreaks[FRONTIER_MODE_SINGLES][levelMode] = 0;
    gSaveBlock2Ptr->frontier.factoryRentsCount[FRONTIER_MODE_SINGLES][levelMode] = 0;
    gSpecialVar_0x8004 = BATTLE_FACTORY_FUNC_GENERATE_RENTAL_MONS;
    SeedRng(0x87654321);
    u32 restrictedCount = 0;
    for (u32 round = 0; round < 128; round++)
    {
        CallBattleFactoryFunction();
        for (u32 i = 0; i < PARTY_SIZE; i++)
        {
            u16 id = gSaveBlock2Ptr->frontier.rentalMons[i].monId;
            EXPECT_LT(id, NUM_FRONTIER_MONS);
            enum Species species = gFacilityTrainerMons[id].species;
            EXPECT(!IsFrontierSpeciesBannedByCurrentRules(species));
            restrictedCount += gSpeciesInfo[species].isFrontierBanned;
        }
    }
    if (unlocked)
        EXPECT_GT(restrictedCount, 0);
    else
        EXPECT_EQ(restrictedCount, 0);
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
}
