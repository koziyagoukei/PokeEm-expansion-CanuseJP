#include "global.h"
#include "data.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "follower_helper.h"
#include "overworld.h"
#include "palette.h"
#include "party_roamer_park.h"
#include "party_bath_expansion.h"
#include "pokemon.h"
#include "random.h"
#include "shadow_pokemon.h"
#include "string_util.h"
#include "task.h"
#include "constants/battle.h"
#include "constants/characters.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/maps.h"
#include "constants/map_groups.h"
#include "constants/pokemon.h"
#include "constants/rgb.h"
#include "constants/shadow_pokemon.h"
#include "constants/trainer_types.h"

#define PARTY_ROAMER_LOCAL_ID_START 1
#define PARTY_ROAMER_PARK_ENTRY_X 13
#define PARTY_ROAMER_PARK_ENTRY_Y 19

static const u16 sPartyRoamerObjectFlags[PARTY_SIZE] =
{
    FLAG_TEMP_11,
    FLAG_TEMP_12,
    FLAG_TEMP_13,
    FLAG_TEMP_14,
    FLAG_TEMP_15,
    FLAG_TEMP_16,
};

static const u8 sDefaultPartyRoamerText[] = _("{JPN}{STR_VAR_1}は のんびりしている");

static const u8 *const sPartyBathNames[PARTY_BATH_COUNT] =
{
    [PARTY_BATH_FUEN] = COMPOUND_STRING("{JPN}フエンかけながし"),
    [PARTY_BATH_SCOVILLAIN] = COMPOUND_STRING("{JPN}スコヴィランのかおり"),
    [PARTY_BATH_SNOW] = COMPOUND_STRING("{JPN}ゆきみおんせんのもと"),
    [PARTY_BATH_HERBAL] = COMPOUND_STRING("{JPN}カンポーやくゆ"),
    [PARTY_BATH_GRACIDEA] = COMPOUND_STRING("{JPN}グラシデアエッセンス"),
    [PARTY_BATH_SALT] = COMPOUND_STRING("{JPN}ソルティジオヅム"),
};

static const u8 *const sPartyBathEffects[PARTY_BATH_COUNT] =
{
    [PARTY_BATH_FUEN] = COMPOUND_STRING("{JPN}なつきど"),
    [PARTY_BATH_SCOVILLAIN] = COMPOUND_STRING("{JPN}かっこよさと けづや"),
    [PARTY_BATH_SNOW] = COMPOUND_STRING("{JPN}たくましさと けづや"),
    [PARTY_BATH_HERBAL] = COMPOUND_STRING("{JPN}かしこさと けづや"),
    [PARTY_BATH_GRACIDEA] = COMPOUND_STRING("{JPN}かわいさと けづや"),
    [PARTY_BATH_SALT] = COMPOUND_STRING("{JPN}うつくしさと けづや"),
};

static const s32 sPartyBathStats[PARTY_BATH_COUNT] =
{
    [PARTY_BATH_FUEN] = MON_DATA_FRIENDSHIP,
    [PARTY_BATH_SCOVILLAIN] = MON_DATA_COOL,
    [PARTY_BATH_SNOW] = MON_DATA_TOUGH,
    [PARTY_BATH_HERBAL] = MON_DATA_SMART,
    [PARTY_BATH_GRACIDEA] = MON_DATA_CUTE,
    [PARTY_BATH_SALT] = MON_DATA_BEAUTY,
};

// Bath water uses colors 9 and 10 of bank 8; rocks and painting frames do not.
static const u16 sPartyBathWaterColors[PARTY_BATH_COUNT][2] =
{
    [PARTY_BATH_SCOVILLAIN] = {RGB(29, 13, 13), RGB(26, 10, 10)},
    [PARTY_BATH_SNOW] = {RGB(30, 27, 13), RGB(27, 24, 10)},
    [PARTY_BATH_HERBAL] = {RGB(15, 27, 17), RGB(12, 24, 14)},
    [PARTY_BATH_GRACIDEA] = {RGB(30, 19, 25), RGB(27, 16, 22)},
    [PARTY_BATH_SALT] = {RGB(13, 19, 30), RGB(10, 16, 27)},
};

