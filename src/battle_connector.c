#include "global.h"
#include "battle.h"
#include "battle_connector.h"
#include "battle_frontier.h"
#include "battle_setup.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "item.h"
#include "main.h"
#include "overworld.h"
#include "pokedex.h"
#include "pokemon.h"
#include "random.h"
#include "script_pokemon_util.h"
#include "shadow_pokemon.h"
#include "string_util.h"
#include "strings.h"
#include "constants/battle.h"
#include "constants/battle_frontier_mons.h"
#include "constants/hold_effects.h"
#include "constants/characters.h"
#include "constants/event_objects.h"
#include "constants/items.h"
#include "constants/opponents.h"
#include "constants/region_map_sections.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/vars.h"

/*
    Battle Connector: the Shadow Pokemon search and rescue facility in Artisan Cave (docs/shadow_pokemon.md).

    A run: the player brings one Pokemon. Each branch offers three paths with vague hints; each path leads to a room
    (a Shadow Pokemon, a person who shut themselves away, rest, a calming scent, an item...). Defeated Shadow
    Pokemon can be protected: they join the party for the run (up to BATTLE_CONNECTOR_PARTY_SIZE) and make the list
    the player takes one Pokemon home from at the end. The party is put back as it was whatever happens.

    Nothing of a run is saved: the run lives in sRun (EWRAM) and saving is off in the facility (start menu), so a
    reset goes back to the save made before the run, with the full party. sRun keeps everything a resume would
    need (rooms, paths, the party before the run, the protected Pokemon), so it can move to the save later.

    It shares nothing with the Battle Frontier: no frontier challenge state, streak, trainers, or save data.
*/

#define NO_SLOT 0xFF
#define NO_SET  0xFFFF // a Pokemon made from its species alone (the boss)

struct BattleConnectorRun
{
    u8 active:1;
    u8 preciseHints:1;  // a calm room makes the next branch's hints exact
    u8 midbossDone:1;
    u8 returnState:3;   // CONNECTOR_RETURN_*: why the player is back at the reception
    u8 padding:2;
    u8 branch;          // branches passed
    u8 roomType;        // the room the player is in
    u8 pathRoomTypes[CONNECTOR_PATH_COUNT];
    u8 pathHints[CONNECTOR_PATH_COUNT];
    u8 pathsRolledFor;  // branch + 1 the paths are rolled for (0: none)
    u16 roomSpecies[2]; // the Shadow Pokemon of the room (the second: a strong trainer's)
    u16 roomSets[2];    // their Battle Frontier pool sets (gBattleFrontierMons), or NO_SET
    u8 roomTrainer;     // index into the room type's trainer table
    u8 baseLevel;       // the level of the Pokemon brought
    u8 protectCursor;   // the next opponent party slot to offer for protection
    u8 offeredSlot;     // the opponent party slot offered now
    u8 lastProtected;   // the candidate protected last (for a swap)
    u8 candidateCount;
    u16 battleSong;
    struct Pokemon savedParty[PARTY_SIZE];
    struct Pokemon candidates[BATTLE_CONNECTOR_MAX_CANDIDATES];
};

static EWRAM_DATA struct BattleConnectorRun sRun = {0};
static EWRAM_DATA bool8 sInConnectorBattle = FALSE;
static EWRAM_DATA bool8 sDynamaxFlagWasSet = FALSE;

#include "data/battle_connector.h"

static void CB2_EndBattleConnectorBattle(void);

// The party (with its held items) is put back when a run ends, so held items cannot be given or taken in the facility
const u8 gText_BattleConnectorCantChangeItems[] = _("{JPN}ここでは もちものを\nもたせたり あずかったり できない！");

// ---- state ------------------------------------------------------------------------------------------------------------

bool32 BattleConnector_IsChallengeActive(void)
{
    return sRun.active;
}

bool32 BattleConnector_IsBattleActive(void)
{
    return sInConnectorBattle;
}

// Colosseum's battle music: the ordinary rooms, the strong ones, the boss
static u16 GetRoomSong(u32 roomType)
{
    switch (roomType)
    {
    case CONNECTOR_ROOM_STRONG_SHADOW:
    case CONNECTOR_ROOM_RARE_SHADOW:
    case CONNECTOR_ROOM_STRONG_TRAINER:
    case CONNECTOR_ROOM_MIDBOSS:
        return MUS_VS_COLOSSEUM_2;
    case CONNECTOR_ROOM_BOSS:
        return MUS_VS_COLOSSEUM_ADMIN;
    default:
        return MUS_VS_COLOSSEUM;
    }
}

u16 BattleConnector_GetBattleBGM(void)
{
    return sInConnectorBattle ? sRun.battleSong : 0;
}

static bool32 IsMonPresent(struct Pokemon *mon)
{
    return GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(mon, MON_DATA_IS_EGG);
}

static u32 CountPartyMons(void)
{
    u32 count = 0;
    for (u32 i = 0; i < PARTY_SIZE; i++)
        count += IsMonPresent(&gParties[B_TRAINER_PLAYER][i]);
    return count;
}

// The protected Pokemon in the party are the ones in use: their changes go back to the list (by personality, so a
// reordered party does not matter).
static void SyncPartyToCandidates(void)
{
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (!IsMonPresent(mon))
            continue;
        u32 personality = GetMonData(mon, MON_DATA_PERSONALITY);
        for (u32 j = 0; j < sRun.candidateCount; j++)
        {
            if (GetMonData(&sRun.candidates[j], MON_DATA_PERSONALITY) == personality)
                sRun.candidates[j] = *mon;
        }
    }
}

