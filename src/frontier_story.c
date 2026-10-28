#include "global.h"
#include "frontier_practice.h"
#include "battle.h"
#include "battle_scripts.h"
#include "battle_setup.h"
#include "battle_stat_change.h"
#include "battle_util.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "frontier_guest.h"
#include "frontier_story.h"
#include "frontier_util.h"
#include "malloc.h"
#include "overworld.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "script_movement.h"
#include "string_util.h"
#include "constants/battle_frontier.h"
#include "constants/characters.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/frontier_util.h"
#include "constants/vars.h"

#include "data/frontier_story_named.h"
#include "data/frontier_story_targets.h"
#include "data/frontier_story_contest.h"
#include "data/frontier_crowd_talk.h"
#include "data/frontier_regulars.h"

// The Frontier never asks for less than level 50, so story battles don't either.
#define STORY_BATTLE_MIN_LEVEL 50
#define RANK_RIBBON_HYPER 3
#define RANK_RIBBON_MASTER 4
// How far someone walks over to the player before a story talk; the start cell of that path.
#define APPROACH_MAX_STEPS 40
#define PATH_START 0xFF

STATIC_ASSERT(NAMED_STORY_GUEST_COUNT == FRONTIER_GUEST_RANDOM_COUNT, NamedStoryGuestsMatchGuestTable);
STATIC_ASSERT(NAMED_STORY_COUNT == NAMED_STORY_GUEST_COUNT + NUM_FRONTIER_FACILITIES, NamedStoryBrainsMatchFacilities);
STATIC_ASSERT(FLAG_STORY_NAMED_STATES_START + NAMED_STORY_COUNT * 2 - 1 == FLAG_STORY_NAMED_STATES_END, NamedStoryFlagsFit);

// Only for the battle being set up or fought; nothing here is saved.
struct StoryBattle
{
    u8 level;
    bool8 releaseBattle;
    u8 faintReplacements; // battler bits
};

EWRAM_DATA static struct StoryBattle sStoryBattle = {0};

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

// Where each person stands in their lobby.
static const u8 sStoryTargetLocalIds[STORY_TARGET_COUNT] =
{
    [STORY_TARGET_KAZUKI] = LOCALID_TOWER_LOBBY_KAZUKI,
    [STORY_TARGET_REIKA]  = LOCALID_DOME_LOBBY_REIKA,
    [STORY_TARGET_IKUE]   = LOCALID_PALACE_LOBBY_IKUE,
    [STORY_TARGET_EISUKE] = LOCALID_ARENA_LOBBY_EISUKE,
    [STORY_TARGET_TOSHIO] = LOCALID_FACTORY_LOBBY_TOSHIO,
    [STORY_TARGET_YAYOI]  = LOCALID_PIKE_LOBBY_YAYOI,
    [STORY_TARGET_NOBUO]  = LOCALID_PYRAMID_LOBBY_NOBUO,
    [STORY_TARGET_YORIKO] = LOCALID_CONTEST_LOBBY_YORIKO,
};

static bool32 IsStoryActive(void)
{
    // Saves from before the opening keep the old Frontier as it was.
    return VarGet(VAR_FRONTIER_INTRO_STATE) == FRONTIER_INTRO_COMPLETE;
}

static u32 GetNamedState(u32 id)
{
    u32 flag = FLAG_STORY_NAMED_STATES_START + id * 2;

    return FlagGet(flag) | (FlagGet(flag + 1) << 1);
}

static void SetNamedState(u32 id, u32 state)
{
    u32 flag = FLAG_STORY_NAMED_STATES_START + id * 2;

    if (state & 1)
        FlagSet(flag);
    else
        FlagClear(flag);
    if (state & 2)
        FlagSet(flag + 1);
    else
        FlagClear(flag + 1);
}

static u32 GetGuestNamedId(u16 trainerId)
{
    if (trainerId == TRAINER_FRONTIER_TATE || trainerId == TRAINER_FRONTIER_LIZA)
        trainerId = TRAINER_FRONTIER_TATE_AND_LIZA;
    if (trainerId < TRAINER_FRONTIER_GUEST_FIRST || trainerId >= TRAINER_FRONTIER_GUEST_FIRST + NAMED_STORY_GUEST_COUNT)
        return NAMED_STORY_NONE;
    return trainerId - TRAINER_FRONTIER_GUEST_FIRST;
}

static const u8 *GetNamedName(u32 id)
{
    if (id < NAMED_STORY_GUEST_COUNT)
        return GetFrontierGuestTrainer(TRAINER_FRONTIER_GUEST_FIRST + id)->name;
    return sNamedStoryTexts[id].name;
}

static u16 GetNamedObjectGfx(u32 id)
{
    if (id < NAMED_STORY_GUEST_COUNT)
        return GetFrontierGuestTrainer(TRAINER_FRONTIER_GUEST_FIRST + id)->objectGfx;
    return gFrontierBrainInfo[id - NAMED_STORY_BRAIN_FIRST].objEventGfx;
}

static u8 GetPlayerPartyMaxLevel(void)
{
    u32 i, level, maxLevel = STORY_BATTLE_MIN_LEVEL;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (!GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SANITY_HAS_SPECIES) || GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_IS_EGG))
            continue;
        level = GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL);
        if (level > maxLevel)
            maxLevel = level;
    }
    return maxLevel;
}

// Call after the player's three Pokémon are chosen and set up with setmultitrainerbattle.
void FrontierStory_PrepareReleaseBattle(void)
{
    sStoryBattle.level = GetPlayerPartyMaxLevel();
    sStoryBattle.releaseBattle = TRUE;
    sStoryBattle.faintReplacements = 0;
    SetPokevirusReleaseTransition();
}

// The last battle, against Pokevirus alone: like a release battle, with the final music.
void FrontierStory_PrepareFinalBattle(void)
{
    FrontierStory_PrepareReleaseBattle();
    SetPokevirusFinalBattle();
}

void FrontierStory_PrepareMatchedBattle(void)
{
    sStoryBattle.level = GetPlayerPartyMaxLevel();
    sStoryBattle.releaseBattle = FALSE;
    sStoryBattle.faintReplacements = 0;
}

void FrontierStory_EndStoryBattle(void)
{
    gSpecialVar_Result = gBattleOutcome;
    memset(&sStoryBattle, 0, sizeof(sStoryBattle));
}

u8 FrontierStory_GetTrainerMonLevel(u8 level)
{
    return sStoryBattle.level != 0 ? sStoryBattle.level : level;
}

bool32 FrontierStory_IsReleaseBattle(void)
{
    return sStoryBattle.releaseBattle
        && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED | BATTLE_TYPE_RECORDED_LINK));
}

void FrontierStory_RecordSendOut(u32 battler, bool32 replacingFaintedMon)
{
    if (replacingFaintedMon)
        sStoryBattle.faintReplacements |= 1u << battler;
    else
        sStoryBattle.faintReplacements &= ~(1u << battler);
}

