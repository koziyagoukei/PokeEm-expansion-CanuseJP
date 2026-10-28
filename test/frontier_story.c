#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "data.h"
#include "event_data.h"
#include "frontier_guest.h"
#include "frontier_story.h"
#include "frontier_util.h"
#include "pokemon.h"
#include "random.h"
#include "string_util.h"
#include "test/test.h"
#include "trainer_pools.h"
#include "constants/abilities.h"
#include "constants/battle_ai.h"
#include "constants/battle_frontier.h"
#include "constants/characters.h"
#include "constants/event_objects.h"
#include "constants/frontier_story.h"
#include "constants/frontier_util.h"
#include "constants/map_event_ids.h"
#include "constants/opponents.h"
#include "constants/trainers.h"

#define BRAIN(facility) (NAMED_STORY_BRAIN_FIRST + (facility))
#define GUEST(trainerId) ((trainerId) - TRAINER_FRONTIER_GUEST_FIRST)

// The battle test harness replaces gTrainers with dummies. Check the real generated data here.
static const struct Trainer sStoryTestTrainers[DIFFICULTY_COUNT][TRAINERS_COUNT] =
{
#include "../src/data/trainers.h"
};

static u32 NamedState(u32 id)
{
    u32 flag = FLAG_STORY_NAMED_STATES_START + id * 2;
    return FlagGet(flag) | (FlagGet(flag + 1) << 1);
}

static void SetNamed(u32 id, u32 state)
{
    u32 flag = FLAG_STORY_NAMED_STATES_START + id * 2;
    if (state & 1) FlagSet(flag); else FlagClear(flag);
    if (state & 2) FlagSet(flag + 1); else FlagClear(flag + 1);
}

static void ResetStory(void)
{
    VarSet(VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE);
    for (u32 i = 0; i < NAMED_STORY_COUNT; i++)
        SetNamed(i, NAMED_STORY_INFECTED);
    for (u32 i = 0; i < NUM_FRONTIER_FACILITIES; i++)
    {
        FlagClear(gFrontierBrainInfo[i].silverSymbolFlag);
        FlagClear(gFrontierBrainInfo[i].goldSymbolFlag);
    }
    FlagClear(FLAG_STORY_CURE_FOUND);
    FlagClear(FLAG_STORY_DARKRAI_JOINED);
    FlagClear(FLAG_STORY_INN_OPEN);
    FlagClear(FLAG_STORY_INN_VISITED);
    FlagClear(FLAG_STORY_TOWER_FEVER_SHOWN);
    FlagClear(FLAG_STORY_TOWER_RELEASE_SHOWN);
    VarSet(VAR_TOWER_STORY_STATE, TOWER_STORY_NOT_MET);
    VarSet(VAR_STORY_TARGET_STATES, 0);
    VarSet(VAR_STORY_TARGET_STATES_2, 0);
    VarSet(VAR_CONTEST_STORY_SCENES, 0);
    VarSet(VAR_STORY_TARGET_FEVERS, 0);
    for (u32 i = FLAG_STORY_RESCUED_TOWER; i <= FLAG_STORY_RESCUED_CONTEST; i++)
        FlagClear(i);
    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_TOWER);
    TRAINER_BATTLE_PARAM.opponentA = 0;
    TRAINER_BATTLE_PARAM.opponentB = 0;
}

static void WinFrontierBattle(u16 opponentA, u16 opponentB, u32 extraFlags)
{
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | extraFlags;
    TRAINER_BATTLE_PARAM.opponentA = opponentA;
    TRAINER_BATTLE_PARAM.opponentB = opponentB;
    gBattleOutcome = B_OUTCOME_WON;
    FrontierStory_OnBattleFinished();
}

TEST("Frontier story: a guest is uneasy after the first loss and remembers after the second")
{
    ResetStory();
    WinFrontierBattle(TRAINER_FRONTIER_WALLACE, 0, 0);
    EXPECT_EQ(NamedState(GUEST(TRAINER_FRONTIER_WALLACE)), NAMED_STORY_UNEASY);
    WinFrontierBattle(TRAINER_FRONTIER_WALLACE, 0, 0);
    EXPECT_EQ(NamedState(GUEST(TRAINER_FRONTIER_WALLACE)), NAMED_STORY_RECOVERED);
    WinFrontierBattle(TRAINER_FRONTIER_WALLACE, 0, 0);
    EXPECT_EQ(NamedState(GUEST(TRAINER_FRONTIER_WALLACE)), NAMED_STORY_RECOVERED);
}

TEST("Frontier story: Tate and Liza share one memory, also in a multi battle")
{
    ResetStory();
    WinFrontierBattle(TRAINER_FRONTIER_TATE, TRAINER_FRONTIER_LIZA, BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_DOUBLE);
    EXPECT_EQ(NamedState(GUEST(TRAINER_FRONTIER_TATE_AND_LIZA)), NAMED_STORY_UNEASY);
    WinFrontierBattle(TRAINER_FRONTIER_TATE_AND_LIZA, 0, 0);
    EXPECT_EQ(NamedState(GUEST(TRAINER_FRONTIER_TATE_AND_LIZA)), NAMED_STORY_RECOVERED);
}

TEST("Frontier story: link, recorded, lost and pre-opening battles leave memories alone")
{
    ResetStory();
    WinFrontierBattle(TRAINER_FRONTIER_STEVEN, 0, BATTLE_TYPE_LINK);
    WinFrontierBattle(TRAINER_FRONTIER_STEVEN, 0, BATTLE_TYPE_RECORDED);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    gBattleOutcome = B_OUTCOME_LOST;
    FrontierStory_OnBattleFinished();
    EXPECT_EQ(NamedState(GUEST(TRAINER_FRONTIER_STEVEN)), NAMED_STORY_INFECTED);
    VarSet(VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_LEGACY);
    WinFrontierBattle(TRAINER_FRONTIER_STEVEN, 0, 0);
    EXPECT_EQ(NamedState(GUEST(TRAINER_FRONTIER_STEVEN)), NAMED_STORY_INFECTED);
    // Older saves keep the original lines.
    EXPECT(!FrontierStory_TryCopyGuestText(FRONTIER_BEFORE_TEXT, TRAINER_FRONTIER_STEVEN));
}