static void RestorePlayerParty(void)
{
    memcpy(gParties[B_TRAINER_PLAYER], sRun.savedParty, sizeof(sRun.savedParty));
    CalculatePlayerPartyCount();
}

static void ClearRun(u32 returnState)
{
    memset(&sRun, 0, sizeof(sRun));
    sRun.returnState = returnState;
}

// ---- Shadow Pokemon ---------------------------------------------------------------------------------------------------

// At least count IVs of 31 (at random stats); the others stay as they are
static void EnsurePerfectIVs(struct Pokemon *mon, u32 count)
{
    const u8 perfect = MAX_PER_STAT_IVS;
    u32 have = 0;

    count = min(count, (u32)NUM_STATS);
    for (u32 stat = 0; stat < NUM_STATS; stat++)
        have += (GetMonData(mon, MON_DATA_HP_IV + stat) == MAX_PER_STAT_IVS);
    for (; have < count; have++)
    {
        // One of the stats not at 31 yet
        u32 pick = RandomUniform(RNG_NONE, 0, NUM_STATS - have - 1);
        for (u32 stat = 0; stat < NUM_STATS; stat++)
        {
            if (GetMonData(mon, MON_DATA_HP_IV + stat) != MAX_PER_STAT_IVS && pick-- == 0)
            {
                SetMonData(mon, MON_DATA_HP_IV + stat, &perfect);
                break;
            }
        }
    }
}

// What every Shadow Pokemon of the facility gets: at least BATTLE_CONNECTOR_PERFECT_IVS IVs of 31, its Shadow state,
// and the Battle Connector mark (its purification move comes from it)
static void FinishShadowMon(struct Pokemon *mon)
{
    const u8 origin = TRUE;
    const u8 metLocation = MAPSEC_ARTISAN_CAVE;
    u16 species = GetMonData(mon, MON_DATA_SPECIES);

    EnsurePerfectIVs(mon, max(BATTLE_CONNECTOR_PERFECT_IVS, gSpeciesInfo[species].perfectIVCount));
    SetMonData(mon, MON_DATA_MET_LOCATION, &metLocation);
    SetMonData(mon, MON_DATA_BATTLE_CONNECTOR_ORIGIN, &origin);
    MarkShadowPokemon(mon);
    CalculateMonStats(mon);
}

static u32 GetPlayerOTID(void)
{
    return gSaveBlock2Ptr->playerTrainerId[0] | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
         | (gSaveBlock2Ptr->playerTrainerId[2] << 16) | (gSaveBlock2Ptr->playerTrainerId[3] << 24);
}

// A Shadow Pokemon from its species alone (the boss): random personality (nature, gender, ability from its usual two),
// random IVs, the usual Tera type
void BattleConnector_CreateShadowMon(struct Pokemon *mon, u16 species, u32 level)
{
    // The random state can be the same when a run is played the same way after loading (the boss came out with the same
    // nature each time): the frame counter goes into its personality and IVs too
    u32 seed = Random32() ^ (gMain.vblankCounter1 * 0x9E3779B1u);
    u32 ivs = 0;

    CreateMon(mon, species, level, seed, OTID_STRUCT_PLAYER_ID);
    // CreateMon leaves the IVs at 0 and the moves empty
    for (u32 stat = 0; stat < NUM_STATS; stat++)
    {
        seed = seed * 1103515245u + 12345u;
        ivs |= ((seed >> 16) % (MAX_PER_STAT_IVS + 1)) << (stat * 5);
    }
    SetMonData(mon, MON_DATA_IVS, &ivs);
    GiveMonInitialMoveset(mon);
    FinishShadowMon(mon);
}

// A Shadow Pokemon from a Battle Frontier pool set (the rooms, the mid-boss): the set's moves, item, nature, ability,
// EVs, IVs and Tera type (not a set's nickname). It cannot Mega Evolve or use Z-Moves, so it holds no Mega Stone or
// Z-Crystal.
void BattleConnector_CreateShadowMonFromSet(struct Pokemon *mon, u16 set, u32 level)
{
    const u16 none = ITEM_NONE;
    enum HoldEffect holdEffect;

    CreateFacilityMon(&gBattleFrontierMons[set], level, USE_RANDOM_IVS, GetPlayerOTID(), 0, mon);
    // A Frontier trainer's nickname does not belong to a wild Shadow Pokemon
    if (gBattleFrontierMons[set].nickname != NULL)
        SetMonData(mon, MON_DATA_NICKNAME, GetSpeciesName(gBattleFrontierMons[set].species));
    holdEffect = GetItemHoldEffect(GetMonData(mon, MON_DATA_HELD_ITEM));
    if (holdEffect == HOLD_EFFECT_MEGA_STONE || holdEffect == HOLD_EFFECT_Z_CRYSTAL)
        SetMonData(mon, MON_DATA_HELD_ITEM, &none);
    FinishShadowMon(mon);
}

// ---- who appears ------------------------------------------------------------------------------------------------------