// In a release battle, each of the other side's Pokémon gets one random stat +2 when it is
// sent out first or replaces a fainted one. A switch the trainer chose gives nothing.
bool32 FrontierStory_TryReleaseBoost(u32 battler)
{
    static const u8 sBoostStats[] = {STAT_ATK, STAT_DEF, STAT_SPEED, STAT_SPATK, STAT_SPDEF, STAT_EVASION};
    bool32 replacedFaintedMon;

    if (!gBattleStruct->battlerState[battler].switchIn)
        return FALSE;
    replacedFaintedMon = (sStoryBattle.faintReplacements >> battler) & 1;
    sStoryBattle.faintReplacements &= ~(1u << battler);
    if (!FrontierStory_IsReleaseBattle() || IsOnPlayerSide(battler) || !IsBattlerAlive(battler))
        return FALSE;
    if (!replacedFaintedMon && gBattleStruct->eventState.beforeFirstTurn == 0)
        return FALSE;

    gEffectBattler = battler;
    SetStatChange(battler, sBoostStats[RandomUniform(RNG_FRONTIER_RELEASE_BOOST, 0, ARRAY_COUNT(sBoostStats) - 1)], 2);
    BattleScriptCall(BattleScript_FrontierReleaseBoost);
    return TRUE;
}

static void AdvanceBrainMemory(u32 facility)
{
    u32 id = NAMED_STORY_BRAIN_FIRST + facility;

    if (facility >= NUM_FRONTIER_FACILITIES || GetNamedState(id) >= NAMED_STORY_RECOVERED)
        return;
    // Losing the Gold Symbol battle, or any rematch after it, brings the memory back.
    if (GetPlayerSymbolCountForFacility(facility) != 0)
        SetNamedState(id, NAMED_STORY_RECOVERED);
    else
        SetNamedState(id, NAMED_STORY_UNEASY);
}

static void AdvanceGuestMemory(u32 id)
{
    u32 state = GetNamedState(id);

    if (state < NAMED_STORY_RECOVERED)
        SetNamedState(id, state + 1);
}

// Called once at the end of every battle, after the losers' last words were shown.
void FrontierStory_OnBattleFinished(void)
{
    u32 guestA, guestB;

    if (FrontierPractice_IsBattle())
        return;
    memset(&sStoryBattle, 0, sizeof(sStoryBattle));
    if (gBattleOutcome != B_OUTCOME_WON || !IsStoryActive()
     || !(gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
     || gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED | BATTLE_TYPE_RECORDED_LINK))
        return;

    if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_FRONTIER_BRAIN)
    {
        AdvanceBrainMemory(VarGet(VAR_FRONTIER_FACILITY));
        return;
    }
    guestA = GetGuestNamedId(TRAINER_BATTLE_PARAM.opponentA);
    guestB = NAMED_STORY_NONE;
    if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
        guestB = GetGuestNamedId(TRAINER_BATTLE_PARAM.opponentB);
    if (guestA != NAMED_STORY_NONE)
        AdvanceGuestMemory(guestA);
    // Tate and Liza are one person here.
    if (guestB != NAMED_STORY_NONE && guestB != guestA)
        AdvanceGuestMemory(guestB);
}

bool32 FrontierStory_TryCopyBrainBattleText(u32 facility, bool32 playerWon)
{
    u32 id = NAMED_STORY_BRAIN_FIRST + facility;

    if (!IsStoryActive() || facility >= NUM_FRONTIER_FACILITIES || gBattleTypeFlags & BATTLE_TYPE_RECORDED
     || GetNamedState(id) >= NAMED_STORY_RECOVERED)
        return FALSE;

    if (!playerWon)
        StringCopy(gStringVar4, sText_NamedCommonWords);
    else if (GetPlayerSymbolCountForFacility(facility) == 0)
        StringCopy(gStringVar4, sText_NamedUneasy);
    else
        StringCopy(gStringVar4, sNamedStoryTexts[id].recovered);
    return TRUE;
}

bool32 FrontierStory_TryCopyGuestText(u8 whichText, u16 trainerId)
{
    u32 id = GetGuestNamedId(trainerId);
    u32 state;
    const u8 *text = NULL;

    if (!IsStoryActive() || id == NAMED_STORY_NONE)
        return FALSE;
    if (whichText != FRONTIER_BEFORE_TEXT && gBattleTypeFlags & BATTLE_TYPE_RECORDED)
        return FALSE;

    state = GetNamedState(id);
    switch (whichText)
    {
    case FRONTIER_BEFORE_TEXT:
        if (state == NAMED_STORY_INFECTED)
            text = sText_NamedCommonIntro;
        else if (state == NAMED_STORY_UNEASY)
            text = sText_NamedUneasyIntro;
        else if (state == NAMED_STORY_RESCUED)
            text = sText_NamedRescuedChallenge;
        break;
    case FRONTIER_PLAYER_WON_TEXT:
        if (state == NAMED_STORY_INFECTED)
            text = sText_NamedUneasy;
        else if (state == NAMED_STORY_UNEASY)
            text = sNamedStoryTexts[id].recovered;
        break;
    default:
        if (state < NAMED_STORY_RECOVERED)
            text = sText_NamedCommonWords;
        break;
    }
    if (text == NULL)
        return FALSE;
    StringCopy(gStringVar4, text);
    return TRUE;
}

// Until a Brain remembers who they are, they open every battle with the network's words.
void FrontierStory_BufferBrainIntro(void)
{
    u32 facility = VarGet(VAR_FRONTIER_FACILITY);
    u32 state;

    gSpecialVar_Result = FALSE;
    if (!IsStoryActive() || facility >= NUM_FRONTIER_FACILITIES)
        return;
    state = GetNamedState(NAMED_STORY_BRAIN_FIRST + facility);
    if (state == NAMED_STORY_INFECTED)
        StringCopy(gStringVar4, sText_NamedCommonIntro);
    else if (state == NAMED_STORY_UNEASY)
        StringCopy(gStringVar4, sText_NamedUneasyIntro);
    else
        return;
    gSpecialVar_Result = TRUE;
}

static u32 CountRescued(void)
{
    u32 i, count = 0;

    for (i = 0; i < ARRAY_COUNT(sRescuedFlags); i++)
        count += FlagGet(sRescuedFlags[i]);
    return count;
}

void FrontierStory_CountRescued(void)
{
    gSpecialVar_Result = CountRescued();
}

// The fourth rescue opens the way to the depths: the rescued gather on the inn's beach once.
void FrontierStory_ShouldGather(void)
{
    gSpecialVar_Result = IsStoryActive() && CountRescued() >= 4 && !FlagGet(FLAG_STORY_PATH_OPEN);
}

// Everyone cured so far: the scenario rescues and the Brains and guests cured at the camp.
static u32 CountCuredPeople(void)
{
    u32 i, count = CountRescued();

    for (i = 0; i < NAMED_STORY_COUNT; i++)
        count += GetNamedState(i) == NAMED_STORY_RESCUED;
    return count;
}

// The cured rest at the inn across the sea, so Darkrai suggests it once anyone is cured.
void FrontierStory_ShouldSuggestInn(void)
{
    gSpecialVar_Result = IsStoryActive() && FlagGet(FLAG_STORY_CURE_FOUND)
                      && !FlagGet(FLAG_STORY_INN_OPEN) && CountCuredPeople() != 0;
}