TEST("Frontier story: guest lines follow their memory")
{
    u8 recovered[64];

    ResetStory();
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    EXPECT(FrontierStory_TryCopyGuestText(FRONTIER_BEFORE_TEXT, TRAINER_FRONTIER_NORMAN));
    EXPECT(FrontierStory_TryCopyGuestText(FRONTIER_PLAYER_LOST_TEXT, TRAINER_FRONTIER_NORMAN));
    EXPECT(FrontierStory_TryCopyGuestText(FRONTIER_PLAYER_WON_TEXT, TRAINER_FRONTIER_NORMAN));
    SetNamed(GUEST(TRAINER_FRONTIER_NORMAN), NAMED_STORY_UNEASY);
    EXPECT(FrontierStory_TryCopyGuestText(FRONTIER_PLAYER_WON_TEXT, TRAINER_FRONTIER_NORMAN));
    StringCopy(recovered, gStringVar4);
    SetNamed(GUEST(TRAINER_FRONTIER_FLANNERY), NAMED_STORY_UNEASY);
    EXPECT(FrontierStory_TryCopyGuestText(FRONTIER_PLAYER_WON_TEXT, TRAINER_FRONTIER_FLANNERY));
    // Each person remembers in their own words.
    EXPECT_NE(StringCompare(recovered, gStringVar4), 0);
    SetNamed(GUEST(TRAINER_FRONTIER_NORMAN), NAMED_STORY_RECOVERED);
    EXPECT(!FrontierStory_TryCopyGuestText(FRONTIER_BEFORE_TEXT, TRAINER_FRONTIER_NORMAN));
    EXPECT(!FrontierStory_TryCopyGuestText(FRONTIER_PLAYER_WON_TEXT, TRAINER_FRONTIER_NORMAN));
    SetNamed(GUEST(TRAINER_FRONTIER_NORMAN), NAMED_STORY_RESCUED);
    EXPECT(FrontierStory_TryCopyGuestText(FRONTIER_BEFORE_TEXT, TRAINER_FRONTIER_NORMAN));
}

TEST("Frontier story: a Brain is uneasy after the Silver battle and remembers at Gold or later")
{
    ResetStory();
    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_ARENA);
    WinFrontierBattle(TRAINER_FRONTIER_BRAIN, 0, 0);
    EXPECT_EQ(NamedState(BRAIN(FRONTIER_FACILITY_ARENA)), NAMED_STORY_UNEASY);
    FlagSet(FLAG_SYS_ARENA_SILVER);
    WinFrontierBattle(TRAINER_FRONTIER_BRAIN, 0, 0);
    EXPECT_EQ(NamedState(BRAIN(FRONTIER_FACILITY_ARENA)), NAMED_STORY_RECOVERED);
    // A save that already had the Silver Symbol goes straight to remembering.
    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_PIKE);
    FlagSet(FLAG_SYS_PIKE_SILVER);
    WinFrontierBattle(TRAINER_FRONTIER_BRAIN, 0, 0);
    EXPECT_EQ(NamedState(BRAIN(FRONTIER_FACILITY_PIKE)), NAMED_STORY_RECOVERED);
    EXPECT_EQ(NamedState(BRAIN(FRONTIER_FACILITY_TOWER)), NAMED_STORY_INFECTED);
}

TEST("Frontier story: Brain battle lines and intros follow their memory")
{
    ResetStory();
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_TOWER);
    FrontierStory_BufferBrainIntro();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT(FrontierStory_TryCopyBrainBattleText(FRONTIER_FACILITY_TOWER, FALSE));
    EXPECT(FrontierStory_TryCopyBrainBattleText(FRONTIER_FACILITY_TOWER, TRUE));
    SetNamed(BRAIN(FRONTIER_FACILITY_TOWER), NAMED_STORY_UNEASY);
    FrontierStory_BufferBrainIntro();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    SetNamed(BRAIN(FRONTIER_FACILITY_TOWER), NAMED_STORY_RECOVERED);
    FrontierStory_BufferBrainIntro();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    EXPECT(!FrontierStory_TryCopyBrainBattleText(FRONTIER_FACILITY_TOWER, TRUE));
}

TEST("Frontier story: story battles match the player's highest level, never below 50")
{
    ZeroPlayerPartyMons();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 30, 0, OTID_STRUCT_PLAYER_ID);
    FrontierStory_PrepareMatchedBattle();
    EXPECT_EQ(FrontierStory_GetTrainerMonLevel(5), 50);
    CreateMon(&gParties[B_TRAINER_PLAYER][1], SPECIES_PIKACHU, 100, 0, OTID_STRUCT_PLAYER_ID);
    FrontierStory_PrepareMatchedBattle();
    EXPECT_EQ(FrontierStory_GetTrainerMonLevel(5), 100);
    EXPECT(!FrontierStory_IsReleaseBattle());
    gBattleOutcome = B_OUTCOME_WON;
    FrontierStory_EndStoryBattle();
    EXPECT_EQ(gSpecialVar_Result, B_OUTCOME_WON);
    EXPECT_EQ(FrontierStory_GetTrainerMonLevel(5), 5);
}

TEST("Frontier story: the release battle flag ends with the battle")
{
    ZeroPlayerPartyMons();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 100, 0, OTID_STRUCT_PLAYER_ID);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER;
    FrontierStory_PrepareReleaseBattle();
    EXPECT(FrontierStory_IsReleaseBattle());
    gBattleTypeFlags |= BATTLE_TYPE_LINK;
    EXPECT(!FrontierStory_IsReleaseBattle());
    gBattleTypeFlags &= ~BATTLE_TYPE_LINK;
    gBattleOutcome = B_OUTCOME_LOST;
    FrontierStory_OnBattleFinished();
    EXPECT(!FrontierStory_IsReleaseBattle());
    EXPECT_EQ(FrontierStory_GetTrainerMonLevel(5), 5);
}

