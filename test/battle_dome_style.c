#include "global.h"
#include "battle_dome.h"
#include "battle_frontier.h"
#include "frontier_util.h"
#include "test/test.h"
#include "constants/battle_dome.h"
#include "constants/battle_frontier_mons.h"
#include "constants/moves.h"

#define DOME_MOVES_COUNT (FRONTIER_PARTY_SIZE * MAX_MON_MOVES)

static u32 GetStyleOf(enum Move move1, enum Move move2, enum Move move3)
{
    enum Move moves[DOME_MOVES_COUNT] = {move1, move2, move3};
    return Test_GetDomeBattleStyle(moves);
}

TEST("Dome battle style: a move few pool sets use is rare, a common one is not")
{
    // Acid Armor is used by a single set of the Frontier pool, Earthquake by about one in eight.
    EXPECT_EQ(GetStyleOf(MOVE_ACID_ARMOR, MOVE_NONE, MOVE_NONE), DOME_BATTLE_STYLE_RARE_MOVE);
    EXPECT_EQ(GetStyleOf(MOVE_ACID_ARMOR, MOVE_ALLY_SWITCH, MOVE_NONE), DOME_BATTLE_STYLE_RARE_MOVES);
    EXPECT_EQ(GetStyleOf(MOVE_EARTHQUAKE, MOVE_NONE, MOVE_NONE), DOME_BATTLE_STYLE_BALANCED);
}

TEST("Dome battle style: every move adds to the points, empty slots add nothing")
{
    // Three strong, sure-hit moves together make a straightforward style.
    EXPECT_EQ(GetStyleOf(MOVE_EARTHQUAKE, MOVE_EARTHQUAKE, MOVE_EARTHQUAKE), DOME_BATTLE_STYLE_STRAIGHTFORWARD);
    EXPECT_EQ(GetStyleOf(MOVE_NONE, MOVE_NONE, MOVE_NONE), DOME_BATTLE_STYLE_BALANCED);
}

TEST("Dome battle style: teams from the Frontier pool are seldom called rare-move users")
{
    u32 rare = 0, teams = 0, styles[NUM_BATTLE_STYLES] = {0};

    // Spread the three members over the pool so each team mixes different species.
    for (u32 first = 0; first < NUM_FRONTIER_MONS; first += 10)
    {
        enum Move moves[DOME_MOVES_COUNT];
        bool32 usable = TRUE;

        for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
        {
            u32 monId = (first + i * (NUM_FRONTIER_MONS / FRONTIER_PARTY_SIZE)) % NUM_FRONTIER_MONS;
            if (!IsFrontierMonPoolEntryEnabled(monId))
                usable = FALSE;
            for (u32 j = 0; j < MAX_MON_MOVES; j++)
                moves[i * MAX_MON_MOVES + j] = gBattleFrontierMons[monId].moves[j];
        }
        if (!usable)
            continue;
        u32 style = Test_GetDomeBattleStyle(moves);
        styles[style]++;
        teams++;
        if (style == DOME_BATTLE_STYLE_RARE_MOVES || style == DOME_BATTLE_STYLE_RARE_MOVE)
            rare++;
    }
    for (u32 i = 0; i < NUM_BATTLE_STYLES; i++)
    {
        if (styles[i] != 0)
            Test_MgbaPrintf("style %d: %d of %d teams", i, styles[i], teams);
    }
    EXPECT_LT(rare * 3, teams);
}