extern const u8 PartyRoamerPark_EventScript_Talk[];

static void FieldCallback_EnterPartyRoamerPark(void);
static void Task_WaitToEnterPartyRoamerPark(u8 taskId);
static void Task_PartyBathTimeEffects(u8 taskId);
static bool8 IsInPartyBathTimeMap(void);

static bool8 IsPartyRoamerLocalId(u16 localId)
{
    return localId >= PARTY_ROAMER_LOCAL_ID_START
        && localId < PARTY_ROAMER_LOCAL_ID_START + PARTY_SIZE;
}

static u8 GetPartySlotFromRoamerLocalId(u16 localId)
{
    // Partybathtime places OBJ_EVENT_GFX_VAR_0 last, so local IDs map to 1, 2, 3, 4, 5, 0.
    if (IsInPartyBathTimeMap())
        return localId % PARTY_SIZE;

    return localId - PARTY_ROAMER_LOCAL_ID_START;
}

static bool8 ShouldSpawnPartyMon(struct Pokemon *mon, enum Species *species, bool32 *shiny, bool32 *female)
{
    enum Species monSpecies = GetMonData(mon, MON_DATA_SPECIES);

    if (monSpecies == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
        return FALSE;

    *species = monSpecies;
    *shiny = IsMonShiny(mon);
    *female = GetMonGender(mon) == MON_FEMALE;

    return SpeciesToGraphicsInfo(*species, *shiny, *female) != NULL;
}

static enum FollowerEmotion ChoosePartyRoamerEmotion(struct Pokemon *mon)
{
    u32 hp = GetMonData(mon, MON_DATA_HP);
    u32 maxHp = GetMonData(mon, MON_DATA_MAX_HP);
    u32 friendship = GetMonData(mon, MON_DATA_FRIENDSHIP);
    u32 status = GetMonData(mon, MON_DATA_STATUS);

    if (status & STATUS1_PSN_ANY)
        return FOLLOWER_EMOTION_POISONED;

    if (maxHp != 0 && hp != 0 && hp * 100 < maxHp * 25)
        return FOLLOWER_EMOTION_SAD;

    if (friendship > 170)
    {
        static const enum FollowerEmotion sHighFriendshipEmotions[] =
        {
            FOLLOWER_EMOTION_HAPPY,
            FOLLOWER_EMOTION_LOVE,
            FOLLOWER_EMOTION_MUSIC,
            FOLLOWER_EMOTION_CURIOUS,
        };

        return sHighFriendshipEmotions[Random() % ARRAY_COUNT(sHighFriendshipEmotions)];
    }

    if (friendship > 80)
    {
        static const enum FollowerEmotion sMidFriendshipEmotions[] =
        {
            FOLLOWER_EMOTION_HAPPY,
            FOLLOWER_EMOTION_NEUTRAL,
            FOLLOWER_EMOTION_SURPRISE,
            FOLLOWER_EMOTION_CURIOUS,
            FOLLOWER_EMOTION_MUSIC,
        };

        return sMidFriendshipEmotions[Random() % ARRAY_COUNT(sMidFriendshipEmotions)];
    }

    {
        static const enum FollowerEmotion sLowFriendshipEmotions[] =
        {
            FOLLOWER_EMOTION_NEUTRAL,
            FOLLOWER_EMOTION_PENSIVE,
            FOLLOWER_EMOTION_UPSET,
            FOLLOWER_EMOTION_ANGRY,
        };

        return sLowFriendshipEmotions[Random() % ARRAY_COUNT(sLowFriendshipEmotions)];
    }
}

static const u8 *GetRandomPartyRoamerText(struct Pokemon *mon)
{
    enum FollowerEmotion emotion = ChoosePartyRoamerEmotion(mon);
    const struct FollowerMessagePool *pool = &gFollowerBasicMessages[emotion];

    if (pool->length == 0)
        return sDefaultPartyRoamerText;

    return pool->messages[Random() % pool->length].text;
}

bool8 FieldCallback_PrepareEnterPartyRoamerPark(void)
{
    RemoveFollowingPokemon();
    FadeInFromBlack();
    CreateTask(Task_WaitToEnterPartyRoamerPark, 8);
    return TRUE;
}

static void Task_WaitToEnterPartyRoamerPark(u8 taskId)
{
    if (IsWeatherNotFadingIn() == TRUE)
    {
        FieldCallback_EnterPartyRoamerPark();
        DestroyTask(taskId);
    }
}

static void FieldCallback_EnterPartyRoamerPark(void)
{
    SetWarpDestination(
        MAP_GROUP(MAP_PARTY_ROAMER_PARK),
        MAP_NUM(MAP_PARTY_ROAMER_PARK),
        WARP_ID_NONE,
        PARTY_ROAMER_PARK_ENTRY_X,
        PARTY_ROAMER_PARK_ENTRY_Y);
    SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
    FldEff_TeleportWarpOutWithPresetDestination();
}

static bool8 IsInPartyBathTimeMap(void)
{
    return PartyBath_IsMap();
}

u16 PartyBath_GetAdditive(void)
{
    u16 additive = VarGet(VAR_PARTY_BATH_ADDITIVE);
    return additive < PARTY_BATH_COUNT ? additive : PARTY_BATH_FUEN;
}

void Special_BufferPartyBathAdditive(void)
{
    u16 additive = PartyBath_GetAdditive();
    StringCopy(gStringVar1, sPartyBathNames[additive]);
    StringCopy(gStringVar2, sPartyBathEffects[additive]);
}

bool8 Special_SetPartyBathAdditive(void)
{
    // The menu lists the five additives first, followed by the default bath.
    if (gSpecialVar_0x8004 >= PARTY_BATH_COUNT)
        return FALSE;
    VarSet(VAR_PARTY_BATH_ADDITIVE, (gSpecialVar_0x8004 + 1) % PARTY_BATH_COUNT);
    Special_BufferPartyBathAdditive();
    return TRUE;
}

void PartyBath_ApplyWaterPalette(bool8 skipFaded)
{
    const u16 *colors;
    u16 additive;
    if (!PartyBath_UsesWaterPalette())
        return;
    additive = PartyBath_GetAdditive();
    colors = additive == PARTY_BATH_FUEN
        ? &gMapHeader.mapLayout->secondaryTileset->palettes[8][9]
        : sPartyBathWaterColors[additive];
    if (skipFaded)
        CpuCopy16(colors, &gPlttBufferUnfaded[BG_PLTT_ID(8) + 9], 2 * sizeof(u16));
    else
        LoadPalette(colors, BG_PLTT_ID(8) + 9, 2 * sizeof(u16));
}

void Special_PreparePartyRoamerPark(void)
{
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_PARTY_ROAMER_PARK)
        || gSaveBlock1Ptr->location.mapNum != MAP_NUM(MAP_PARTY_ROAMER_PARK))
        return;
    // Upgrade cached object templates in saves made before the plots were removed.
    for (u32 i = 0; i < OBJECT_EVENT_TEMPLATES_COUNT; i++)
        if (gSaveBlock1Ptr->objectEventTemplates[i].graphicsId == OBJ_EVENT_GFX_BERRY_TREE)
        {
            LoadObjEventTemplatesFromHeader();
            break;
        }
}

