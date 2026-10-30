#include "global.h"
#include "event_data.h"
#include "frontier_util.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/frontier_util.h"

TEST("Frontier streaks: setting a challenge's streak keeps it active and raises only its own record")
{
    struct { u8 facility, mode; u32 flag50; } challenge = {0};

    PARAMETRIZE { challenge.facility = FRONTIER_FACILITY_TOWER; challenge.mode = FRONTIER_MODE_DOUBLES; challenge.flag50 = STREAK_TOWER_DOUBLES_50; }
    PARAMETRIZE { challenge.facility = FRONTIER_FACILITY_DOME; challenge.mode = FRONTIER_MODE_SINGLES; challenge.flag50 = STREAK_DOME_SINGLES_50; }
    PARAMETRIZE { challenge.facility = FRONTIER_FACILITY_FACTORY; challenge.mode = FRONTIER_MODE_DOUBLES; challenge.flag50 = STREAK_FACTORY_DOUBLES_50; }
    PARAMETRIZE { challenge.facility = FRONTIER_FACILITY_ARENA; challenge.mode = FRONTIER_MODE_SINGLES; challenge.flag50 = STREAK_ARENA_50; }
    PARAMETRIZE { challenge.facility = FRONTIER_FACILITY_PYRAMID; challenge.mode = FRONTIER_MODE_SINGLES; challenge.flag50 = STREAK_PYRAMID_50; }

    memset(&gSaveBlock2Ptr->frontier, 0, sizeof(gSaveBlock2Ptr->frontier));
    for (u32 lvl = FRONTIER_LVL_50; lvl <= FRONTIER_LVL_OPEN; lvl++)
    {
        u32 flag = challenge.flag50 << lvl;

        EXPECT(SetFrontierWinStreak(challenge.facility, challenge.mode, lvl, 34));
        EXPECT_EQ(GetFrontierWinStreak(challenge.facility, challenge.mode, lvl), 34);
        EXPECT(gSaveBlock2Ptr->frontier.winStreakActiveFlags & flag);

        // The streak used by the facility itself is the one that changed.
        VarSet(VAR_FRONTIER_FACILITY, challenge.facility);
        VarSet(VAR_FRONTIER_BATTLE_MODE, challenge.mode);
        gSaveBlock2Ptr->frontier.lvlMode = lvl;
        EXPECT_EQ(GetCurrentFacilityWinStreak(), 34);

        EXPECT(SetFrontierWinStreak(challenge.facility, challenge.mode, lvl, 0));
        EXPECT_EQ(GetFrontierWinStreak(challenge.facility, challenge.mode, lvl), 0);
        EXPECT(!(gSaveBlock2Ptr->frontier.winStreakActiveFlags & flag));
    }
    // Only this challenge's bits were touched.
    EXPECT_EQ(gSaveBlock2Ptr->frontier.winStreakActiveFlags, 0);
    EXPECT_EQ(GetFrontierWinStreak(challenge.facility, challenge.mode == FRONTIER_MODE_SINGLES, FRONTIER_LVL_50), 0);
}

TEST("Frontier streaks: modes a facility lacks are refused and streaks are capped")
{
    memset(&gSaveBlock2Ptr->frontier, 0, sizeof(gSaveBlock2Ptr->frontier));
    EXPECT(!SetFrontierWinStreak(FRONTIER_FACILITY_ARENA, FRONTIER_MODE_DOUBLES, FRONTIER_LVL_50, 7));
    EXPECT(!SetFrontierWinStreak(FRONTIER_FACILITY_DOME, FRONTIER_MODE_MULTIS, FRONTIER_LVL_50, 7));
    EXPECT(!SetFrontierWinStreak(FRONTIER_FACILITY_TOWER, FRONTIER_MODE_SINGLES, FRONTIER_LVL_TENT, 7));
    EXPECT_EQ(gSaveBlock2Ptr->frontier.winStreakActiveFlags, 0);
    EXPECT(SetFrontierWinStreak(FRONTIER_FACILITY_PIKE, FRONTIER_MODE_SINGLES, FRONTIER_LVL_OPEN, 0xFFFF));
    EXPECT_EQ(GetFrontierWinStreak(FRONTIER_FACILITY_PIKE, FRONTIER_MODE_SINGLES, FRONTIER_LVL_OPEN), MAX_STREAK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.pikeRecordStreaks[FRONTIER_LVL_OPEN], MAX_STREAK);
    EXPECT(gSaveBlock2Ptr->frontier.winStreakActiveFlags & STREAK_PIKE_OPEN);
}