// A species of the rooms: Battle Frontier pool Pokemon up to BATTLE_CONNECTOR_MAX_DEX_NUM, no Legendary or Mythical
bool32 BattleConnector_IsRoomSpecies(u16 species)
{
    u32 dexNum;

    if (species == SPECIES_NONE || species >= NUM_SPECIES || GET_BASE_SPECIES_ID(species) != species)
        return FALSE;
    dexNum = SpeciesToNationalPokedexNum(species);
    if (dexNum == 0 || dexNum > BATTLE_CONNECTOR_MAX_DEX_NUM)
        return FALSE;
    return !gSpeciesInfo[species].isRestrictedLegendary && !gSpeciesInfo[species].isSubLegendary
        && !gSpeciesInfo[species].isMythical;
}

// The ordinary, the strong and the rare rooms' Pokemon, by base stat total
u32 BattleConnector_GetSpeciesTier(u16 species)
{
    u32 total = GetSpeciesBaseStatTotal(species);

    if (total >= BATTLE_CONNECTOR_RARE_BST)
        return CONNECTOR_TIER_RARE;
    if (total >= BATTLE_CONNECTOR_STRONG_BST)
        return CONNECTOR_TIER_STRONG;
    return CONNECTOR_TIER_NORMAL;
}

static bool32 IsSetOfTier(u32 set, u32 tier)
{
    u16 species = gBattleFrontierMons[set].species;
    return BattleConnector_IsRoomSpecies(species) && BattleConnector_GetSpeciesTier(species) == tier;
}

// A random set of the tier (each set counts once, so a species with more sets comes more often)
static u16 PickRoomSet(u32 tier)
{
    u32 count = 0, pick;

    for (u32 set = 0; set < NUM_FRONTIER_MONS; set++)
        count += IsSetOfTier(set, tier);
    if (count == 0)
        return NO_SET;
    pick = RandomUniform(RNG_NONE, 0, count - 1);
    for (u32 set = 0; set < NUM_FRONTIER_MONS; set++)
    {
        if (IsSetOfTier(set, tier) && pick-- == 0)
            return set;
    }
    return NO_SET;
}

// A random set of the species (NO_SET if the pool has none)
static u16 PickSpeciesSet(u16 species)
{
    u32 count = 0, pick;

    for (u32 set = 0; set < NUM_FRONTIER_MONS; set++)
        count += (gBattleFrontierMons[set].species == species);
    if (count == 0)
        return NO_SET;
    pick = RandomUniform(RNG_NONE, 0, count - 1);
    for (u32 set = 0; set < NUM_FRONTIER_MONS; set++)
    {
        if (gBattleFrontierMons[set].species == species && pick-- == 0)
            return set;
    }
    return NO_SET;
}

// A room's Pokemon from a set; with no set in the pool (it should not happen), a Mightyena from its species alone
static void SetRoomMonFromTier(u32 slot, u32 tier)
{
    u16 set = PickRoomSet(tier);

    sRun.roomSets[slot] = set;
    sRun.roomSpecies[slot] = (set != NO_SET) ? gBattleFrontierMons[set].species : SPECIES_MIGHTYENA;
}

static void SetRoomMonFromSpecies(u32 slot, u16 species, bool32 useSet)
{
    sRun.roomSets[slot] = useSet ? PickSpeciesSet(species) : NO_SET;
    sRun.roomSpecies[slot] = species;
}

static void CreateRoomShadowMon(struct Pokemon *mon, u32 slot, u32 level)
{
    if (sRun.roomSets[slot] != NO_SET)
        BattleConnector_CreateShadowMonFromSet(mon, sRun.roomSets[slot], level);
    else
        BattleConnector_CreateShadowMon(mon, sRun.roomSpecies[slot], level);
}

static u32 GetLevelOffset(u32 roomType)
{
    switch (roomType)
    {
    case CONNECTOR_ROOM_STRONG_SHADOW:  return BATTLE_CONNECTOR_LEVEL_STRONG_SHADOW;
    case CONNECTOR_ROOM_RARE_SHADOW:    return BATTLE_CONNECTOR_LEVEL_RARE_SHADOW;
    case CONNECTOR_ROOM_DARK_TRAINER:   return BATTLE_CONNECTOR_LEVEL_TRAINER;
    case CONNECTOR_ROOM_STRONG_TRAINER: return BATTLE_CONNECTOR_LEVEL_STRONG_TRAINER;
    case CONNECTOR_ROOM_MIDBOSS:        return BATTLE_CONNECTOR_LEVEL_MIDBOSS;
    case CONNECTOR_ROOM_BOSS:           return BATTLE_CONNECTOR_LEVEL_BOSS;
    default:                            return BATTLE_CONNECTOR_LEVEL_SHADOW;
    }
}

static u32 GetRoomLevel(u32 roomType)
{
    u32 level = sRun.baseLevel + GetLevelOffset(roomType);
    return min(max(level, (u32)BATTLE_CONNECTOR_LEVEL_MIN), (u32)MAX_LEVEL);
}

static bool32 IsEntryUnlocked(const struct ConnectorSpeciesEntry *entry)
{
    return VarGet(VAR_BATTLE_CONNECTOR_CLEARS) >= entry->minClears && (entry->flag == 0 || FlagGet(entry->flag));
}

static u16 PickUnlocked(const struct ConnectorSpeciesEntry *pool, u32 count)
{
    u32 unlocked = 0, pick;

    for (u32 i = 0; i < count; i++)
        unlocked += IsEntryUnlocked(&pool[i]);
    if (unlocked == 0)
        return pool[0].species;
    pick = RandomUniform(RNG_NONE, 0, unlocked - 1);
    for (u32 i = 0; i < count; i++)
    {
        if (IsEntryUnlocked(&pool[i]) && pick-- == 0)
            return pool[i].species;
    }
    return pool[0].species;
}