// One bit per contest category this Pokémon has won at the rank or above.
static u32 GetRankRibbonCategories(struct BoxPokemon *boxMon, u32 rank)
{
    u32 field, categories = 0;

    if (!GetBoxMonData(boxMon, MON_DATA_SANITY_HAS_SPECIES))
        return 0;
    for (field = MON_DATA_COOL_RIBBON; field <= MON_DATA_TOUGH_RIBBON; field++)
    {
        if (GetBoxMonData(boxMon, field) >= rank)
            categories |= 1u << (field - MON_DATA_COOL_RIBBON);
    }
    return categories;
}

// The categories won at this rank, by any of the player's Pokémon. Stops looking once it has
// `enough` of them.
static u32 CountWonContestCategories(u32 rank, u32 enough)
{
    u32 i, j, categories = 0;

    for (i = 0; i < PARTY_SIZE && (u32)__builtin_popcount(categories) < enough; i++)
        categories |= GetRankRibbonCategories(&gParties[B_TRAINER_PLAYER][i].box, rank);
    for (i = 0; i < TOTAL_BOXES_COUNT; i++)
    {
        for (j = 0; j < IN_BOX_COUNT && (u32)__builtin_popcount(categories) < enough; j++)
            categories |= GetRankRibbonCategories(GetBoxedMonPtr(i, j), rank);
    }
    return __builtin_popcount(categories);
}

// A contest win at this rank in any category, by any of the player's Pokémon.
static bool32 HasWonContestRank(u32 rank)
{
    return CountWonContestCategories(rank, 1) != 0;
}

// Darkrai joins after the first Silver Symbol or the first Hyper Rank win in any category.
void FrontierStory_CanDarkraiJoin(void)
{
    u32 i;

    gSpecialVar_Result = FALSE;
    if (!IsStoryActive() || FlagGet(FLAG_STORY_DARKRAI_JOINED))
        return;

    for (i = 0; i < NUM_FRONTIER_FACILITIES; i++)
    {
        if (FlagGet(FLAG_SYS_TOWER_SILVER + i * 2))
        {
            gSpecialVar_Result = TRUE;
            return;
        }
    }
    gSpecialVar_Result = HasWonContestRank(RANK_RIBBON_HYPER);
}

// Everyone after Kazuki has four bits: Reika to Toshio in one var, Yayoi to Yoriko in the next.
static u16 GetTargetStateVar(u32 target, u32 *shift)
{
    *shift = (target - STORY_TARGET_REIKA) % 4 * 4;
    return target <= STORY_TARGET_TOSHIO ? VAR_STORY_TARGET_STATES : VAR_STORY_TARGET_STATES_2;
}

static u32 GetTargetState(u32 target)
{
    u32 shift;
    u16 var;

    if (target == STORY_TARGET_KAZUKI)
        return VarGet(VAR_TOWER_STORY_STATE);
    var = GetTargetStateVar(target, &shift);
    return (VarGet(var) >> shift) & 0xF;
}

static void SetTargetState(u32 target, u32 state)
{
    u32 shift;
    u16 var;

    if (target == STORY_TARGET_KAZUKI)
    {
        VarSet(VAR_TOWER_STORY_STATE, state);
        return;
    }
    var = GetTargetStateVar(target, &shift);
    VarSet(var, (VarGet(var) & ~(0xF << shift)) | (state << shift));
}

// Their Silver Symbol step; at the contest, the first Master Rank win in any category.
static bool32 HasReachedWish(u32 target)
{
    if (target == STORY_TARGET_YORIKO)
        return HasWonContestRank(RANK_RIBBON_MASTER);
    return FlagGet(gFrontierBrainInfo[target].silverSymbolFlag);
}

// Their Gold Symbol step; at the contest, the last of its scenes after all five Master Rank wins.
static bool32 HasReachedRescue(u32 target)
{
    if (target == STORY_TARGET_YORIKO)
        return VarGet(VAR_CONTEST_STORY_SCENES) >= CONTEST_STORY_SCENE_COUNT;
    return FlagGet(gFrontierBrainInfo[target].goldSymbolFlag);
}

static bool32 HasLeftNetwork(u32 target)
{
    return FlagGet(sRescuedFlags[target]) || GetTargetState(target) >= TARGET_STORY_RETURNED;
}

// TARGET_FEVER_*: the fever that talking to them reaches now, after any steps they have yet to show.
static u32 GetFeverAhead(u32 target)
{
    if (!IsStoryActive() || target == STORY_TARGET_KAZUKI || target >= STORY_TARGET_COUNT
     || sStoryTargetRescueTexts[target][RESCUE_TEXT_FEVER] == NULL
     || GetTargetState(target) > TARGET_STORY_WISH || !HasReachedRescue(target))
        return TARGET_FEVER_NONE;
    return FlagGet(FLAG_STORY_DARKRAI_JOINED) ? TARGET_FEVER_RELEASE : TARGET_FEVER_ALONE;
}

// TARGET_FEVER_*: whether their fever comes as soon as the player talks to them.
static u32 GetTargetFever(u32 target)
{
    u32 fever = GetFeverAhead(target);

    return fever != TARGET_FEVER_NONE && GetTargetState(target) == TARGET_STORY_WISH ? fever : TARGET_FEVER_NONE;
}

// Each kind of fever brings them over to the player once (VAR_STORY_TARGET_FEVERS).
static bool32 TryMarkFeverShown(u32 target, u32 fever)
{
    u32 bit = 1u << (target * 2 + (fever == TARGET_FEVER_RELEASE));
    u32 shown = VarGet(VAR_STORY_TARGET_FEVERS);

    if (fever == TARGET_FEVER_NONE || (shown & bit))
        return FALSE;
    VarSet(VAR_STORY_TARGET_FEVERS, shown | bit);
    return TRUE;
}

// VAR_0x8004: STORY_TARGET_*. Whether they are in their lobby: until the network lets them go.
void FrontierStory_IsTargetHere(void)
{
    u32 target = gSpecialVar_0x8004;

    gSpecialVar_Result = IsStoryActive() && target < STORY_TARGET_COUNT && !HasLeftNetwork(target);
}

// VAR_0x8004: STORY_TARGET_* other than Kazuki. Whether they come over to the player on entering
// the lobby: the first time, the first time after the Silver Symbol (Master Rank) step, and the
// first time their fever comes, alone and then with Darkrai there for the release battle.
void FrontierStory_CheckTargetApproach(void)
{
    u32 target = gSpecialVar_0x8004, state;
    bool32 feverNew;

    gSpecialVar_Result = FALSE;
    if (!IsStoryActive() || target == STORY_TARGET_KAZUKI || target >= STORY_TARGET_COUNT)
        return;
    state = GetTargetState(target);
    // The talk goes on to the fever after any earlier steps, so that counts as shown too.
    feverNew = TryMarkFeverShown(target, GetFeverAhead(target));
    gSpecialVar_Result = feverNew || state == TARGET_STORY_NOT_MET
                      || (state == TARGET_STORY_MET && HasReachedWish(target));
}

// VAR_0x8004: STORY_TARGET_*. VAR_RESULT: TARGET_FEVER_*, whether talking to them now brings on
// their fever.
void FrontierStory_CheckTargetFever(void)
{
    gSpecialVar_Result = GetTargetFever(gSpecialVar_0x8004);
}