TEST("Frontier story: camp visitors come only after the cure, and leave cured")
{
    u32 tries;

    ResetStory();
    SetNamed(BRAIN(FRONTIER_FACILITY_DOME), NAMED_STORY_RECOVERED);
    FrontierStory_PickCampVisitor();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    EXPECT_EQ(VarGet(VAR_CAMP_VISITOR), NAMED_STORY_NONE);

    FlagSet(FLAG_STORY_CURE_FOUND);
    for (tries = 0; tries < 20; tries++)
    {
        FrontierStory_PickCampVisitor();
        if (gSpecialVar_Result)
            break;
    }
    EXPECT_LT(tries, 20);
    EXPECT_EQ(VarGet(VAR_CAMP_VISITOR), BRAIN(FRONTIER_FACILITY_DOME));
    EXPECT_EQ(VarGet(VAR_CAMP_VISITOR_GFX), gFrontierBrainInfo[FRONTIER_FACILITY_DOME].objEventGfx);

    gSpecialVar_0x8004 = CAMP_VISITOR_TEXT_GREETING;
    FrontierStory_BufferVisitorText();
    EXPECT_NE(gStringVar4[0], EOS);
    FrontierStory_RescueVisitor();
    EXPECT_EQ(NamedState(BRAIN(FRONTIER_FACILITY_DOME)), NAMED_STORY_RESCUED);
    for (tries = 0; tries < 20; tries++)
    {
        FrontierStory_PickCampVisitor();
        EXPECT_EQ(gSpecialVar_Result, FALSE);
    }
}

TEST("Frontier story: rescues are counted once per scenario")
{
    ResetStory();
    FlagSet(FLAG_STORY_RESCUED_TOWER);
    FlagSet(FLAG_STORY_RESCUED_CONTEST);
    FrontierStory_CountRescued();
    EXPECT_EQ(gSpecialVar_Result, 2);
}

TEST("Frontier story: Darkrai joins after any Silver Symbol")
{
    ResetStory();
    ZeroPlayerPartyMons();
    FrontierStory_CanDarkraiJoin();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    FlagSet(FLAG_SYS_PYRAMID_SILVER);
    FrontierStory_CanDarkraiJoin();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    FrontierStory_CanDarkraiJoin();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
}

TEST("Frontier story: Darkrai suggests the inn once anyone is cured")
{
    ResetStory();
    FrontierStory_ShouldSuggestInn();
    EXPECT_EQ(gSpecialVar_Result, FALSE);

    // Not until the cure is known.
    FlagSet(FLAG_STORY_RESCUED_TOWER);
    FrontierStory_ShouldSuggestInn();
    EXPECT_EQ(gSpecialVar_Result, FALSE);

    FlagSet(FLAG_STORY_CURE_FOUND);
    FrontierStory_ShouldSuggestInn();
    EXPECT_EQ(gSpecialVar_Result, TRUE);

    // A cured Brain counts too; recovered but not yet cured does not.
    FlagClear(FLAG_STORY_RESCUED_TOWER);
    SetNamed(BRAIN(FRONTIER_FACILITY_ARENA), NAMED_STORY_RECOVERED);
    FrontierStory_ShouldSuggestInn();
    EXPECT_EQ(gSpecialVar_Result, FALSE);

    SetNamed(BRAIN(FRONTIER_FACILITY_ARENA), NAMED_STORY_RESCUED);
    FrontierStory_ShouldSuggestInn();
    EXPECT_EQ(gSpecialVar_Result, TRUE);

    FlagSet(FLAG_STORY_INN_OPEN);
    FrontierStory_ShouldSuggestInn();
    EXPECT_EQ(gSpecialVar_Result, FALSE);

    FlagClear(FLAG_STORY_INN_OPEN);
    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    FrontierStory_ShouldSuggestInn();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
}

static bool32 KazukiComesOver(void)
{
    TowerStory_CheckKazukiApproach();
    return gSpecialVar_Result;
}

TEST("Tower story: Kazuki comes over once at each step of his story")
{
    ResetStory();
    EXPECT(KazukiComesOver());
    VarSet(VAR_TOWER_STORY_STATE, TOWER_STORY_MET);
    EXPECT(!KazukiComesOver());

    FlagSet(FLAG_SYS_TOWER_SILVER);
    EXPECT(KazukiComesOver());
    VarSet(VAR_TOWER_STORY_STATE, TOWER_STORY_WISH);
    EXPECT(!KazukiComesOver());

    // The fever: once alone, then once more after Darkrai joins.
    FlagSet(FLAG_SYS_TOWER_GOLD);
    EXPECT(KazukiComesOver());
    EXPECT(!KazukiComesOver());
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    EXPECT(KazukiComesOver());
    EXPECT(!KazukiComesOver());

    VarSet(VAR_TOWER_STORY_STATE, TOWER_STORY_RETURNED);
    FlagClear(FLAG_STORY_TOWER_FEVER_SHOWN);
    FlagClear(FLAG_STORY_TOWER_RELEASE_SHOWN);
    EXPECT(!KazukiComesOver());
}

TEST("Tower story: one visit covers every step Kazuki missed, and old saves are left alone")
{
    ResetStory();
    FlagSet(FLAG_SYS_TOWER_SILVER);
    FlagSet(FLAG_SYS_TOWER_GOLD);
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    // Meeting, wish and fever run on from one another in a single talk.
    EXPECT(KazukiComesOver());
    VarSet(VAR_TOWER_STORY_STATE, TOWER_STORY_WISH);
    EXPECT(!KazukiComesOver());

    ResetStory();
    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    EXPECT(!KazukiComesOver());
}

static u32 TargetState(u32 target)
{
    if (target == STORY_TARGET_KAZUKI)
        return VarGet(VAR_TOWER_STORY_STATE);
    if (target <= STORY_TARGET_TOSHIO)
        return (VarGet(VAR_STORY_TARGET_STATES) >> ((target - STORY_TARGET_REIKA) * 4)) & 0xF;
    return (VarGet(VAR_STORY_TARGET_STATES_2) >> ((target - STORY_TARGET_YAYOI) * 4)) & 0xF;
}