static void IncreasePartyBathStat(struct Pokemon *mon, s32 field, u8 limit, u32 amount)
{
    u32 value = GetMonData(mon, field);
    if (value < limit)
    {
        value = min(value + amount, limit);
        SetMonData(mon, field, &value);
    }
}

static void ApplyPartyBathTimeEffects(void)
{
    u8 i;
    u16 additive = PartyBath_GetAdditive();

    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];

        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(mon, MON_DATA_IS_EGG))
        {
            IncreasePartyBathStat(mon, sPartyBathStats[additive],
                                 additive == PARTY_BATH_FUEN ? MAX_FRIENDSHIP : MAX_CONDITION, PartyBath_GetGain());
            if (additive != PARTY_BATH_FUEN)
                IncreasePartyBathStat(mon, MON_DATA_SHEEN, MAX_SHEEN, 1);
        }
    }
    ShadowPokemon_OnBathTick();
}

static void Task_PartyBathTimeEffects(u8 taskId)
{
    if (!IsInPartyBathTimeMap())
    {
        DestroyTask(taskId);
        return;
    }

    if (++gTasks[taskId].data[0] >= PARTY_BATH_INTERVAL)
    {
        gTasks[taskId].data[0] = 0;
        ApplyPartyBathTimeEffects();
    }
}