// VAR_0x8004: STORY_TARGET_* other than Kazuki. Puts what they say now in gStringVar4 and moves
// their story on: the first meeting, then the change after the Silver Symbol (Master Rank) step.
// VAR_0x8005 becomes the TARGET_TALK_* for FrontierStory_BufferTargetBranch; VAR_RESULT is TRUE
// if they go straight on to their next step (the player already reached it when they met), up to
// their fever.
void FrontierStory_BufferTargetTalk(void)
{
    u32 target = gSpecialVar_0x8004;
    const struct StoryTargetText *texts;
    const u8 *text;

    gSpecialVar_Result = FALSE;
    gSpecialVar_0x8005 = TARGET_TALK_HAPPY;
    gStringVar4[0] = EOS;
    if (target == STORY_TARGET_KAZUKI || target >= STORY_TARGET_COUNT)
        return;
    texts = &sStoryTargetTexts[target];
    switch (GetTargetState(target))
    {
    case TARGET_STORY_NOT_MET:
        text = texts->meet;
        SetTargetState(target, TARGET_STORY_MET);
        gSpecialVar_Result = HasReachedWish(target);
        break;
    case TARGET_STORY_MET:
        text = texts->happy;
        if (HasReachedWish(target))
        {
            text = texts->wish;
            SetTargetState(target, TARGET_STORY_WISH);
            gSpecialVar_0x8005 = TARGET_TALK_WISH;
            gSpecialVar_Result = GetTargetFever(target) != TARGET_FEVER_NONE;
        }
        break;
    default:
        text = texts->wishAgain;
        gSpecialVar_0x8005 = TARGET_TALK_WISH;
        break;
    }
    StringExpandPlaceholders(gStringVar4, text);
}

// VAR_0x8004: STORY_TARGET_*, VAR_0x8005: TARGET_TALK_*. What they feel about the others in the
// network: someone who has left it, or, once they waver themselves, someone else wavering too.
// The line goes in gStringVar4; VAR_RESULT is FALSE if there is none.
void FrontierStory_BufferTargetBranch(void)
{
    u32 target = gSpecialVar_0x8004, i, gone = 0, wavering = 0, goneTarget = 0;
    const struct StoryTargetText *texts;
    const u8 *text;

    gSpecialVar_Result = FALSE;
    if (!IsStoryActive() || target >= STORY_TARGET_COUNT)
        return;
    for (i = 0; i < STORY_TARGET_COUNT; i++)
    {
        if (i == target)
            continue;
        if (HasLeftNetwork(i))
        {
            gone++;
            goneTarget = i;
        }
        else if (GetTargetState(i) == TARGET_STORY_WISH)
        {
            wavering++;
        }
    }
    texts = &sStoryTargetTexts[target];
    if (gSpecialVar_0x8005 == TARGET_TALK_WISH)
        text = gone != 0 ? texts->wishGone : wavering != 0 ? texts->wishShared : NULL;
    else
        text = gone != 0 ? texts->othersGone : NULL;
    if (text == NULL)
        return;
    StringCopy(gStringVar2, gone == 1 ? sStoryTargetTexts[goneTarget].name : sText_SeveralPeople);
    StringExpandPlaceholders(gStringVar4, text);
    gSpecialVar_Result = TRUE;
}

// For each person's release battle and cure: the cook and Pokevirus in their lobby, how they look
// in the hub, and their Pokénav entry.
struct StoryTargetRescue
{
    u8 cookLocalId;
    u8 pokevirusLocalId;
    u16 graphicsId;
    u16 matchCallTrainer;
};

static const struct StoryTargetRescue sStoryTargetRescues[STORY_TARGET_COUNT] =
{
    [STORY_TARGET_REIKA]  = {LOCALID_DOME_LOBBY_COOK, LOCALID_DOME_LOBBY_POKEVIRUS, OBJ_EVENT_GFX_WOMAN_2, TRAINER_CINDY_1},
    [STORY_TARGET_IKUE]   = {LOCALID_PALACE_LOBBY_COOK, LOCALID_PALACE_LOBBY_POKEVIRUS, OBJ_EVENT_GFX_WOMAN_2, TRAINER_LYDIA_1},
    [STORY_TARGET_TOSHIO] = {LOCALID_FACTORY_LOBBY_COOK, LOCALID_FACTORY_LOBBY_POKEVIRUS, OBJ_EVENT_GFX_MANIAC, TRAINER_STEVE_1},
    [STORY_TARGET_EISUKE] = {LOCALID_ARENA_LOBBY_COOK, LOCALID_ARENA_LOBBY_POKEVIRUS, OBJ_EVENT_GFX_RUNNING_TRIATHLETE_M, TRAINER_ISAIAH_1},
    [STORY_TARGET_YAYOI]  = {LOCALID_PIKE_LOBBY_COOK, LOCALID_PIKE_LOBBY_POKEVIRUS, OBJ_EVENT_GFX_WOMAN_2, TRAINER_ROSE_1},
    [STORY_TARGET_NOBUO]  = {LOCALID_PYRAMID_LOBBY_COOK, LOCALID_PYRAMID_LOBBY_POKEVIRUS, OBJ_EVENT_GFX_HIKER, TRAINER_DUSTY_1},
    [STORY_TARGET_YORIKO] = {LOCALID_CONTEST_LOBBY_COOK, LOCALID_CONTEST_LOBBY_POKEVIRUS, OBJ_EVENT_GFX_BEAUTY, TRAINER_JESSICA_1},
};

// The person whose release battle or cure is playing; kept across the battle. Not saved.
EWRAM_DATA static u8 sRescueTarget = 0;

// VAR_RESULT: someone other than Kazuki whose mind is back but who is not cured yet, on their way
// to the hub after their release battle; STORY_TARGET_COUNT if nobody is.
void FrontierStory_FindReturnedTarget(void)
{
    u32 target;

    gSpecialVar_Result = STORY_TARGET_COUNT;
    if (!IsStoryActive())
        return;
    for (target = STORY_TARGET_REIKA; target < STORY_TARGET_COUNT; target++)
    {
        if (GetTargetState(target) == TARGET_STORY_RETURNED)
        {
            gSpecialVar_Result = target;
            return;
        }
    }
}

// VAR_0x8004: STORY_TARGET_* other than Kazuki. Starts their fever, release battle or cure scene;
// their name goes in gStringVar1.
void FrontierStory_StartRescueScene(void)
{
    if (gSpecialVar_0x8004 == STORY_TARGET_KAZUKI || gSpecialVar_0x8004 >= STORY_TARGET_COUNT)
        return;
    sRescueTarget = gSpecialVar_0x8004;
    TryMarkFeverShown(sRescueTarget, GetTargetFever(sRescueTarget));
    StringCopy(gStringVar1, sStoryTargetTexts[sRescueTarget].name);
}

// For the current scene: VAR_0x8004, 0x8005 and 0x8006 become the local IDs of the person, the
// cook and Pokevirus in their lobby, and VAR_RESULT the person (STORY_TARGET_*). Their name goes
// in gStringVar1 again, in case a battle overwrote it.
void FrontierStory_GetRescueScene(void)
{
    gSpecialVar_0x8004 = sStoryTargetLocalIds[sRescueTarget];
    gSpecialVar_0x8005 = sStoryTargetRescues[sRescueTarget].cookLocalId;
    gSpecialVar_0x8006 = sStoryTargetRescues[sRescueTarget].pokevirusLocalId;
    gSpecialVar_Result = sRescueTarget;
    StringCopy(gStringVar1, sStoryTargetTexts[sRescueTarget].name);
}