// ---- rooms ------------------------------------------------------------------------------------------------------------

static bool32 IsTrainerRoom(u32 roomType)
{
    return roomType == CONNECTOR_ROOM_DARK_TRAINER || roomType == CONNECTOR_ROOM_STRONG_TRAINER;
}

static bool32 IsShadowRoom(u32 roomType)
{
    return roomType == CONNECTOR_ROOM_SHADOW || roomType == CONNECTOR_ROOM_STRONG_SHADOW
        || roomType == CONNECTOR_ROOM_RARE_SHADOW || roomType == CONNECTOR_ROOM_MIDBOSS || roomType == CONNECTOR_ROOM_BOSS;
}

static const struct ConnectorTrainer *GetRoomTrainer(void)
{
    if (sRun.roomType == CONNECTOR_ROOM_STRONG_TRAINER)
        return &sConnectorStrongTrainers[sRun.roomTrainer % ARRAY_COUNT(sConnectorStrongTrainers)];
    return &sConnectorDarkTrainers[sRun.roomTrainer % ARRAY_COUNT(sConnectorDarkTrainers)];
}

// Decides what the room holds: its Shadow Pokemon are known before the battle, so the room can show them
static void SetUpRoom(u32 roomType)
{
    if (roomType == CONNECTOR_ROOM_RANDOM)
        roomType = sConnectorRandomRooms[RandomUniform(RNG_NONE, 0, ARRAY_COUNT(sConnectorRandomRooms) - 1)];

    sRun.roomType = roomType;
    sRun.roomSpecies[0] = sRun.roomSpecies[1] = SPECIES_NONE;
    sRun.roomSets[0] = sRun.roomSets[1] = NO_SET;
    sRun.roomTrainer = 0;
    sRun.protectCursor = 0;
    sRun.offeredSlot = NO_SLOT;
    switch (roomType)
    {
    case CONNECTOR_ROOM_SHADOW:
        SetRoomMonFromTier(0, CONNECTOR_TIER_NORMAL);
        break;
    case CONNECTOR_ROOM_STRONG_SHADOW:
        SetRoomMonFromTier(0, CONNECTOR_TIER_STRONG);
        break;
    case CONNECTOR_ROOM_RARE_SHADOW:
        SetRoomMonFromTier(0, CONNECTOR_TIER_RARE);
        break;
    case CONNECTOR_ROOM_DARK_TRAINER:
        sRun.roomTrainer = RandomUniform(RNG_NONE, 0, ARRAY_COUNT(sConnectorDarkTrainers) - 1);
        SetRoomMonFromTier(0, CONNECTOR_TIER_NORMAL);
        break;
    case CONNECTOR_ROOM_STRONG_TRAINER:
        sRun.roomTrainer = RandomUniform(RNG_NONE, 0, ARRAY_COUNT(sConnectorStrongTrainers) - 1);
        SetRoomMonFromTier(0, CONNECTOR_TIER_STRONG);
        SetRoomMonFromTier(1, CONNECTOR_TIER_NORMAL);
        break;
    case CONNECTOR_ROOM_MIDBOSS:
        SetRoomMonFromSpecies(0, PickUnlocked(sConnectorMidbossPool, ARRAY_COUNT(sConnectorMidbossPool)), TRUE);
        break;
    case CONNECTOR_ROOM_BOSS:
        SetRoomMonFromSpecies(0, PickUnlocked(sConnectorBossPool, ARRAY_COUNT(sConnectorBossPool)), FALSE);
        break;
    }
}

static u32 RollRoomType(u32 branch)
{
    u32 total = 0, pick;

    for (u32 i = 0; i < ARRAY_COUNT(sConnectorRoomWeights); i++)
    {
        if (branch >= sConnectorRoomWeights[i].minBranch)
            total += sConnectorRoomWeights[i].weight;
    }
    pick = RandomUniform(RNG_NONE, 0, total - 1);
    for (u32 i = 0; i < ARRAY_COUNT(sConnectorRoomWeights); i++)
    {
        if (branch < sConnectorRoomWeights[i].minBranch)
            continue;
        if (pick < sConnectorRoomWeights[i].weight)
            return sConnectorRoomWeights[i].roomType;
        pick -= sConnectorRoomWeights[i].weight;
    }
    return CONNECTOR_ROOM_SHADOW;
}

static u32 RollHint(u32 roomType)
{
    u32 count = 0;
    while (count < ARRAY_COUNT(sConnectorRoomHints[0]) && sConnectorRoomHints[roomType][count] != CONNECTOR_HINT_COUNT)
        count++;
    if (count == 0)
        return CONNECTOR_HINT_NOTHING;
    return sConnectorRoomHints[roomType][RandomUniform(RNG_NONE, 0, count - 1)];
}

// ---- specials: challenge ----------------------------------------------------------------------------------------------

// VAR_0x8004 and VAR_0x8005: the party slots of the Pokemon the player brings (VAR_0x8005 PARTY_SIZE: only one; up to
// BATTLE_CONNECTOR_BRING_COUNT). Anything that can be in the party may come (no species rules: this is not a Battle
// Frontier facility). Returns FALSE if they cannot.
// Before a run: TRUE if a Pokemon taken home at the end will find a place (the party as it is now, or the PC). Nothing
// can change the party or the PC during the run, so this holds until the results.
u16 BattleConnector_HasRoomToTakeHome(void)
{
    return CountPartyMons() < PARTY_SIZE || !IsPokemonStorageFull();
}

