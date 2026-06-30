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
#include "party_roamer_park.h"
#include "pokemon.h"
#include "random.h"
#include "string_util.h"
#include "task.h"
#include "constants/battle.h"
#include "constants/characters.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/maps.h"
#include "constants/map_groups.h"
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

static const u8 sDefaultPartyRoamerText[] = _("{STR_VAR_1} seems relaxed.");

extern const u8 PartyRoamerPark_EventScript_Talk[];

static void FieldCallback_EnterPartyRoamerPark(void);
static void Task_WaitToEnterPartyRoamerPark(u8 taskId);

static bool8 IsPartyRoamerLocalId(u16 localId)
{
    return localId >= PARTY_ROAMER_LOCAL_ID_START
        && localId < PARTY_ROAMER_LOCAL_ID_START + PARTY_SIZE;
}

static u8 GetPartySlotFromRoamerLocalId(u16 localId)
{
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
    SetDynamicWarpWithCoords(
        0,
        gSaveBlock1Ptr->location.mapGroup,
        gSaveBlock1Ptr->location.mapNum,
        WARP_ID_NONE,
        gSaveBlock1Ptr->pos.x,
        gSaveBlock1Ptr->pos.y);
    SetWarpDestination(
        MAP_GROUP(MAP_PARTY_ROAMER_PARK),
        MAP_NUM(MAP_PARTY_ROAMER_PARK),
        WARP_ID_NONE,
        PARTY_ROAMER_PARK_ENTRY_X,
        PARTY_ROAMER_PARK_ENTRY_Y);
    SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
    FldEff_TeleportWarpOutWithPresetDestination();
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