// VAR_0x8004: RESCUE_TEXT_*. That part of the current scene goes in gStringVar4; VAR_RESULT is FALSE
// if the person has none.
void FrontierStory_BufferRescueText(void)
{
    const u8 *text = NULL;

    if (gSpecialVar_0x8004 < RESCUE_TEXT_COUNT)
        text = sStoryTargetRescueTexts[sRescueTarget][gSpecialVar_0x8004];
    gStringVar4[0] = EOS;
    gSpecialVar_Result = text != NULL;
    if (text != NULL)
        StringExpandPlaceholders(gStringVar4, text);
}

// Won the release battle: the person is on their way to the hub.
void FrontierStory_ReturnRescueTarget(void)
{
    if (sRescueTarget != STORY_TARGET_KAZUKI)
        SetTargetState(sRescueTarget, TARGET_STORY_RETURNED);
}

// On entering the hub with someone to cure: they become the current scene, shown as themselves.
void FrontierStory_PrepareHubRescue(void)
{
    FrontierStory_FindReturnedTarget();
    if (gSpecialVar_Result >= STORY_TARGET_COUNT)
        return;
    sRescueTarget = gSpecialVar_Result;
    VarSet(VAR_HUB_RESCUED_GFX, sStoryTargetRescues[sRescueTarget].graphicsId);
}

// For register_matchcall's steps: VAR_0x8004 and VAR_0x8000 become the person's Pokénav entry.
void FrontierStory_GetRescueMatchCall(void)
{
    gSpecialVar_0x8004 = gSpecialVar_0x8000 = sStoryTargetRescues[sRescueTarget].matchCallTrainer;
}

// Cured: the person is rescued.
void FrontierStory_FinishRescue(void)
{
    if (sRescueTarget == STORY_TARGET_KAZUKI)
        return;
    SetTargetState(sRescueTarget, TARGET_STORY_RESCUED);
    FlagSet(sRescuedFlags[sRescueTarget]);
}

// On entering the contest lobby: whether the next of its scenes about the world is due. One
// follows each category won at Master Rank after the first, while Yoriko wavers and once Darkrai
// is there to explain.
void FrontierStory_CheckContestScene(void)
{
    u32 seen = VarGet(VAR_CONTEST_STORY_SCENES);

    gSpecialVar_Result = IsStoryActive() && FlagGet(FLAG_STORY_DARKRAI_JOINED)
                      && seen < CONTEST_STORY_SCENE_COUNT
                      && GetTargetState(STORY_TARGET_YORIKO) == TARGET_STORY_WISH
                      && CountWonContestCategories(RANK_RIBBON_MASTER, seen + 2) >= seen + 2;
}

// VAR_0x8004: CONTEST_SCENE_*. Puts in gStringVar4 that speaker's lines in the current contest
// scene that change with the rescues so far.
void FrontierStory_BufferContestSceneBranch(void)
{
    u32 scene = VarGet(VAR_CONTEST_STORY_SCENES), stage = 0, i;

    gStringVar4[0] = EOS;
    if (scene >= CONTEST_STORY_SCENE_COUNT || gSpecialVar_0x8004 > CONTEST_SCENE_DARKRAI)
        return;
    for (i = 0; i < STORY_TARGET_COUNT; i++)
    {
        if (!FlagGet(sRescuedFlags[i]))
            continue;
        if (stage++ == 0)
            StringCopy(gStringVar2, sStoryTargetTexts[i].name);
    }
    if (stage >= STORY_RESCUE_STAGE_COUNT)
        stage = STORY_RESCUE_STAGE_COUNT - 1;
    StringExpandPlaceholders(gStringVar4, sContestSceneBranches[scene][gSpecialVar_0x8004][stage]);
}

// Once the cure is known, someone who remembered themself may come to the camp.
void FrontierStory_PickCampVisitor(void)
{
    u8 candidates[NAMED_STORY_COUNT];
    u32 i, count = 0;

    VarSet(VAR_CAMP_VISITOR, NAMED_STORY_NONE);
    gSpecialVar_Result = FALSE;
    if (!IsStoryActive() || !FlagGet(FLAG_STORY_CURE_FOUND))
        return;

    for (i = 0; i < NAMED_STORY_COUNT; i++)
    {
        if (GetNamedState(i) == NAMED_STORY_RECOVERED)
            candidates[count++] = i;
    }
    // Two visits in three, so the camp is not always waiting with someone.
    if (count == 0 || Random() % 3 == 0)
        return;

    i = candidates[Random() % count];
    VarSet(VAR_CAMP_VISITOR, i);
    VarSet(VAR_CAMP_VISITOR_GFX, GetNamedObjectGfx(i));
    gSpecialVar_Result = TRUE;
}

void FrontierStory_BufferVisitorText(void)
{
    u32 id = VarGet(VAR_CAMP_VISITOR);
    const u8 *text;

    if (id >= NAMED_STORY_COUNT)
    {
        gStringVar4[0] = EOS;
        return;
    }
    StringCopy(gStringVar1, GetNamedName(id));
    if (gSpecialVar_0x8004 == CAMP_VISITOR_TEXT_CURED)
        text = sNamedStoryTexts[id].cured;
    else
        text = sNamedStoryTexts[id].visit;
    // Expanded here so the message box doesn't expand gStringVar4 into itself.
    StringExpandPlaceholders(gStringVar4, text);
}

void FrontierStory_RescueVisitor(void)
{
    u32 id = VarGet(VAR_CAMP_VISITOR);

    if (id < NAMED_STORY_COUNT && GetNamedState(id) == NAMED_STORY_RECOVERED)
        SetNamedState(id, NAMED_STORY_RESCUED);
}

// VAR_0x8004: INN_AREA_*. On entering that part of the inn: shows a few of the Brains and guests
// cured so far who rest there, picked at random each visit.
void FrontierStory_PickInnGuests(void)
{
    u8 candidates[NAMED_STORY_COUNT];
    u32 i, slot, pick, count = 0;

    for (slot = 0; slot < INN_GUEST_SLOTS; slot++)
    {
        FlagSet(FLAG_INN_GUEST_HIDDEN + slot);
        VarSet(VAR_INN_GUEST + slot, NAMED_STORY_NONE);
    }
    if (!IsStoryActive() || gSpecialVar_0x8004 >= INN_AREA_COUNT)
        return;

    for (i = gSpecialVar_0x8004; i < NAMED_STORY_COUNT; i += INN_AREA_COUNT)
    {
        if (GetNamedState(i) == NAMED_STORY_RESCUED)
            candidates[count++] = i;
    }
    for (slot = 0; slot < INN_GUEST_SLOTS && count != 0; slot++)
    {
        pick = Random() % count;
        i = candidates[pick];
        candidates[pick] = candidates[--count];
        VarSet(VAR_INN_GUEST + slot, i);
        VarSet(VAR_INN_GUEST_GFX + slot, GetNamedObjectGfx(i));
        FlagClear(FLAG_INN_GUEST_HIDDEN + slot);
    }
}