u16 BattleConnector_StartChallenge(void)
{
    u16 slots[2] = { gSpecialVar_0x8004, gSpecialVar_0x8005 };
    u32 count = (BATTLE_CONNECTOR_BRING_COUNT >= 2 && slots[1] < PARTY_SIZE) ? 2 : 1;

    if (sRun.active || !BattleConnector_HasRoomToTakeHome())
        return FALSE;   // a run already going on would lose the party it keeps
    if (slots[0] >= PARTY_SIZE || !IsMonPresent(&gParties[B_TRAINER_PLAYER][slots[0]]))
        return FALSE;
    if (count == 2 && (slots[1] == slots[0] || !IsMonPresent(&gParties[B_TRAINER_PLAYER][slots[1]])))
        return FALSE;

    ClearRun(CONNECTOR_RETURN_NONE);
    memcpy(sRun.savedParty, gParties[B_TRAINER_PLAYER], sizeof(sRun.savedParty));
    ZeroPlayerPartyMons();
    for (u32 i = 0; i < count; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        *mon = sRun.savedParty[slots[i]];
        HealPokemon(mon);
        // The rooms' levels follow the stronger Pokemon brought
        sRun.baseLevel = max(sRun.baseLevel, (u8)GetMonData(mon, MON_DATA_LEVEL));
    }
    CalculatePlayerPartyCount();
    sRun.active = TRUE;
    return TRUE;
}

// The three paths of the branch the player is at, rolled once per branch
void BattleConnector_PrepareBranch(void)
{
    if (!sRun.active || sRun.pathsRolledFor == sRun.branch + 1)
        return;
    for (u32 path = 0; path < CONNECTOR_PATH_COUNT; path++)
    {
        sRun.pathRoomTypes[path] = RollRoomType(sRun.branch);
        sRun.pathHints[path] = RollHint(sRun.pathRoomTypes[path]);
    }
    sRun.pathsRolledFor = sRun.branch + 1;
}

// VAR_0x8004: a path. STR_VAR_1: what the player senses from it.
void BattleConnector_BufferPathHint(void)
{
    u32 path = min(gSpecialVar_0x8004, (u16)(CONNECTOR_PATH_COUNT - 1));

    BattleConnector_PrepareBranch();
    if (sRun.preciseHints)
        StringCopy(gStringVar1, sConnectorPreciseHintTexts[sRun.pathRoomTypes[path]]);
    else
        StringCopy(gStringVar1, sConnectorHintTexts[sRun.pathHints[path]]);
}

// VAR_0x8004: the path taken
void BattleConnector_EnterPath(void)
{
    u32 path = min(gSpecialVar_0x8004, (u16)(CONNECTOR_PATH_COUNT - 1));

    BattleConnector_PrepareBranch();
    SetUpRoom(sRun.pathRoomTypes[path]);
    sRun.branch++;
    sRun.pathsRolledFor = 0;
    sRun.preciseHints = FALSE;
}

u16 BattleConnector_GetRoomType(void)
{
    return sRun.roomType;
}

// The room's object: its Shadow Pokemon, the person in it, or what it holds. FLAG_TEMP_11 hides it.
void BattleConnector_SetUpRoomObjects(void)
{
    u16 graphicsId = OBJ_EVENT_GFX_ITEM_BALL;

    FlagClear(FLAG_TEMP_11);
    if (IsShadowRoom(sRun.roomType))
    {
        // Its overworld sprite (a species without one shows as a Poke Ball)
        if (SpeciesToGraphicsInfo(sRun.roomSpecies[0], FALSE, FALSE) != NULL)
            graphicsId = GetGraphicsIdForMon(sRun.roomSpecies[0], FALSE, FALSE);
    }
    else if (IsTrainerRoom(sRun.roomType))
        graphicsId = GetRoomTrainer()->graphicsId;
    else if (sRun.roomType == CONNECTOR_ROOM_HEAL)
        graphicsId = OBJ_EVENT_GFX_NURSE;
    else if (sRun.roomType == CONNECTOR_ROOM_CALM)
        graphicsId = OBJ_EVENT_GFX_OLD_WOMAN;
    VarSet(VAR_OBJ_GFX_ID_0, graphicsId);
}

// STR_VAR_1: the room's Shadow Pokemon. gStringVar4: a trainer's opening words.
void BattleConnector_BufferRoomText(void)
{
    StringCopy(gStringVar1, GetSpeciesName(sRun.roomSpecies[0]));
    if (IsTrainerRoom(sRun.roomType))
        StringExpandPlaceholders(gStringVar4, GetRoomTrainer()->intro);
}

void BattleConnector_BufferTrainerAfterText(void)
{
    StringExpandPlaceholders(gStringVar4, IsTrainerRoom(sRun.roomType) ? GetRoomTrainer()->after : gText_EmptyString2);
}

// ---- battles ----------------------------------------------------------------------------------------------------------