static bool32 TargetComesOver(u32 target)
{
    gSpecialVar_0x8004 = target;
    FrontierStory_CheckTargetApproach();
    return gSpecialVar_Result;
}

// Talks to them; returns whether their next step follows at once.
static bool32 TalkToTarget(u32 target)
{
    gSpecialVar_0x8004 = target;
    FrontierStory_BufferTargetTalk();
    EXPECT_NE(gStringVar4[0], EOS);
    return gSpecialVar_Result;
}

static bool32 TargetBranch(u32 target, u32 talk)
{
    gSpecialVar_0x8004 = target;
    gSpecialVar_0x8005 = talk;
    FrontierStory_BufferTargetBranch();
    return gSpecialVar_Result;
}

TEST("Frontier story: each facility's person is met, then wavers after its Silver Symbol")
{
    for (u32 target = STORY_TARGET_REIKA; target <= STORY_TARGET_NOBUO; target++)
    {
        ResetStory();
        EXPECT(TargetComesOver(target));
        EXPECT(!TalkToTarget(target));
        EXPECT_EQ(TargetState(target), TARGET_STORY_MET);
        EXPECT_EQ(gSpecialVar_0x8005, TARGET_TALK_HAPPY);
        EXPECT(!TargetComesOver(target));
        TalkToTarget(target);
        EXPECT_EQ(TargetState(target), TARGET_STORY_MET);

        // Another facility's symbol does not count.
        FlagSet(gFrontierBrainInfo[target == STORY_TARGET_REIKA ? FRONTIER_FACILITY_TOWER : FRONTIER_FACILITY_DOME].silverSymbolFlag);
        EXPECT(!TargetComesOver(target));
        FlagSet(gFrontierBrainInfo[target].silverSymbolFlag);
        EXPECT(TargetComesOver(target));
        EXPECT(!TalkToTarget(target));
        EXPECT_EQ(TargetState(target), TARGET_STORY_WISH);
        EXPECT_EQ(gSpecialVar_0x8005, TARGET_TALK_WISH);
        EXPECT(!TargetComesOver(target));
        TalkToTarget(target);
        EXPECT_EQ(TargetState(target), TARGET_STORY_WISH);
        // Nobody else's step moved.
        for (u32 other = STORY_TARGET_KAZUKI; other < STORY_TARGET_COUNT; other++)
            if (other != target) EXPECT_EQ(TargetState(other), TARGET_STORY_NOT_MET);
    }
}

TEST("Frontier story: met with the symbol already won, the person's change follows at once")
{
    ResetStory();
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_PIKE].silverSymbolFlag);
    EXPECT(TalkToTarget(STORY_TARGET_YAYOI));
    EXPECT_EQ(TargetState(STORY_TARGET_YAYOI), TARGET_STORY_MET);
    EXPECT(!TalkToTarget(STORY_TARGET_YAYOI));
    EXPECT_EQ(TargetState(STORY_TARGET_YAYOI), TARGET_STORY_WISH);
}

TEST("Frontier story: the contest's person changes after the first Master Rank win, not Hyper")
{
    u8 rank = 3;

    ResetStory();
    ZeroPlayerPartyMons();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 50, 0, OTID_STRUCT_PLAYER_ID);
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SMART_RIBBON, &rank);
    TalkToTarget(STORY_TARGET_YORIKO);
    EXPECT(!TargetComesOver(STORY_TARGET_YORIKO));
    rank = 4;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SMART_RIBBON, &rank);
    EXPECT(TargetComesOver(STORY_TARGET_YORIKO));
    TalkToTarget(STORY_TARGET_YORIKO);
    EXPECT_EQ(TargetState(STORY_TARGET_YORIKO), TARGET_STORY_WISH);
    ZeroPlayerPartyMons();
}

TEST("Frontier story: people talk about whoever has left the network and whoever wavers")
{
    static const u8 sKazuki[] = _("カズキ");

    ResetStory();
    EXPECT(!TargetBranch(STORY_TARGET_REIKA, TARGET_TALK_HAPPY));
    EXPECT(!TargetBranch(STORY_TARGET_REIKA, TARGET_TALK_WISH));

    // Someone else wavering only matters once they waver themselves.
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_ARENA].silverSymbolFlag);
    while (TalkToTarget(STORY_TARGET_EISUKE))
        ;
    EXPECT_EQ(TargetState(STORY_TARGET_EISUKE), TARGET_STORY_WISH);
    EXPECT(!TargetBranch(STORY_TARGET_REIKA, TARGET_TALK_HAPPY));
    EXPECT(TargetBranch(STORY_TARGET_REIKA, TARGET_TALK_WISH));
    EXPECT(!TargetBranch(STORY_TARGET_EISUKE, TARGET_TALK_WISH)); // not about themself

    // Kazuki leaving is noticed by name, and comes before the wavering.
    VarSet(VAR_TOWER_STORY_STATE, TOWER_STORY_RESCUED);
    FlagSet(FLAG_STORY_RESCUED_TOWER);
    EXPECT(TargetBranch(STORY_TARGET_REIKA, TARGET_TALK_HAPPY));
    EXPECT_EQ(StringCompare(gStringVar2, sKazuki), 0);
    EXPECT(TargetBranch(STORY_TARGET_REIKA, TARGET_TALK_WISH));
    EXPECT_EQ(StringCompare(gStringVar2, sKazuki), 0);
    EXPECT(!TargetBranch(STORY_TARGET_KAZUKI, TARGET_TALK_HAPPY));

    // Kazuki hears about the others the same way.
    FlagSet(FLAG_STORY_RESCUED_CONTEST);
    EXPECT(TargetBranch(STORY_TARGET_KAZUKI, TARGET_TALK_HAPPY));
    EXPECT_NE(StringCompare(gStringVar2, sKazuki), 0);
    // Two gone: "several".
    EXPECT(TargetBranch(STORY_TARGET_REIKA, TARGET_TALK_HAPPY));
    EXPECT_NE(StringCompare(gStringVar2, sKazuki), 0);
}