// VAR_0x8004: the inn guest slot talked to. What they say goes in gStringVar4.
void FrontierStory_BufferInnGuestText(void)
{
    u32 id = NAMED_STORY_NONE;

    gStringVar4[0] = EOS;
    if (gSpecialVar_0x8004 < INN_GUEST_SLOTS)
        id = VarGet(VAR_INN_GUEST + gSpecialVar_0x8004);
    if (id < NAMED_STORY_COUNT)
        StringExpandPlaceholders(gStringVar4, sNamedStoryTexts[id].inn);
}

// What passers-by talk about: how many scenarios are rescued, or the ending.
u32 FrontierStory_GetCrowdStage(void)
{
    u32 rescued = CountRescued();

    if (FlagGet(FLAG_STORY_POKEVIRUS_FADED))
        return CROWD_STAGE_AFTER_ENDING;
    if (rescued >= 4)
        return CROWD_STAGE_PATH_OPEN;
    if (rescued >= 2)
        return CROWD_STAGE_FEW_RESCUED;
    return rescued == 1 ? CROWD_STAGE_FIRST_RESCUE : CROWD_STAGE_NONE_RESCUED;
}

// How a passer-by talks, by how they look; CROWD_VOICE_COUNT for anyone else.
u32 FrontierStory_GetCrowdVoice(u16 graphicsId)
{
    switch (graphicsId)
    {
    case OBJ_EVENT_GFX_BOY_1:
    case OBJ_EVENT_GFX_BOY_2:
    case OBJ_EVENT_GFX_BOY_3:
    case OBJ_EVENT_GFX_GIRL_1:
    case OBJ_EVENT_GFX_GIRL_2:
    case OBJ_EVENT_GFX_GIRL_3:
    case OBJ_EVENT_GFX_TWIN:
    case OBJ_EVENT_GFX_NINJA_BOY:
    case OBJ_EVENT_GFX_LITTLE_BOY:
    case OBJ_EVENT_GFX_LITTLE_GIRL:
    case OBJ_EVENT_GFX_SCHOOL_KID_M:
    case OBJ_EVENT_GFX_RICH_BOY:
    case OBJ_EVENT_GFX_YOUNGSTER:
    case OBJ_EVENT_GFX_CAMPER:
    case OBJ_EVENT_GFX_BUG_CATCHER:
        return CROWD_VOICE_CHILD;
    case OBJ_EVENT_GFX_MAN_1:
    case OBJ_EVENT_GFX_MAN_2:
    case OBJ_EVENT_GFX_MAN_3:
    case OBJ_EVENT_GFX_MAN_4:
    case OBJ_EVENT_GFX_MAN_5:
    case OBJ_EVENT_GFX_FAT_MAN:
    case OBJ_EVENT_GFX_HIKER:
    case OBJ_EVENT_GFX_BLACK_BELT:
    case OBJ_EVENT_GFX_MANIAC:
    case OBJ_EVENT_GFX_GENTLEMAN:
    case OBJ_EVENT_GFX_FISHERMAN:
    case OBJ_EVENT_GFX_SAILOR:
    case OBJ_EVENT_GFX_PSYCHIC_M:
    case OBJ_EVENT_GFX_COOK:
    case OBJ_EVENT_GFX_RUNNING_TRIATHLETE_M:
    case OBJ_EVENT_GFX_CYCLING_TRIATHLETE_M:
    case OBJ_EVENT_GFX_SWIMMER_M:
    case OBJ_EVENT_GFX_ARTIST:
    case OBJ_EVENT_GFX_POKEFAN_M:
    case OBJ_EVENT_GFX_SCIENTIST_1:
        return CROWD_VOICE_MAN;
    case OBJ_EVENT_GFX_PICNICKER:
    case OBJ_EVENT_GFX_LASS:
    case OBJ_EVENT_GFX_WOMAN_1:
    case OBJ_EVENT_GFX_WOMAN_2:
    case OBJ_EVENT_GFX_WOMAN_3:
    case OBJ_EVENT_GFX_WOMAN_4:
    case OBJ_EVENT_GFX_WOMAN_5:
    case OBJ_EVENT_GFX_BEAUTY:
    case OBJ_EVENT_GFX_POKEFAN_F:
    case OBJ_EVENT_GFX_HEX_MANIAC:
    case OBJ_EVENT_GFX_CYCLING_TRIATHLETE_F:
    case OBJ_EVENT_GFX_RUNNING_TRIATHLETE_F:
    case OBJ_EVENT_GFX_SWIMMER_F:
    case OBJ_EVENT_GFX_REPORTER_F:
        return CROWD_VOICE_WOMAN;
    case OBJ_EVENT_GFX_OLD_MAN:
    case OBJ_EVENT_GFX_OLD_WOMAN:
    case OBJ_EVENT_GFX_EXPERT_M:
    case OBJ_EVENT_GFX_EXPERT_F:
        return CROWD_VOICE_ELDER;
    }
    return CROWD_VOICE_COUNT;
}

EWRAM_DATA static u8 sLastCrowdLine = 0;

// One of the stage's lines in the voice at random, in gStringVar4: not the one heard last if
// there is another. FALSE if there are none.
bool32 FrontierStory_BufferCrowdLine(u32 stage, u32 voice)
{
    const struct CrowdLines *lines;
    u32 i;

    gStringVar4[0] = EOS;
    if (stage >= CROWD_STAGE_COUNT || voice >= CROWD_VOICE_COUNT)
        return FALSE;
    lines = &sCrowdLines[stage][voice];
    if (lines->count == 0)
        return FALSE;
    i = Random() % lines->count;
    if (i == sLastCrowdLine && lines->count > 1)
        i = (i + 1) % lines->count;
    sLastCrowdLine = i;
    StringExpandPlaceholders(gStringVar4, lines->lines[i]);
    return TRUE;
}

// The passer-by just talked to (VAR_LAST_TALKED).
void FrontierStory_BufferCrowdTalk(void)
{
    u32 voice = CROWD_VOICE_COUNT;
    u8 objectEventId;

    if (!TryGetObjectEventIdByLocalIdAndMap(gSpecialVar_LastTalked, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId))
        voice = FrontierStory_GetCrowdVoice(gObjectEvents[objectEventId].graphicsId);
    if (voice >= CROWD_VOICE_COUNT)
        voice = CROWD_VOICE_MAN;
    FrontierStory_BufferCrowdLine(FrontierStory_GetCrowdStage(), voice);
}

// A facility attendant's word after their welcome, in gStringVar4. VAR_RESULT is FALSE on saves
// from before the opening, where they only welcome the player.
void FrontierStory_BufferStaffAside(void)
{
    gStringVar4[0] = EOS;
    gSpecialVar_Result = IsStoryActive();
    if (gSpecialVar_Result)
        StringExpandPlaceholders(gStringVar4, sStaffAsideLines[FrontierStory_GetCrowdStage()][Random() % STAFF_ASIDE_LINES]);
}

u32 FrontierStory_GetRegularPhase(void)
{
    switch (FrontierStory_GetCrowdStage())
    {
    case CROWD_STAGE_NONE_RESCUED:
        return REGULAR_PHASE_BEFORE_RESCUES;
    case CROWD_STAGE_AFTER_ENDING:
        return REGULAR_PHASE_AFTER_ENDING;
    }
    return REGULAR_PHASE_RESCUING;
}