void Special_StartPartyBathTimeFriendship(void)
{
    if (!IsInPartyBathTimeMap())
        return;

    if (FindTaskIdByFunc(Task_PartyBathTimeEffects) == TASK_NONE)
        CreateTask(Task_PartyBathTimeEffects, 8);
}

void Special_SpawnPartyRoamerMons(void)
{
    u8 i;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        enum Species species;
        bool32 shiny;
        bool32 female;
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];

        if (ShouldSpawnPartyMon(mon, &species, &shiny, &female))
        {
            VarSet(VAR_OBJ_GFX_ID_0 + i, GetGraphicsIdForMon(species, shiny, female));
            FlagClear(sPartyRoamerObjectFlags[i]);
        }
        else
        {
            VarSet(VAR_OBJ_GFX_ID_0 + i, OBJ_EVENT_GFX_PIKACHU);
            FlagSet(sPartyRoamerObjectFlags[i]);
        }
    }
}

#define tTimer    data[0]
#define tTries    data[1]
#define tMapNum   data[2]
#define tMapGroup data[3]

#define ROAMER_RETRY_INTERVAL 30 // frames
#define ROAMER_RETRY_COUNT    20

static bool32 IsPartyRoamerOut(u32 slot, u8 *objectEventId)
{
    // TryGetObjectEventIdByLocalIdAndMap is FALSE when it finds the object
    return !TryGetObjectEventIdByLocalIdAndMap(PARTY_ROAMER_LOCAL_ID_START + slot, gSaveBlock1Ptr->location.mapNum,
                                               gSaveBlock1Ptr->location.mapGroup, objectEventId);
}

// The Pokemon out in the camp farthest from the player that is not a Shadow Pokemon (PARTY_SIZE if none)
static u32 GetFarthestOtherPartyRoamer(void)
{
    struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    u32 best = PARTY_SIZE, bestDistance = 0;

    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        u8 objectEventId;
        u32 distance;

        if (!IsPartyRoamerOut(i, &objectEventId) || IsShadowPokemon(&gParties[B_TRAINER_PLAYER][i]))
            continue;
        distance = abs(gObjectEvents[objectEventId].currentCoords.x - player->currentCoords.x)
                 + abs(gObjectEvents[objectEventId].currentCoords.y - player->currentCoords.y);
        if (best == PARTY_SIZE || distance > bestDistance)
        {
            best = i;
            bestDistance = distance;
        }
    }
    return best;
}

// Spawns the party's Pokemon that should be out but are not. A Shadow Pokemon must come out, as it is talked to to be
// purified: if there is no room for it in the sprite tiles (a big 64x64 Pokemon such as Lugia does not fit beside five
// others), the other Pokemon farthest from the player go back until it fits. TRUE if none is missing.
static bool32 TrySpawnMissingPartyRoamers(void)
{
    bool32 allOut = TRUE;

    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        u8 objectEventId;
        u8 localId = PARTY_ROAMER_LOCAL_ID_START + i;

        if (FlagGet(sPartyRoamerObjectFlags[i]) || IsPartyRoamerOut(i, &objectEventId))
            continue;
        while (TrySpawnObjectEvent(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup) == OBJECT_EVENTS_COUNT)
        {
            u32 other = IsShadowPokemon(&gParties[B_TRAINER_PLAYER][i]) ? GetFarthestOtherPartyRoamer() : PARTY_SIZE;
            if (other == PARTY_SIZE)
            {
                allOut = FALSE;
                break;
            }
            RemoveObjectEventByLocalIdAndMap(PARTY_ROAMER_LOCAL_ID_START + other, gSaveBlock1Ptr->location.mapNum,
                                             gSaveBlock1Ptr->location.mapGroup);
        }
    }
    return allOut;
}