TEST("Frontier story: people leave their lobby once the network lets them go")
{
    ResetStory();
    for (u32 target = STORY_TARGET_KAZUKI; target < STORY_TARGET_COUNT; target++)
    {
        gSpecialVar_0x8004 = target;
        FrontierStory_IsTargetHere();
        EXPECT(gSpecialVar_Result);
    }
    FlagSet(FLAG_STORY_RESCUED_DOME);
    gSpecialVar_0x8004 = STORY_TARGET_REIKA;
    FrontierStory_IsTargetHere();
    EXPECT(!gSpecialVar_Result);
    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    gSpecialVar_0x8004 = STORY_TARGET_IKUE;
    FrontierStory_IsTargetHere();
    EXPECT(!gSpecialVar_Result);
}

static bool32 ContestSceneDue(void)
{
    FrontierStory_CheckContestScene();
    return gSpecialVar_Result;
}

// Gives the first party Pokémon a Master Rank win in the first `count` categories.
static void WinMasterRanks(u32 count)
{
    u8 rank = 4;

    for (u32 i = 0; i < count; i++)
        SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_COOL_RIBBON + i, &rank);
}

TEST("Frontier story: the contest's scenes follow the second to fifth Master Rank categories")
{
    ResetStory();
    ZeroPlayerPartyMons();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_SEVIPER, 50, 0, OTID_STRUCT_PLAYER_ID);
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    WinMasterRanks(1);
    while (TalkToTarget(STORY_TARGET_YORIKO))
        ;
    EXPECT_EQ(TargetState(STORY_TARGET_YORIKO), TARGET_STORY_WISH);
    EXPECT(!ContestSceneDue());

    for (u32 scene = 0; scene < CONTEST_STORY_SCENE_COUNT; scene++)
    {
        WinMasterRanks(scene + 2);
        EXPECT(ContestSceneDue());
        VarSet(VAR_CONTEST_STORY_SCENES, scene + 1);
        EXPECT(!ContestSceneDue());
    }

    // Each waits for Darkrai, and none comes once Yoriko is back.
    VarSet(VAR_CONTEST_STORY_SCENES, 0);
    FlagClear(FLAG_STORY_DARKRAI_JOINED);
    EXPECT(!ContestSceneDue());
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    EXPECT(ContestSceneDue());
    VarSet(VAR_STORY_TARGET_STATES_2, TARGET_STORY_RETURNED << 8);
    EXPECT(!ContestSceneDue());
    ZeroPlayerPartyMons();
}

// Tells lines apart without keeping copies of them; IWRAM has no room for those.
static u32 HashText(const u8 *text)
{
    u32 hash = 2166136261u;

    for (; *text != EOS; text++)
        hash = (hash ^ *text) * 16777619u;
    return hash;
}

TEST("Frontier story: the contest's scenes change with every count of rescues")
{
    static const u8 sKazuki[] = _("カズキ");
    u32 previous[2] = {0};

    ResetStory();
    for (u32 scene = 0; scene < CONTEST_STORY_SCENE_COUNT; scene++)
    {
        VarSet(VAR_CONTEST_STORY_SCENES, scene);
        for (u32 rescued = 0; rescued <= STORY_TARGET_COUNT - 1; rescued++)
        {
            for (u32 i = 0; i < STORY_TARGET_COUNT; i++)
            {
                if (i < rescued) FlagSet(FLAG_STORY_RESCUED_TOWER + i); else FlagClear(FLAG_STORY_RESCUED_TOWER + i);
            }
            for (u32 speaker = CONTEST_SCENE_YORIKO; speaker <= CONTEST_SCENE_DARKRAI; speaker++)
            {
                gSpecialVar_0x8004 = speaker;
                FrontierStory_BufferContestSceneBranch();
                EXPECT_NE(gStringVar4[0], EOS);
                // A new line for none to four rescued; past that, the four-or-more one stays.
                if (rescued > 0)
                    EXPECT_EQ(HashText(gStringVar4) == previous[speaker], rescued >= STORY_RESCUE_STAGE_COUNT);
                previous[speaker] = HashText(gStringVar4);
            }
            if (rescued == 1)
                EXPECT_EQ(StringCompare(gStringVar2, sKazuki), 0);
        }
    }
    VarSet(VAR_CONTEST_STORY_SCENES, CONTEST_STORY_SCENE_COUNT);
    gSpecialVar_0x8004 = CONTEST_SCENE_DARKRAI;
    FrontierStory_BufferContestSceneBranch();
    EXPECT_EQ(gStringVar4[0], EOS);
}

static void SetTargetStep(u32 target, u32 state)
{
    u16 var = target <= STORY_TARGET_TOSHIO ? VAR_STORY_TARGET_STATES : VAR_STORY_TARGET_STATES_2;
    u32 shift = (target - STORY_TARGET_REIKA) % 4 * 4;

    VarSet(var, (VarGet(var) & ~(0xF << shift)) | (state << shift));
}

static u32 TargetFever(u32 target)
{
    gSpecialVar_0x8004 = target;
    FrontierStory_CheckTargetFever();
    return gSpecialVar_Result;
}

static u32 ReturnedTarget(void)
{
    FrontierStory_FindReturnedTarget();
    return gSpecialVar_Result;
}

TEST("Frontier story: Eisuke's fever comes at the Gold Symbol, alone until Darkrai joins")
{
    ResetStory();
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_ARENA].silverSymbolFlag);
    while (TalkToTarget(STORY_TARGET_EISUKE))
        ;
    EXPECT_EQ(TargetState(STORY_TARGET_EISUKE), TARGET_STORY_WISH);
    EXPECT_EQ(TargetFever(STORY_TARGET_EISUKE), TARGET_FEVER_NONE);
    EXPECT(!TargetComesOver(STORY_TARGET_EISUKE));

    // He comes over once for each kind of fever.
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_ARENA].goldSymbolFlag);
    EXPECT_EQ(TargetFever(STORY_TARGET_EISUKE), TARGET_FEVER_ALONE);
    EXPECT(TargetComesOver(STORY_TARGET_EISUKE));
    EXPECT(!TargetComesOver(STORY_TARGET_EISUKE));
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    EXPECT_EQ(TargetFever(STORY_TARGET_EISUKE), TARGET_FEVER_RELEASE);
    EXPECT(TargetComesOver(STORY_TARGET_EISUKE));
    EXPECT(!TargetComesOver(STORY_TARGET_EISUKE));

    // Kazuki's fever is scripted on its own.
    FlagSet(FLAG_SYS_TOWER_GOLD);
    VarSet(VAR_TOWER_STORY_STATE, TOWER_STORY_WISH);
    EXPECT_EQ(TargetFever(STORY_TARGET_KAZUKI), TARGET_FEVER_NONE);
    EXPECT(!TargetComesOver(STORY_TARGET_KAZUKI));
}