// The room's battle; the script waits (waitstate) and gets CONNECTOR_BATTLE_WON / LOST in VAR_RESULT. Losing does not
// white out: the script ends the run.
void BattleConnector_StartRoomBattle(void)
{
    u16 trainerId = TRAINER_NONE;
    const u8 *defeatText = NULL;

    ZeroEnemyPartyMons();
    sRun.protectCursor = 0;
    CreateRoomShadowMon(&gParties[B_TRAINER_OPPONENT_A][0], 0, GetRoomLevel(sRun.roomType));
    if (sRun.roomSpecies[1] != SPECIES_NONE)
        CreateRoomShadowMon(&gParties[B_TRAINER_OPPONENT_A][1], 1, GetRoomLevel(sRun.roomType));
    CalculateEnemyPartyCount();
    if (IsTrainerRoom(sRun.roomType))
    {
        trainerId = GetRoomTrainer()->trainerId;
        defeatText = GetRoomTrainer()->defeat;
    }

    // Dynamax is allowed in the facility's battles (a Dynamax Band is still needed)
    sDynamaxFlagWasSet = FlagGet(B_FLAG_DYNAMAX_BATTLE);
    FlagSet(B_FLAG_DYNAMAX_BATTLE);
    sRun.battleSong = GetRoomSong(sRun.roomType);
    sInConnectorBattle = TRUE;
    BattleSetup_StartBattleConnectorBattle(trainerId, defeatText, sRun.battleSong, CB2_EndBattleConnectorBattle);
}

static void CB2_EndBattleConnectorBattle(void)
{
    sInConnectorBattle = FALSE;
    if (!sDynamaxFlagWasSet)
        FlagClear(B_FLAG_DYNAMAX_BATTLE);
    gSpecialVar_Result = (gBattleOutcome == B_OUTCOME_WON) ? CONNECTOR_BATTLE_WON : CONNECTOR_BATTLE_LOST;
    SyncPartyToCandidates();
    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

// ---- protecting -------------------------------------------------------------------------------------------------------

// The next defeated Shadow Pokemon of the battle to offer for protection (declined, it is not offered again): TRUE and
// STR_VAR_1 its name, or FALSE
u16 BattleConnector_BufferDefeatedShadowMon(void)
{
    sRun.offeredSlot = NO_SLOT;
    while (sRun.protectCursor < PARTY_SIZE)
    {
        u32 slot = sRun.protectCursor++;
        struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][slot];
        if (IsMonPresent(mon) && IsShadowPokemon(mon))
        {
            sRun.offeredSlot = slot;
            StringCopy(gStringVar1, GetSpeciesName(GetMonData(mon, MON_DATA_SPECIES)));
            return TRUE;
        }
    }
    return FALSE;
}

// Protects the Shadow Pokemon BattleConnector_BufferDefeatedShadowMon offered: it is kept (healed, the player's own
// for the run) in the list, and joins the party if there is room.
u16 BattleConnector_ProtectDefeatedMon(void)
{
    struct Pokemon *candidate;
    u32 slot = sRun.offeredSlot;

    if (slot >= PARTY_SIZE || !IsShadowPokemon(&gParties[B_TRAINER_OPPONENT_A][slot]))
        return CONNECTOR_PROTECT_NONE;
    sRun.offeredSlot = NO_SLOT;
    if (sRun.candidateCount >= BATTLE_CONNECTOR_MAX_CANDIDATES)
        return CONNECTOR_PROTECT_LIST_FULL;

    candidate = &sRun.candidates[sRun.candidateCount];
    *candidate = gParties[B_TRAINER_OPPONENT_A][slot];
    SetMonData(candidate, MON_DATA_OT_NAME, gSaveBlock2Ptr->playerName);
    SetMonData(candidate, MON_DATA_OT_GENDER, &gSaveBlock2Ptr->playerGender);
    SetMonData(candidate, MON_DATA_OT_ID, gSaveBlock2Ptr->playerTrainerId);
    SetShadowPokemonHyperMode(candidate, FALSE);
    SetShadowPokemonReverseMode(candidate, FALSE);
    HealPokemon(candidate);
    sRun.lastProtected = sRun.candidateCount++;
    StringCopy(gStringVar1, GetSpeciesName(GetMonData(candidate, MON_DATA_SPECIES)));

    if (CountPartyMons() >= BATTLE_CONNECTOR_PARTY_SIZE)
        return CONNECTOR_PROTECT_PARTY_FULL;
    gParties[B_TRAINER_PLAYER][CountPartyMons()] = *candidate;
    CalculatePlayerPartyCount();
    return CONNECTOR_PROTECT_JOINED;
}

// VAR_0x8004: the party slot the last protected Pokemon takes. The one it replaces keeps its place in the list (or, the
// Pokemon brought, comes back at the end anyway). Returns FALSE if nothing was swapped.
u16 BattleConnector_SwapIntoParty(void)
{
    u32 slot = gSpecialVar_0x8004;

    if (slot >= PARTY_SIZE || !IsMonPresent(&gParties[B_TRAINER_PLAYER][slot]) || sRun.lastProtected >= sRun.candidateCount)
        return FALSE;
    SyncPartyToCandidates();
    gParties[B_TRAINER_PLAYER][slot] = sRun.candidates[sRun.lastProtected];
    CalculatePlayerPartyCount();
    StringCopy(gStringVar1, GetSpeciesName(GetMonData(&gParties[B_TRAINER_PLAYER][slot], MON_DATA_SPECIES)));
    return TRUE;
}

// ---- other rooms ------------------------------------------------------------------------------------------------------

