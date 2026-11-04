#include "global.h"
#include "battle.h"
#include "battle_frontier.h"
#include "battle_setup.h"
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

static u32 CountBannedSpecies(enum BattleTrainer trainer)
{
    u32 i, count = 0;

    for (i = 0; i < PARTY_SIZE; i++)
        count += gSpeciesInfo[GetMonData(&gParties[trainer][i], MON_DATA_SPECIES)].isFrontierBanned;
    return count;
}

// A forced banned-species slot has no entry left once the party already has one. The last
// resort then picked any Pokemon at all, so a second banned species could get in.
TEST("Frontier pool draws keep the one-banned-species limit when a forced filter has nothing left")
{
    bool32 oldAllow = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    u32 seed = 0;

    PARAMETRIZE { seed = 1; }
    PARAMETRIZE { seed = 77; }

    FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    SeedRng(seed);
    for (u32 draw = 0; draw < 50; draw++)
    {
        u16 party[2];

        party[0] = GetRandomFrontierMonFromFullPoolWithFilter(party, 0, NULL, 0, NULL, 0, FRONTIER_MON_FILTER_BANNED_SPECIES);
        party[1] = GetRandomFrontierMonFromFullPoolWithFilter(party, 1, NULL, 0, NULL, 0, FRONTIER_MON_FILTER_BANNED_SPECIES);
        EXPECT(gSpeciesInfo[gBattleFrontierMons[party[0]].species].isFrontierBanned);
        EXPECT(!gSpeciesInfo[gBattleFrontierMons[party[1]].species].isFrontierBanned);
    }

    if (!oldAllow)
        FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
}

// From the fourth battle on, each Trainer's party was given a banned species, so both
// opponents of a multi battle brought one.
TEST("The two opponents of a multi battle bring one banned species between them")
{
    bool32 oldAllow = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    u16 oldBattleNum = gSaveBlock2Ptr->frontier.curChallengeBattleNum;
    u8 oldLvlMode = gSaveBlock2Ptr->frontier.lvlMode;
    u32 seed = 0;

    PARAMETRIZE { seed = 1; }
    PARAMETRIZE { seed = 77; }

    FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = 3;
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_50;
    gBattleTypeFlags = 0;
    TRAINER_BATTLE_PARAM.opponentA = 0;
    TRAINER_BATTLE_PARAM.opponentB = 1;
    SeedRng(seed);
    for (u32 battle = 0; battle < 20; battle++)
    {
        FillFrontierTrainersParties(FRONTIER_MULTI_PARTY_SIZE);
        EXPECT_EQ(CountBannedSpecies(B_TRAINER_OPPONENT_A), 1);
        EXPECT_EQ(CountBannedSpecies(B_TRAINER_OPPONENT_B), 0);
    }

    ZeroEnemyPartyMons();
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = oldBattleNum;
    gSaveBlock2Ptr->frontier.lvlMode = oldLvlMode;
    if (!oldAllow)
        FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
}