TEST("Frontier story: met with the Gold Symbol already won, the talk goes on to the fever")
{
    ResetStory();
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_ARENA].silverSymbolFlag);
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_ARENA].goldSymbolFlag);
    EXPECT(TargetComesOver(STORY_TARGET_EISUKE));
    EXPECT(TalkToTarget(STORY_TARGET_EISUKE));
    EXPECT(TalkToTarget(STORY_TARGET_EISUKE));
    EXPECT_EQ(TargetFever(STORY_TARGET_EISUKE), TARGET_FEVER_RELEASE);
    // That visit already showed the fever.
    EXPECT(!TargetComesOver(STORY_TARGET_EISUKE));
}

TEST("Frontier story: Yoriko's fever follows the last contest scene")
{
    ResetStory();
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    SetTargetStep(STORY_TARGET_YORIKO, TARGET_STORY_WISH);
    VarSet(VAR_CONTEST_STORY_SCENES, CONTEST_STORY_SCENE_COUNT - 1);
    EXPECT_EQ(TargetFever(STORY_TARGET_YORIKO), TARGET_FEVER_NONE);
    VarSet(VAR_CONTEST_STORY_SCENES, CONTEST_STORY_SCENE_COUNT);
    EXPECT_EQ(TargetFever(STORY_TARGET_YORIKO), TARGET_FEVER_RELEASE);
    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    EXPECT_EQ(TargetFever(STORY_TARGET_YORIKO), TARGET_FEVER_NONE);
}

TEST("Frontier story: the release battle and cure take the person from their lobby to the inn")
{
    static const u8 sEisuke[] = _("エイスケ");

    ResetStory();
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_ARENA].goldSymbolFlag);
    SetTargetStep(STORY_TARGET_EISUKE, TARGET_STORY_WISH);
    gSpecialVar_0x8004 = STORY_TARGET_EISUKE;
    FrontierStory_StartRescueScene();
    EXPECT_EQ(StringCompare(gStringVar1, sEisuke), 0);
    FrontierStory_GetRescueScene();
    EXPECT_EQ(gSpecialVar_Result, STORY_TARGET_EISUKE);
    EXPECT_NE(gSpecialVar_0x8004, gSpecialVar_0x8005);
    EXPECT_NE(gSpecialVar_0x8005, gSpecialVar_0x8006);
    EXPECT_NE(gSpecialVar_0x8004, gSpecialVar_0x8006);
    for (u32 part = 0; part < RESCUE_TEXT_COUNT; part++)
    {
        gSpecialVar_0x8004 = part;
        FrontierStory_BufferRescueText();
        EXPECT_EQ(gSpecialVar_Result, part != RESCUE_TEXT_AFTER_CURE);
        EXPECT_EQ(gStringVar4[0] != EOS, part != RESCUE_TEXT_AFTER_CURE);
    }

    // Won: on his way to the hub and gone from the lobby.
    EXPECT_EQ(ReturnedTarget(), STORY_TARGET_COUNT);
    FrontierStory_ReturnRescueTarget();
    EXPECT_EQ(TargetState(STORY_TARGET_EISUKE), TARGET_STORY_RETURNED);
    EXPECT_EQ(TargetFever(STORY_TARGET_EISUKE), TARGET_FEVER_NONE);
    gSpecialVar_0x8004 = STORY_TARGET_EISUKE;
    FrontierStory_IsTargetHere();
    EXPECT(!gSpecialVar_Result);
    EXPECT_EQ(ReturnedTarget(), STORY_TARGET_EISUKE);

    // The hub shows him, he joins the Pokénav, and he is rescued.
    VarSet(VAR_HUB_RESCUED_GFX, 0);
    FrontierStory_PrepareHubRescue();
    EXPECT_EQ(VarGet(VAR_HUB_RESCUED_GFX), OBJ_EVENT_GFX_RUNNING_TRIATHLETE_M);
    FrontierStory_GetRescueMatchCall();
    EXPECT_EQ(gSpecialVar_0x8004, TRAINER_ISAIAH_1);
    EXPECT_EQ(gSpecialVar_0x8000, TRAINER_ISAIAH_1);
    FrontierStory_FinishRescue();
    EXPECT_EQ(TargetState(STORY_TARGET_EISUKE), TARGET_STORY_RESCUED);
    EXPECT(FlagGet(FLAG_STORY_RESCUED_ARENA));
    EXPECT_EQ(ReturnedTarget(), STORY_TARGET_COUNT);
    for (u32 other = STORY_TARGET_KAZUKI; other < STORY_TARGET_COUNT; other++)
    {
        if (other != STORY_TARGET_EISUKE)
            EXPECT_EQ(TargetState(other), TARGET_STORY_NOT_MET);
    }

    // Kazuki's scenes are his own.
    gSpecialVar_0x8004 = STORY_TARGET_KAZUKI;
    FrontierStory_StartRescueScene();
    FrontierStory_GetRescueScene();
    EXPECT_EQ(gSpecialVar_Result, STORY_TARGET_EISUKE);

    // Yoriko has more to say once cured, and no fever without Darkrai.
    gSpecialVar_0x8004 = STORY_TARGET_YORIKO;
    FrontierStory_StartRescueScene();
    gSpecialVar_0x8004 = RESCUE_TEXT_AFTER_CURE;
    FrontierStory_BufferRescueText();
    EXPECT(gSpecialVar_Result);
    gSpecialVar_0x8004 = RESCUE_TEXT_FEVER_STARTING;
    FrontierStory_BufferRescueText();
    EXPECT(!gSpecialVar_Result);
}