void BattleConnector_HealParty(void)
{
    HealPlayerParty();
    SyncPartyToCandidates();
}

// A scent that opens hearts: each Shadow Pokemon in the party clears one more bar and calms down from Hyper and
// Reverse Mode, and the next branch tells exactly what is behind each path. Returns how many Shadow Pokemon it reached.
u16 BattleConnector_CalmParty(void)
{
    u32 count = 0;

    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (!IsMonPresent(mon) || !IsShadowPokemon(mon))
            continue;
        AddShadowHeartGauge(mon, 1);
        SetShadowPokemonHyperMode(mon, FALSE);
        SetShadowPokemonReverseMode(mon, FALSE);
        count++;
    }
    sRun.preciseHints = TRUE;
    SyncPartyToCandidates();
    return count;
}

// VAR_0x8004: the item the room holds
void BattleConnector_PickRoomItem(void)
{
    gSpecialVar_0x8004 = sConnectorRoomItems[RandomUniform(RNG_NONE, 0, ARRAY_COUNT(sConnectorRoomItems) - 1)];
}

// Leaving a cleared room: the next branch, or the mid-boss / the boss room (set up here), or the results
u16 BattleConnector_LeaveRoom(void)
{
    if (sRun.roomType == CONNECTOR_ROOM_BOSS)
    {
        sRun.returnState = CONNECTOR_RETURN_CLEARED;
        return CONNECTOR_NEXT_RESULTS;
    }
    if (BATTLE_CONNECTOR_MIDBOSS_AFTER != 0 && !sRun.midbossDone && sRun.branch >= BATTLE_CONNECTOR_MIDBOSS_AFTER)
    {
        sRun.midbossDone = TRUE;
        SetUpRoom(CONNECTOR_ROOM_MIDBOSS);
        return CONNECTOR_NEXT_ROOM;
    }
    if (sRun.branch >= BATTLE_CONNECTOR_BRANCHES)
    {
        SetUpRoom(CONNECTOR_ROOM_BOSS);
        return CONNECTOR_NEXT_ROOM;
    }
    return CONNECTOR_NEXT_BRANCH;
}

// STR_VAR_1: the room number, STR_VAR_2: how many Pokemon are protected
void BattleConnector_BufferStatus(void)
{
    ConvertIntToDecimalStringN(gStringVar1, sRun.branch + 1, STR_CONV_MODE_LEFT_ALIGN, 2);
    ConvertIntToDecimalStringN(gStringVar2, sRun.candidateCount, STR_CONV_MODE_LEFT_ALIGN, 1);
}

// ---- the end ----------------------------------------------------------------------------------------------------------

// Losing or retiring: the party comes back as it was and everything of the run is thrown away
static void EndChallenge(u32 returnState)
{
    if (sRun.active)
        RestorePlayerParty();
    ClearRun(returnState);
}

void BattleConnector_FailChallenge(void)
{
    EndChallenge(CONNECTOR_RETURN_FAILED);
}

void BattleConnector_RetireChallenge(void)
{
    EndChallenge(CONNECTOR_RETURN_RETIRED);
}

// Back at the reception: why (once; it clears itself unless the results are still to come)
u16 BattleConnector_GetReturnState(void)
{
    u32 state = sRun.returnState;
    if (state != CONNECTOR_RETURN_CLEARED)
        sRun.returnState = CONNECTOR_RETURN_NONE;
    return state;
}

u16 BattleConnector_GetCandidateCount(void)
{
    return sRun.active ? sRun.candidateCount : 0;
}

// The party menu shows the protected Pokemon to choose from (the party before the run waits in sRun)
void BattleConnector_ShowCandidates(void)
{
    SyncPartyToCandidates();
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        if (i < sRun.candidateCount)
            gParties[B_TRAINER_PLAYER][i] = sRun.candidates[i];
        else
            ZeroMonData(&gParties[B_TRAINER_PLAYER][i]);
    }
    CalculatePlayerPartyCount();
}

static const u8 *const sConnectorStatNames[NUM_STATS] =
{
    COMPOUND_STRING("{JPN}HP"), COMPOUND_STRING("{JPN}こうげき"), COMPOUND_STRING("{JPN}ぼうぎょ"),
    COMPOUND_STRING("{JPN}すばやさ"), COMPOUND_STRING("{JPN}とくこう"), COMPOUND_STRING("{JPN}とくぼう"),
};