// Pokemon that could not come out while the map was loading are tried again for a few seconds.
static void Task_RetryPartyRoamerSpawns(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (gSaveBlock1Ptr->location.mapNum != tMapNum || gSaveBlock1Ptr->location.mapGroup != tMapGroup
     || tTries >= ROAMER_RETRY_COUNT)
    {
        DestroyTask(taskId);
        return;
    }
    if (++tTimer < ROAMER_RETRY_INTERVAL)
        return;
    tTimer = 0;
    tTries++;
    if (TrySpawnMissingPartyRoamers())
        DestroyTask(taskId);
}

static void StartPartyRoamerSpawnRetries(void)
{
    u8 taskId;

    if (FuncIsActiveTask(Task_RetryPartyRoamerSpawns) || TrySpawnMissingPartyRoamers())
        return;
    taskId = CreateTask(Task_RetryPartyRoamerSpawns, 80);
    gTasks[taskId].tMapNum = gSaveBlock1Ptr->location.mapNum;
    gTasks[taskId].tMapGroup = gSaveBlock1Ptr->location.mapGroup;
}

#undef tTimer
#undef tTries
#undef tMapNum
#undef tMapGroup

void Special_RefreshPartyRoamerMons(void)
{
    u8 i;

    Special_SpawnPartyRoamerMons();

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u8 objectEventId;
        u8 localId = PARTY_ROAMER_LOCAL_ID_START + i;

        if (FlagGet(sPartyRoamerObjectFlags[i]))
        {
            RemoveObjectEventByLocalIdAndMap(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
        }
        else if (TryGetObjectEventIdByLocalIdAndMap(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId))
        {
            TrySpawnObjectEvent(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
        }
        else
        {
            ObjectEventSetGraphicsId(&gObjectEvents[objectEventId], VarGet(VAR_OBJ_GFX_ID_0 + i));
        }
    }
    StartPartyRoamerSpawnRetries();
}

// The Pokemon talked to in the camp, as a Shadow Pokemon (SHADOW_ROAMER_*). For a Shadow Pokemon, VAR_0x8004 is its
// party slot and STR_VAR_1 its nickname. Pokemon XD purifies one with four other Pokemon round it.
u16 Special_GetPartyRoamerShadowState(void)
{
    u16 localId = gSpecialVar_LastTalked;
    struct Pokemon *mon;
    u32 slot;

    gSpecialVar_0x8004 = PARTY_SIZE;
    if (!IsPartyRoamerLocalId(localId))
        return SHADOW_ROAMER_NORMAL;
    slot = GetPartySlotFromRoamerLocalId(localId);
    mon = &gParties[B_TRAINER_PLAYER][slot];
    if (!IsShadowPokemon(mon))
        return SHADOW_ROAMER_NORMAL;

    gSpecialVar_0x8004 = slot;
    GetMonData(mon, MON_DATA_NICKNAME, gStringVar1);
    StringGet_Nickname(gStringVar1);
    if (!CanPurifyShadowPokemon(mon))
        return SHADOW_ROAMER_CLOSED;
    if (CountShadowPurifyPartners(slot) < SHADOW_PURIFY_PARTNERS)
        return SHADOW_ROAMER_NEEDS_PARTNERS;
    return SHADOW_ROAMER_CAN_PURIFY;
}

void Special_BufferPartyRoamerMessage(void)
{
    u16 localId = gSpecialVar_LastTalked;
    const u8 *text = sDefaultPartyRoamerText;

    gStringVar1[0] = EOS;

    if (IsPartyRoamerLocalId(localId))
    {
        u8 partySlot = GetPartySlotFromRoamerLocalId(localId);
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][partySlot];

        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(mon, MON_DATA_IS_EGG))
        {
            GetMonData(mon, MON_DATA_NICKNAME, gStringVar1);
            StringGet_Nickname(gStringVar1);
            text = GetRandomPartyRoamerText(mon);
        }
    }

    StringExpandPlaceholders(gStringVar4, text);
}
