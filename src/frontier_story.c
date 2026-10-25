#include "global.h"
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
#include "overworld.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "string_util.h"
#include "constants/battle_frontier.h"
#include "constants/characters.h"
#include "constants/frontier_util.h"
#include "constants/vars.h"

#include "data/frontier_story_named.h"

// The Frontier never asks for less than level 50, so story battles don't either.
#define STORY_BATTLE_MIN_LEVEL 50
#define RANK_RIBBON_HYPER 3

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

void FrontierStory_CountRescued(void)
{
    u32 i, count = 0;

    for (i = 0; i < ARRAY_COUNT(sRescuedFlags); i++)
        count += FlagGet(sRescuedFlags[i]);
    gSpecialVar_Result = count;
}

static bool32 HasHyperRankRibbon(struct BoxPokemon *boxMon)
{
    u32 field;

    if (!GetBoxMonData(boxMon, MON_DATA_SANITY_HAS_SPECIES))
        return FALSE;
    for (field = MON_DATA_COOL_RIBBON; field <= MON_DATA_TOUGH_RIBBON; field++)
    {
        if (GetBoxMonData(boxMon, field) >= RANK_RIBBON_HYPER)
            return TRUE;
    }
    return FALSE;
}

// Darkrai joins after the first Silver Symbol or the first Hyper Rank win in any category.
void FrontierStory_CanDarkraiJoin(void)
{
    u32 i, j;

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
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (HasHyperRankRibbon(&gParties[B_TRAINER_PLAYER][i].box))
        {
            gSpecialVar_Result = TRUE;
            return;
        }
    }
    for (i = 0; i < TOTAL_BOXES_COUNT; i++)
    {
        for (j = 0; j < IN_BOX_COUNT; j++)
        {
            if (HasHyperRankRibbon(GetBoxedMonPtr(i, j)))
            {
                gSpecialVar_Result = TRUE;
                return;
            }
        }
    }
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
    s16 px, py, ix, iy, dx, dy;

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
    MoveAndTurnObject(cook, px + (dx == 0), py + (dy == 0), GetDirectionToFace(px, py, ix, iy));
    MoveAndTurnObject(pokevirus, ix - 2 * dx, iy - 2 * dy, GetDirectionToFace(ix, iy, px, py));
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