// VAR_0x8004: a candidate. STR_VAR_1: its name; gStringVar4: what the player should know before taking it home
// (species, gender, nature, ability, heart gauge, its perfect IVs, its purification move).
void BattleConnector_BufferCandidateDetails(void)
{
    struct Pokemon *mon;
    u8 *end;
    u32 gender, bars, perfect = 0;
    enum Move move;

    if (gSpecialVar_0x8004 >= sRun.candidateCount)
    {
        gStringVar4[0] = EOS;
        return;
    }
    mon = &sRun.candidates[gSpecialVar_0x8004];
    StringCopy(gStringVar1, GetSpeciesName(GetMonData(mon, MON_DATA_SPECIES)));

    end = StringCopy(gStringVar4, gStringVar1);
    gender = GetMonGender(mon);
    end = StringCopy(end, gender == MON_MALE ? COMPOUND_STRING("{JPN} ♂") : gender == MON_FEMALE ? COMPOUND_STRING("{JPN} ♀") : COMPOUND_STRING("{JPN}"));
    end = StringCopy(end, COMPOUND_STRING("{JPN}\n"));
    end = StringCopy(end, gNaturesInfo[GetNature(mon)].name);
    end = StringCopy(end, COMPOUND_STRING("{JPN} / "));
    end = StringCopy(end, gAbilitiesInfo[GetMonAbility(mon)].name);
    end = StringCopy(end, COMPOUND_STRING("{JPN}\p"));

    bars = GetShadowHeartGauge(mon);
    end = StringCopy(end, COMPOUND_STRING("{JPN}ハートゲージ のこり "));
    end = ConvertIntToDecimalStringN(end, SHADOW_GAUGE_BARS - bars, STR_CONV_MODE_LEFT_ALIGN, 1);
    end = StringCopy(end, COMPOUND_STRING("{JPN}/5\n31:"));
    for (u32 stat = 0; stat < NUM_STATS; stat++)
    {
        if (GetMonData(mon, MON_DATA_HP_IV + stat) != MAX_PER_STAT_IVS)
            continue;
        if (perfect++ != 0)
            end = StringCopy(end, COMPOUND_STRING("{JPN}・"));
        end = StringCopy(end, sConnectorStatNames[stat]);
    }

    move = GetShadowPurifyMove(mon);
    if (move != MOVE_NONE)
    {
        end = StringCopy(end, COMPOUND_STRING("{JPN}\pリライブすると とくべつな わざ\n"));
        end = StringCopy(end, GetMoveName(move));
        end = StringCopy(end, COMPOUND_STRING("{JPN}を おぼえるようだ"));
    }
}

// VAR_0x8004: the candidate the player takes home. The party comes back, then the Pokemon joins it (or the PC);
// the other candidates are thrown away. Returns MON_GIVEN_TO_PARTY / MON_GIVEN_TO_PC / MON_CANT_GIVE; STR_VAR_1 is
// its name.
u16 BattleConnector_TakeHomeCandidate(void)
{
    struct Pokemon mon;
    u32 result;

    if (!sRun.active || gSpecialVar_0x8004 >= sRun.candidateCount)
        return MON_CANT_GIVE;
    // Nowhere to put it (the start of the run checks for room, so this should not happen): nothing changes, and the
    // results say so instead of claiming it came home
    {
        u32 partyCount = 0;
        for (u32 i = 0; i < PARTY_SIZE; i++)
            partyCount += IsMonPresent(&sRun.savedParty[i]);
        if (partyCount >= PARTY_SIZE && IsPokemonStorageFull())
            return MON_CANT_GIVE;
    }

    SyncPartyToCandidates();
    mon = sRun.candidates[gSpecialVar_0x8004];
    RestorePlayerParty();
    ClearRun(CONNECTOR_RETURN_NONE);

    SetShadowPokemonHyperMode(&mon, FALSE);
    SetShadowPokemonReverseMode(&mon, FALSE);
    HealPokemon(&mon);
    StringCopy(gStringVar1, GetSpeciesName(GetMonData(&mon, MON_DATA_SPECIES)));
    result = GiveCapturedMonToPlayer(&mon);
    if (result != MON_CANT_GIVE)
    {
        GetSetPokedexFlag(SpeciesToNationalPokedexNum(GetMonData(&mon, MON_DATA_SPECIES)), FLAG_SET_SEEN);
        GetSetPokedexFlag(SpeciesToNationalPokedexNum(GetMonData(&mon, MON_DATA_SPECIES)), FLAG_SET_CAUGHT);
    }
    if (VarGet(VAR_BATTLE_CONNECTOR_CLEARS) < 0xFFFF)
        VarSet(VAR_BATTLE_CONNECTOR_CLEARS, VarGet(VAR_BATTLE_CONNECTOR_CLEARS) + 1);
    return result;
}

// A cleared run where the player takes no one home
void BattleConnector_TakeNothingHome(void)
{
    bool32 cleared = sRun.active && sRun.returnState == CONNECTOR_RETURN_CLEARED;

    EndChallenge(CONNECTOR_RETURN_NONE);
    if (cleared && VarGet(VAR_BATTLE_CONNECTOR_CLEARS) < 0xFFFF)
        VarSet(VAR_BATTLE_CONNECTOR_CLEARS, VarGet(VAR_BATTLE_CONNECTOR_CLEARS) + 1);
}

#if TESTING
void BattleConnector_Test_SetBattleActive(bool32 active, u32 roomType)
{
    sInConnectorBattle = active;
    sRun.battleSong = GetRoomSong(roomType);
}

u32 BattleConnector_Test_GetPathRoomType(u32 path)
{
    return sRun.pathRoomTypes[path];
}

u32 BattleConnector_Test_GetCandidateCount(void)
{
    return sRun.candidateCount;
}

struct Pokemon *BattleConnector_Test_GetCandidate(u32 index)
{
    return &sRun.candidates[index];
}

u16 BattleConnector_Test_GetRoomSpecies(void)
{
    return sRun.roomSpecies[0];
}

u16 BattleConnector_Test_GetRoomSet(void)
{
    return sRun.roomSets[0];
}

void BattleConnector_Test_SetUpRoom(u32 roomType)
{
    SetUpRoom(roomType);
}

// As BattleConnector_StartRoomBattle does: a new battle's Pokemon are offered from the first
void BattleConnector_Test_NewBattle(void)
{
    sRun.protectCursor = 0;
}
#endif