// What a regular (REGULAR_*) says in the phase, in gStringVar4; FALSE if there is no such line.
bool32 FrontierStory_BufferRegularLine(u32 regular, u32 phase)
{
    gStringVar4[0] = EOS;
    if (regular >= REGULAR_COUNT || phase >= REGULAR_PHASE_COUNT || sRegularLines[regular][phase] == NULL)
        return FALSE;
    StringExpandPlaceholders(gStringVar4, sRegularLines[regular][phase]);
    return TRUE;
}

// VAR_0x8004: the regular talked to.
void FrontierStory_BufferRegularTalk(void)
{
    FrontierStory_BufferRegularLine(gSpecialVar_0x8004, FrontierStory_GetRegularPhase());
}

static bool32 TryGetLocalObject(u16 localId, struct ObjectEvent **object)
{
    u8 objectEventId;

    if (TryGetObjectEventIdByLocalIdAndMap(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId))
        return FALSE;
    *object = &gObjectEvents[objectEventId];
    return TRUE;
}

static const u8 sFaceMovementTypes[] =
{
    [DIR_SOUTH] = MOVEMENT_TYPE_FACE_DOWN,
    [DIR_NORTH] = MOVEMENT_TYPE_FACE_UP,
    [DIR_WEST]  = MOVEMENT_TYPE_FACE_LEFT,
    [DIR_EAST]  = MOVEMENT_TYPE_FACE_RIGHT,
};

static const s8 sDirectionSteps[][2] =
{
    [DIR_SOUTH] = { 0,  1},
    [DIR_NORTH] = { 0, -1},
    [DIR_WEST]  = {-1,  0},
    [DIR_EAST]  = { 1,  0},
};

static const u8 sOppositeDirections[] =
{
    [DIR_SOUTH] = DIR_NORTH,
    [DIR_NORTH] = DIR_SOUTH,
    [DIR_WEST]  = DIR_EAST,
    [DIR_EAST]  = DIR_WEST,
};

// Open floor that no other shown object stands on or is stepping off; `self` doesn't count.
static bool32 IsSpotFree(s32 x, s32 y, struct ObjectEvent *self)
{
    u32 i;

    if (x < 0 || y < 0 || x >= gBackupMapLayout.width || y >= gBackupMapLayout.height
     || MapGridGetCollisionAt(x, y) != 0)
        return FALSE;
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent *object = &gObjectEvents[i];

        if (object == self || !object->active || object->invisible)
            continue;
        if ((object->currentCoords.x == x && object->currentCoords.y == y)
         || (object->previousCoords.x == x && object->previousCoords.y == y))
            return FALSE;
    }
    return TRUE;
}

// Also updates the map's template for this visit, so the scene looks the same after a battle.
static void MoveAndTurnObject(struct ObjectEvent *object, s16 x, s16 y, enum Direction direction)
{
    MoveObjectEventToMapCoords(object, x, y);
    ObjectEventTurn(object, direction);
    SetObjEventTemplateCoords(object->localId, x - MAP_OFFSET, y - MAP_OFFSET);
    if (direction < ARRAY_COUNT(sFaceMovementTypes) && sFaceMovementTypes[direction] != 0)
        SetObjEventTemplateMovementType(object->localId, sFaceMovementTypes[direction]);
}

// Before a release battle. VAR_0x8004: the infected person the player is talking to,
// VAR_0x8005: the cook, VAR_0x8006: Pokevirus. The cook stands at the player's side and
// Pokevirus floats two steps behind the infected person, so the two pairs face each other.
void FrontierStory_ArrangeReleaseScene(void)
{
    struct ObjectEvent *infected, *cook, *pokevirus;
    s16 px, py, ix, iy, dx, dy, x, y;
    u32 i;

    if (!TryGetLocalObject(gSpecialVar_0x8004, &infected)
     || !TryGetLocalObject(gSpecialVar_0x8005, &cook)
     || !TryGetLocalObject(gSpecialVar_0x8006, &pokevirus))
        return;

    PlayerGetDestCoords(&px, &py);
    ix = infected->currentCoords.x;
    iy = infected->currentCoords.y;
    dx = (px > ix) - (px < ix);
    dy = (py > iy) - (py < iy);
    if (dx != 0 && dy != 0)
        dy = 0;

    MoveAndTurnObject(infected, ix, iy, GetDirectionToFace(ix, iy, px, py));
    // People can come up to the player from any side, so take the first open spot for the
    // cook beside the player and for Pokevirus behind the infected person.
    for (i = 0; i < 2; i++)
    {
        x = px + (i == 0 ? 1 : -1) * (dx == 0);
        y = py + (i == 0 ? 1 : -1) * (dy == 0);
        if (IsSpotFree(x, y, cook) || i == 1)
            break;
    }
    MoveAndTurnObject(cook, x, y, GetDirectionToFace(px, py, ix, iy));
    x = ix - 2 * dx;
    y = iy - 2 * dy;
    if (!IsSpotFree(x, y, pokevirus))
    {
        x = ix - dx;
        y = iy - dy;
    }
    MoveAndTurnObject(pokevirus, x, y, GetDirectionToFace(ix, iy, px, py));
}