static u32 PickInnGuests(u32 area)
{
    u32 shown = 0;

    gSpecialVar_0x8004 = area;
    FrontierStory_PickInnGuests();
    for (u32 slot = 0; slot < INN_GUEST_SLOTS; slot++)
    {
        if (!FlagGet(FLAG_INN_GUEST_HIDDEN + slot))
            shown++;
    }
    return shown;
}

TEST("Frontier story: the inn shows a few of the cured Brains and guests where they rest")
{
    ResetStory();
    EXPECT_EQ(PickInnGuests(INN_AREA_LOBBY), 0);
    EXPECT_EQ(VarGet(VAR_INN_GUEST), NAMED_STORY_NONE);

    // 0, 3 and 6 rest in the lobby and 1 in the east wing; 6 is not cured yet.
    SetNamed(0, NAMED_STORY_RESCUED);
    SetNamed(3, NAMED_STORY_RESCUED);
    SetNamed(6, NAMED_STORY_RECOVERED);
    SetNamed(1, NAMED_STORY_RESCUED);
    for (u32 round = 0; round < 8; round++)
    {
        u32 first, second;

        EXPECT_EQ(PickInnGuests(INN_AREA_LOBBY), 2);
        first = VarGet(VAR_INN_GUEST);
        second = VarGet(VAR_INN_GUEST + 1);
        EXPECT(first == 0 || first == 3);
        EXPECT(second == 0 || second == 3);
        EXPECT_NE(first, second);
        EXPECT_NE(VarGet(VAR_INN_GUEST_GFX), 0);
    }
    EXPECT_EQ(PickInnGuests(INN_AREA_EAST_WING), 1);
    EXPECT_EQ(VarGet(VAR_INN_GUEST), 1);
    EXPECT_EQ(VarGet(VAR_INN_GUEST + 1), NAMED_STORY_NONE);
    gSpecialVar_0x8004 = 0;
    FrontierStory_BufferInnGuestText();
    EXPECT_NE(gStringVar4[0], EOS);
    gSpecialVar_0x8004 = 1;
    FrontierStory_BufferInnGuestText();
    EXPECT_EQ(gStringVar4[0], EOS);
    EXPECT_EQ(PickInnGuests(INN_AREA_WEST_WING), 0);

    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    EXPECT_EQ(PickInnGuests(INN_AREA_LOBBY), 0);
}

TEST("Frontier story: every Brain and guest has words for the inn")
{
    ResetStory();
    for (u32 id = 0; id < NAMED_STORY_COUNT; id++)
    {
        SetNamed(id, NAMED_STORY_RESCUED);
        VarSet(VAR_INN_GUEST, id);
        gSpecialVar_0x8004 = 0;
        FrontierStory_BufferInnGuestText();
        EXPECT_NE(gStringVar4[0], EOS);
    }
}

TEST("Frontier story: everyone with a release battle has all of its lines and places")
{
    u32 withScenes = 0;

    for (u32 target = STORY_TARGET_REIKA; target < STORY_TARGET_COUNT; target++)
    {
        ResetStory();
        gSpecialVar_0x8004 = target;
        FrontierStory_StartRescueScene();
        gSpecialVar_0x8004 = RESCUE_TEXT_FEVER;
        FrontierStory_BufferRescueText();
        if (!gSpecialVar_Result)
            continue;
        withScenes++;
        for (u32 part = RESCUE_TEXT_FEVER; part < RESCUE_TEXT_COUNT; part++)
        {
            if (part == RESCUE_TEXT_AFTER_CURE)
                continue;
            gSpecialVar_0x8004 = part;
            FrontierStory_BufferRescueText();
            EXPECT(gSpecialVar_Result);
        }
        // Only the contest's fever comes when Darkrai is always there already.
        gSpecialVar_0x8004 = RESCUE_TEXT_FEVER_STARTING;
        FrontierStory_BufferRescueText();
        EXPECT_EQ(gSpecialVar_Result, target != STORY_TARGET_YORIKO);

        FrontierStory_GetRescueScene();
        EXPECT_EQ(gSpecialVar_Result, target);
        EXPECT_NE(gSpecialVar_0x8005, 0);
        EXPECT_NE(gSpecialVar_0x8006, 0);
        EXPECT_NE(gSpecialVar_0x8004, gSpecialVar_0x8005);
        EXPECT_NE(gSpecialVar_0x8005, gSpecialVar_0x8006);
        EXPECT_NE(gSpecialVar_0x8004, gSpecialVar_0x8006);
        FrontierStory_GetRescueMatchCall();
        EXPECT_NE(gSpecialVar_0x8004, 0);
        FrontierStory_ReturnRescueTarget();
        VarSet(VAR_HUB_RESCUED_GFX, 0);
        FrontierStory_PrepareHubRescue();
        EXPECT_NE(VarGet(VAR_HUB_RESCUED_GFX), 0);
    }
    EXPECT_EQ(withScenes, STORY_TARGET_COUNT - 1);
}

