#include "global.h"
#include "event_data.h"
#include "task.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/flags.h"
#include "constants/vars.h"

void Test_OptionMenuSaveTypeIcons(u8 taskId, bool32 off);
void Test_OptionMenuSaveFrontier(u8 taskId, u32 aiIndex, bool32 allowBanned);
u32 Test_OptionMenuReadFrontierAi(void);
u32 Test_OptionMenuScrollFor(u32 selection);
u32 Test_OptionMenuRowCount(void);

#define VISIBLE_ROWS 8

TEST("Option menu: the Frontier AI and banned species rows save to the hub's settings")
{
    static const u16 levels[] = {
        FRONTIER_AI_LEVEL_EASY, FRONTIER_AI_LEVEL_NORMAL, FRONTIER_AI_LEVEL_HARD, FRONTIER_AI_LEVEL_STRONGEST,
    };
    u16 oldLevel = VarGet(VAR_FRONTIER_AI_LEVEL);
    bool32 oldAllow = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);

    ResetTasks();
    for (u32 i = 0; i < ARRAY_COUNT(levels); i++)
    {
        Test_OptionMenuSaveFrontier(0, i, i % 2);
        EXPECT_EQ(VarGet(VAR_FRONTIER_AI_LEVEL), levels[i]);
        EXPECT_EQ(Test_OptionMenuReadFrontierAi(), i);
        EXPECT_EQ(FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES), i % 2);
    }
    VarSet(VAR_FRONTIER_AI_LEVEL, oldLevel);
    if (oldAllow)
        FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    else
        FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
}

TEST("Option menu: saving the other rows keeps the Frontier settings")
{
    u16 oldLevel = VarGet(VAR_FRONTIER_AI_LEVEL);
    bool32 oldAllow = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    bool32 oldOff = FlagGet(FLAG_HIDE_BATTLE_TYPE_ICONS);

    ResetTasks();
    VarSet(VAR_FRONTIER_AI_LEVEL, FRONTIER_AI_LEVEL_STRONGEST);
    FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    Test_OptionMenuSaveTypeIcons(0, TRUE);
    EXPECT_EQ(VarGet(VAR_FRONTIER_AI_LEVEL), FRONTIER_AI_LEVEL_STRONGEST);
    EXPECT(FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES));
    // An unknown saved value is shown as the usual level, not as out of range.
    VarSet(VAR_FRONTIER_AI_LEVEL, 99);
    EXPECT_EQ(Test_OptionMenuReadFrontierAi(), 1);

    VarSet(VAR_FRONTIER_AI_LEVEL, oldLevel);
    if (!oldAllow)
        FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    if (oldOff)
        FlagSet(FLAG_HIDE_BATTLE_TYPE_ICONS);
    else
        FlagClear(FLAG_HIDE_BATTLE_TYPE_ICONS);
}

TEST("Option menu: the list scrolls so the selected row stays in the window")
{
    u32 rows = Test_OptionMenuRowCount();

    EXPECT_GT(rows, VISIBLE_ROWS);
    for (u32 row = 0; row < rows; row++)
    {
        u32 scroll = Test_OptionMenuScrollFor(row);
        EXPECT_LE(scroll, row);
        EXPECT_LT(row, scroll + VISIBLE_ROWS);
        EXPECT_LE(scroll + VISIBLE_ROWS, rows);
        if (row < VISIBLE_ROWS)
            EXPECT_EQ(scroll, 0);
    }
    EXPECT_EQ(Test_OptionMenuScrollFor(rows - 1), rows - VISIBLE_ROWS);
}
