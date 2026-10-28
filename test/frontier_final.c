#include "global.h"
#include "battle_setup.h"
#include "battle_transition.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "frontier_story.h"
#include "test/test.h"
#include "constants/event_objects.h"
#include "constants/frontier_story.h"
#include "constants/vars.h"

static const u16 sRescuedFlags[] =
{
    FLAG_STORY_RESCUED_TOWER,
    FLAG_STORY_RESCUED_DOME,
    FLAG_STORY_RESCUED_PALACE,
    FLAG_STORY_RESCUED_ARENA,
    FLAG_STORY_RESCUED_FACTORY,
    FLAG_STORY_RESCUED_PIKE,
    FLAG_STORY_RESCUED_PYRAMID,
    FLAG_STORY_RESCUED_CONTEST,
};

static void SetRescued(u32 count)
{
    for (u32 i = 0; i < ARRAY_COUNT(sRescuedFlags); i++)
    {
        if (i < count)
            FlagSet(sRescuedFlags[i]);
        else
            FlagClear(sRescuedFlags[i]);
    }
}

static bool32 ShouldGather(void)
{
    FrontierStory_ShouldGather();
    return gSpecialVar_Result;
}

TEST("Frontier final: the rescued gather on the beach once, after the fourth rescue")
{
    u32 intro = VarGet(VAR_FRONTIER_INTRO_STATE);

    VarSet(VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE);
    FlagClear(FLAG_STORY_PATH_OPEN);
    SetRescued(3);
    EXPECT(!ShouldGather());
    SetRescued(4);
    EXPECT(ShouldGather());
    SetRescued(8);
    EXPECT(ShouldGather());
    FlagSet(FLAG_STORY_PATH_OPEN);
    EXPECT(!ShouldGather());
    FlagClear(FLAG_STORY_PATH_OPEN);
    // Saves from before the opening keep the old Frontier.
    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    EXPECT(!ShouldGather());
    SetRescued(0);
    VarSet(VAR_FRONTIER_INTRO_STATE, intro);
}

TEST("Frontier final: the final chapter's flags and ending state are their own")
{
    static const u16 sFlags[] =
    {
        FLAG_STORY_PATH_OPEN, FLAG_STORY_DEPTHS_ENTERED, FLAG_STORY_DEPTHS_HELIX_TALK,
        FLAG_STORY_POKEVIRUS_FADED, FLAG_STORY_INN_OPEN, FLAG_STORY_TOWER_RELEASE_SHOWN,
        FLAG_HIDE_BATTLE_TYPE_ICONS,
    };

    for (u32 i = 0; i < ARRAY_COUNT(sFlags); i++)
        for (u32 j = i + 1; j < ARRAY_COUNT(sFlags); j++)
            EXPECT_NE(sFlags[i], sFlags[j]);
    EXPECT_NE(VAR_STORY_ENDING_STATE, VAR_STORY_TARGET_FEVERS);
    EXPECT_NE(VAR_STORY_ENDING_STATE, VAR_CONTEST_STORY_SCENES);
    EXPECT_LE(VAR_STORY_ENDING_STATE, VARS_END);
}

TEST("Frontier final: the rift uses its own small graphic in Pokevirus's colors")
{
    const struct ObjectEventGraphicsInfo *rift = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_POKEVIRUS_RIFT);
    const struct ObjectEventGraphicsInfo *pokevirus = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_POKEVIRUS);

    EXPECT_NE(rift, pokevirus);
    EXPECT_EQ(rift->paletteTag, pokevirus->paletteTag);
    EXPECT_EQ(rift->width, 32);
    EXPECT_EQ(rift->height, 32);
    EXPECT(rift->inanimate);
    EXPECT_NE(rift->images, NULL);
}

TEST("Frontier final: the last battle uses the release transition")
{
    InitTrainerBattleParameter();
    SetPokevirusFinalBattle();
    EXPECT_EQ(GetTrainerBattleTransition(), B_TRANSITION_POKEVIRUS_RELEASE);
    // Another battle set up afterwards is an ordinary one again.
    InitTrainerBattleParameter();
    EXPECT_NE(GetTrainerBattleTransition(), B_TRANSITION_POKEVIRUS_RELEASE);
}
