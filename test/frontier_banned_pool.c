#include "global.h"
#include "battle_frontier.h"
#include "event_data.h"
#include "frontier_util.h"
#include "random.h"
#include "test/test.h"
#include "constants/flags.h"

// The Dome drew its tournament parties before setting the facility pointers, so the
// restricted-legendary rule was checked against the wrong table and opponents got banned species.
TEST("Frontier pool draws respect the banned-species rule even with stale facility pointers")
{
    static const struct TrainerMon sStalePool[1] = {0};
    bool32 oldAllow = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    u32 seed = 0;

    PARAMETRIZE { seed = 1; }
    PARAMETRIZE { seed = 77; }
    PARAMETRIZE { seed = 4242; }

    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    SeedRng(seed);
    for (u32 draw = 0; draw < 200; draw++)
    {
        u16 party[FRONTIER_PARTY_SIZE];

        gFacilityTrainerMons = sStalePool;
        for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
        {
            party[i] = GetRandomFrontierMonFromFullPool(party, i, NULL, 0, NULL, 0);
            EXPECT(!gSpeciesInfo[gBattleFrontierMons[party[i]].species].isFrontierBanned);
        }
    }

    if (oldAllow)
        FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
}
