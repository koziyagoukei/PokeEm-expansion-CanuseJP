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

TEST("Frontier Factory: restricted opponent draws count each base species once")
{
    enum Species excluded[255];
    u32 excludedCount = 0;
    u32 kyuremCount = 0;
    bool32 sawKyuremWhite = FALSE;

    FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    for (enum Species species = 1; species < NUM_SPECIES; species++)
    {
        if (IsFrontierSpeciesSubjectToBannedLimit(species)
         && species != SPECIES_MEW && GET_BASE_SPECIES_ID(species) != SPECIES_KYUREM)
        {
            EXPECT_LT(excludedCount, ARRAY_COUNT(excluded));
            excluded[excludedCount++] = species;
        }
    }

    SeedRng(0x4359AB21);
    for (u32 i = 0; i < 96; i++)
    {
        u16 id = GetRandomFactoryOpponentMon(NULL, 0, NULL, 0, excluded, excludedCount, FRONTIER_MON_FILTER_BANNED_SPECIES);
        enum Species species = gFacilityTrainerMons[id].species;

        EXPECT(species == SPECIES_MEW || GET_BASE_SPECIES_ID(species) == SPECIES_KYUREM);
        kyuremCount += (species != SPECIES_MEW);
        sawKyuremWhite |= (species == SPECIES_KYUREM_WHITE);
    }
    EXPECT_GE(kyuremCount, 32);
    EXPECT_LE(kyuremCount, 64);
    EXPECT(sawKyuremWhite);
}

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