// VAR_0x8004: an object to stand on the first free side of the player, facing them.
void FrontierStory_PlaceBesidePlayer(void)
{
    static const s8 sSides[][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
    struct ObjectEvent *object;
    s16 px, py, x, y;
    u32 i;

    if (!TryGetLocalObject(gSpecialVar_0x8004, &object))
        return;
    PlayerGetDestCoords(&px, &py);
    for (i = 0; i < ARRAY_COUNT(sSides); i++)
    {
        x = px + sSides[i][0];
        y = py + sSides[i][1];
        if (MapGridGetCollisionAt(x, y) == 0 && GetObjectEventIdByXY(x, y) == OBJECT_EVENTS_COUNT)
        {
            MoveAndTurnObject(object, x, y, GetDirectionToFace(x, y, px, py));
            return;
        }
    }
}

// The last walk to the player, so the person can go back the way they came. Not saved.
struct Approach
{
    u8 localId;
    u8 steps;
    u8 facing; // before they set off
    u8 directions[APPROACH_MAX_STEPS];
};

EWRAM_DATA static struct Approach sApproach = {0};
// Room for an exclamation mark and a pause, the walk, a turn and the end.
EWRAM_DATA static u8 sApproachMovement[APPROACH_MAX_STEPS + 5] = {0};
EWRAM_DATA static u8 sPlayerTurnMovement[APPROACH_MAX_STEPS + 5] = {0};

// Breadth-first search over the loaded map from the object to any open side of the player.
// Returns the number of steps written to sApproach.directions, or -1 if there is no short path.
static s32 FindPathToPlayer(struct ObjectEvent *object, s16 px, s16 py)
{
    s32 width = gBackupMapLayout.width, height = gBackupMapLayout.height;
    s32 head = 0, tail = 0, goal = -1, cell, steps = 0, x, y, i;
    u8 *cameFrom = AllocZeroed(width * height); // the direction each cell was entered by
    u16 *queue = Alloc(width * height * sizeof(u16));
    u32 dir;

    if (cameFrom != NULL && queue != NULL)
    {
        cell = object->currentCoords.y * width + object->currentCoords.x;
        cameFrom[cell] = PATH_START;
        queue[tail++] = cell;
        while (head < tail)
        {
            cell = queue[head++];
            x = cell % width;
            y = cell / width;
            if (abs(x - px) + abs(y - py) == 1)
            {
                goal = cell;
                break;
            }
            for (dir = DIR_SOUTH; dir <= DIR_EAST; dir++)
            {
                s32 nextX = x + sDirectionSteps[dir][0], nextY = y + sDirectionSteps[dir][1];

                if (!IsSpotFree(nextX, nextY, object) || cameFrom[nextY * width + nextX] != 0)
                    continue;
                cameFrom[nextY * width + nextX] = dir;
                queue[tail++] = nextY * width + nextX;
            }
        }
    }
    // Count the steps back from the goal, then write them out in walking order.
    for (cell = goal; cell >= 0 && cameFrom[cell] != PATH_START; steps++)
        cell -= sDirectionSteps[cameFrom[cell]][1] * width + sDirectionSteps[cameFrom[cell]][0];
    if (goal < 0 || steps > APPROACH_MAX_STEPS)
        steps = -1;
    for (cell = goal, i = steps - 1; i >= 0; i--)
    {
        sApproach.directions[i] = cameFrom[cell];
        cell -= sDirectionSteps[cameFrom[cell]][1] * width + sDirectionSteps[cameFrom[cell]][0];
    }
    Free(cameFrom);
    Free(queue);
    return steps;
}

// Walks someone by the shortest open path to the player's side to face them; the player turns to
// meet them as they arrive. With `exclaim`, they first show an exclamation mark and pause (also
// when there is no path). FALSE if they do not walk.
static bool32 StartApproach(u8 localId, bool32 exclaim)
{
    struct ObjectEvent *object;
    s16 px, py, x, y;
    s32 steps, lead = 0, i;

    sApproach.steps = 0;
    // Someone across a wide room may not be loaded yet; they walk in from out of view.
    TrySpawnObjectEvent(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    if (!TryGetLocalObject(localId, &object))
        return FALSE;
    if (exclaim)
    {
        sApproachMovement[lead++] = MOVEMENT_ACTION_EMOTE_EXCLAMATION_MARK;
        sApproachMovement[lead++] = MOVEMENT_ACTION_DELAY_16;
        sApproachMovement[lead++] = MOVEMENT_ACTION_DELAY_16;
    }
    PlayerGetDestCoords(&px, &py);
    steps = FindPathToPlayer(object, px, py);
    if (steps < 0)
    {
        sApproachMovement[lead] = MOVEMENT_ACTION_STEP_END;
        if (exclaim)
            ScriptMovement_StartObjectMovementScript(localId, gSaveBlock1Ptr->location.mapNum,
                                                     gSaveBlock1Ptr->location.mapGroup, sApproachMovement);
        return FALSE;
    }

    sApproach.localId = localId;
    sApproach.steps = steps;
    sApproach.facing = object->facingDirection;
    x = object->currentCoords.x;
    y = object->currentCoords.y;
    // The player waits through the pause and the walk, then turns on the last step.
    for (i = 0; i < steps + lead - 1; i++)
        sPlayerTurnMovement[i] = MOVEMENT_ACTION_DELAY_16;
    for (i = 0; i < steps; i++)
    {
        sApproachMovement[lead + i] = GetWalkNormalMovementAction(sApproach.directions[i]);
        x += sDirectionSteps[sApproach.directions[i]][0];
        y += sDirectionSteps[sApproach.directions[i]][1];
    }
    sApproachMovement[lead + steps] = GetFaceDirectionMovementAction(GetDirectionToFace(x, y, px, py));
    sApproachMovement[lead + steps + 1] = MOVEMENT_ACTION_STEP_END;
    i = steps + lead > 0 ? steps + lead - 1 : 0;
    sPlayerTurnMovement[i] = GetFaceDirectionMovementAction(GetDirectionToFace(px, py, x, y));
    sPlayerTurnMovement[i + 1] = MOVEMENT_ACTION_STEP_END;

    ScriptMovement_StartObjectMovementScript(localId, gSaveBlock1Ptr->location.mapNum,
                                             gSaveBlock1Ptr->location.mapGroup, sApproachMovement);
    ScriptMovement_StartObjectMovementScript(LOCALID_PLAYER, gSaveBlock1Ptr->location.mapNum,
                                             gSaveBlock1Ptr->location.mapGroup, sPlayerTurnMovement);
    return TRUE;
}

// VAR_0x8004: someone who walks over to the player (see StartApproach). Wait for both with
// waitmovement. VAR_RESULT is FALSE (and nobody moves) if there is no path.
void FrontierStory_ApproachPlayer(void)
{
    gSpecialVar_Result = StartApproach(gSpecialVar_0x8004, FALSE);
}


// VAR_0x8004: STORY_TARGET_*. They notice the player and walk over. VAR_0x8005 becomes their local
// ID for waitmovement; VAR_RESULT is FALSE if they could only call out from where they stand.
void FrontierStory_TargetComesOver(void)
{
    u8 localId = sStoryTargetLocalIds[gSpecialVar_0x8004];

    gSpecialVar_0x8005 = localId;
    gSpecialVar_Result = StartApproach(localId, TRUE);
}

// Sends the person from the last walk over back to their place. VAR_0x8005 becomes their local
// ID for waitmovement.
void FrontierStory_ReturnFromApproach(void)
{
    u32 i;

    for (i = 0; i < sApproach.steps; i++)
        sApproachMovement[i] = GetWalkNormalMovementAction(sOppositeDirections[sApproach.directions[sApproach.steps - 1 - i]]);
    sApproachMovement[i] = GetFaceDirectionMovementAction(sApproach.facing);
    sApproachMovement[i + 1] = MOVEMENT_ACTION_STEP_END;
    ScriptMovement_StartObjectMovementScript(sApproach.localId, gSaveBlock1Ptr->location.mapNum,
                                             gSaveBlock1Ptr->location.mapGroup, sApproachMovement);
    gSpecialVar_0x8005 = sApproach.localId;
    sApproach.steps = 0;
}

// On entering the Tower lobby: does Kazuki have something new to tell the player? He comes over
// the first time at each step of his story; the fever comes once alone and once more when Darkrai
// can step in. Records that he came, so it happens once.
void TowerStory_CheckKazukiApproach(void)
{
    u32 state = VarGet(VAR_TOWER_STORY_STATE), flag;

    gSpecialVar_Result = FALSE;
    if (!IsStoryActive() || state >= TOWER_STORY_RETURNED)
        return;
    if (FlagGet(FLAG_SYS_TOWER_GOLD))
    {
        flag = FlagGet(FLAG_STORY_DARKRAI_JOINED) ? FLAG_STORY_TOWER_RELEASE_SHOWN : FLAG_STORY_TOWER_FEVER_SHOWN;
        gSpecialVar_Result = !FlagGet(flag);
        FlagSet(FLAG_STORY_TOWER_FEVER_SHOWN);
        FlagSet(flag);
    }
    else
    {
        gSpecialVar_Result = state == TOWER_STORY_NOT_MET
                          || (state == TOWER_STORY_MET && FlagGet(FLAG_SYS_TOWER_SILVER));
    }
}