TEST("Frontier story: Nobuo reaches his release battle and cure without moving saved flags")
{
    static const u8 name[] = _("ノブオ");
    ResetStory();
    EXPECT(!TalkToTarget(STORY_TARGET_NOBUO));
    FlagSet(FLAG_SYS_PYRAMID_SILVER);
    EXPECT(!TalkToTarget(STORY_TARGET_NOBUO));
    EXPECT_EQ(TargetState(STORY_TARGET_NOBUO), TARGET_STORY_WISH);
    EXPECT_EQ(TargetFever(STORY_TARGET_NOBUO), TARGET_FEVER_NONE);
    FlagSet(FLAG_SYS_PYRAMID_GOLD);
    EXPECT_EQ(TargetFever(STORY_TARGET_NOBUO), TARGET_FEVER_ALONE);
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    EXPECT_EQ(TargetFever(STORY_TARGET_NOBUO), TARGET_FEVER_RELEASE);
    gSpecialVar_0x8004 = STORY_TARGET_NOBUO;
    FrontierStory_StartRescueScene();
    FrontierStory_GetRescueScene();
    EXPECT_EQ(StringCompare(gStringVar1, name), 0);
    EXPECT_EQ(gSpecialVar_0x8004, LOCALID_PYRAMID_LOBBY_NOBUO);
    EXPECT_EQ(gSpecialVar_0x8005, LOCALID_PYRAMID_LOBBY_COOK);
    EXPECT_EQ(gSpecialVar_0x8006, LOCALID_PYRAMID_LOBBY_POKEVIRUS);
    for (u32 part = 0; part < RESCUE_TEXT_COUNT; part++)
    {
        gSpecialVar_0x8004 = part;
        FrontierStory_BufferRescueText();
        EXPECT(gSpecialVar_Result);
        EXPECT_NE(gStringVar4[0], EOS);
    }
    FrontierStory_ReturnRescueTarget();
    EXPECT_EQ(ReturnedTarget(), STORY_TARGET_NOBUO);
    FrontierStory_PrepareHubRescue();
    EXPECT_EQ(VarGet(VAR_HUB_RESCUED_GFX), OBJ_EVENT_GFX_HIKER);
    FrontierStory_GetRescueMatchCall();
    EXPECT_EQ(gSpecialVar_0x8004, TRAINER_DUSTY_1);
    EXPECT_EQ(gSpecialVar_0x8000, TRAINER_DUSTY_1);
    FrontierStory_FinishRescue();
    EXPECT(FlagGet(FLAG_STORY_RESCUED_PYRAMID));
    EXPECT_EQ(TargetState(STORY_TARGET_NOBUO), TARGET_STORY_RESCUED);
    EXPECT_EQ(TargetFever(STORY_TARGET_NOBUO), TARGET_FEVER_NONE);
    EXPECT_EQ(ReturnedTarget(), STORY_TARGET_COUNT);
    EXPECT_EQ(SYSTEM_FLAGS, 0x860);
    EXPECT_EQ(MAX_TRAINERS_COUNT_EMERALD, 864);
}

TEST("Frontier story: Nobuo and Pokevirus each occupy half an enemy party")
{
    const struct Trainer *trainer = &sStoryTestTrainers[DIFFICULTY_NORMAL][TRAINER_NOBUO_RELEASE];
    EXPECT_EQ((u32)trainer->partySize, 3);
    EXPECT_EQ((u32)trainer->multiTeamSize, MULTI_TEAM_SIZE_HALF);
    EXPECT_EQ(trainer->trainerClass, TRAINER_CLASS_RUIN_MANIAC);
    EXPECT_EQ(trainer->party[0].species, SPECIES_SANDSLASH);
    EXPECT_EQ(trainer->party[0].heldItem, ITEM_GROUNDIUM_Z);
    for (u32 i = 0; i < 3; i++)
    {
        EXPECT_EQ(trainer->party[i].lvl, 50);
        EXPECT_NE(trainer->party[i].species, SPECIES_NONE);
        for (u32 move = 0; move < MAX_MON_MOVES; move++)
            EXPECT_NE(trainer->party[i].moves[move], MOVE_NONE);
    }
    trainer = &sStoryTestTrainers[DIFFICULTY_NORMAL][TRAINER_POKEVIRUS];
    EXPECT_EQ((u32)trainer->partySize, 3);
    EXPECT_EQ((u32)trainer->multiTeamSize, MULTI_TEAM_SIZE_HALF);
}

TEST("Frontier story: in the last battle Pokevirus alone sends out all six of its own")
{
    static const u16 sFinalSpecies[] =
    {
        SPECIES_UXIE, SPECIES_MESPRIT, SPECIES_AZELF,
        SPECIES_NECROZMA_DUSK_MANE, SPECIES_ETERNATUS_ETERNAMAX, SPECIES_PECHARUNT,
    };
    const struct Trainer *trainer = &sStoryTestTrainers[DIFFICULTY_NORMAL][TRAINER_POKEVIRUS_FINAL];

    EXPECT_EQ((u32)trainer->partySize, ARRAY_COUNT(sFinalSpecies));
    EXPECT_NE((u32)trainer->multiTeamSize, MULTI_TEAM_SIZE_HALF);
    EXPECT_EQ(trainer->trainerPic, sStoryTestTrainers[DIFFICULTY_NORMAL][TRAINER_POKEVIRUS].trainerPic);
    // Necrozma Ultra Bursts in the battle: begun in that form, it would have nothing to change back to.
    EXPECT_EQ(trainer->party[3].heldItem, ITEM_ULTRANECROZIUM_Z);
    for (u32 i = 0; i < ARRAY_COUNT(sFinalSpecies); i++)
    {
        EXPECT_EQ(trainer->party[i].species, sFinalSpecies[i]);
        EXPECT_EQ(trainer->party[i].lvl, 50);
        for (u32 move = 0; move < MAX_MON_MOVES; move++)
            EXPECT_NE(trainer->party[i].moves[move], MOVE_NONE);
    }
}

TEST("Frontier story: all eight rescue targets can finish without losing another target's state")
{
    ResetStory();
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    FlagSet(FLAG_STORY_CURE_FOUND);
    VarSet(VAR_TOWER_STORY_STATE, TOWER_STORY_RESCUED);
    FlagSet(FLAG_STORY_RESCUED_TOWER);
    for (u32 target = STORY_TARGET_REIKA; target < STORY_TARGET_COUNT; target++)
    {
        SetTargetStep(target, TARGET_STORY_WISH);
        gSpecialVar_0x8004 = target;
        FrontierStory_StartRescueScene();
        FrontierStory_ReturnRescueTarget();
        EXPECT_EQ(ReturnedTarget(), target);
        FrontierStory_PrepareHubRescue();
        FrontierStory_FinishRescue();
        EXPECT_EQ(ReturnedTarget(), STORY_TARGET_COUNT);
        for (u32 finished = STORY_TARGET_KAZUKI; finished <= target; finished++)
            EXPECT_EQ(TargetState(finished), TARGET_STORY_RESCUED);
    }
    FrontierStory_CountRescued();
    EXPECT_EQ(gSpecialVar_Result, STORY_TARGET_COUNT);
}
