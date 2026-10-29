#include "global.h"
#include "event_data.h"
#include "frontier_story.h"
#include "party_bath_expansion.h"
#include "random.h"
#include "script.h"
#include "string_util.h"
#include "constants/frontier_guest.h"
#include "constants/maps.h"

static const struct
{
    u16 trainer;
    u16 built;
    const u8 *name;
} sExpansions[PARTY_BATH_EXPANSION_COUNT] =
{
    {TRAINER_FRONTIER_STEVEN, FLAG_BATH_ROCK_BUILT, COMPOUND_STRING("{JPN}岩盤浴")},
    {TRAINER_FRONTIER_WALLACE, FLAG_BATH_WATERFALL_BUILT, COMPOUND_STRING("{JPN}滝の湯")},
    {TRAINER_FRONTIER_MAXIE, FLAG_BATH_SAUNA_BUILT, COMPOUND_STRING("{JPN}サウナ")},
    {TRAINER_FRONTIER_ARCHIE, FLAG_BATH_DEEP_BUILT, COMPOUND_STRING("{JPN}深いお風呂")},
};

extern const u8 PartyBathExpansion_EventScript_Call[];
static EWRAM_DATA u8 sCallSteps = 0;

bool32 PartyBath_IsMap(void)
{
    u32 map = (gSaveBlock1Ptr->location.mapGroup << 8) | gSaveBlock1Ptr->location.mapNum;
    return map == MAP_PARTYBATHTIME || map == MAP_PARTY_BATH_ROCK
        || map == MAP_PARTY_BATH_WATERFALL || map == MAP_PARTY_BATH_SAUNA
        || map == MAP_PARTY_BATH_DEEP || map == MAP_PARTY_BATH_UNDERWATER;
}

bool32 PartyBath_UsesWaterPalette(void)
{
    u32 map = (gSaveBlock1Ptr->location.mapGroup << 8) | gSaveBlock1Ptr->location.mapNum;
    return map == MAP_PARTYBATHTIME || map == MAP_PARTY_BATH_WATERFALL || map == MAP_PARTY_BATH_DEEP;
}

u32 PartyBath_GetGain(void)
{
    u32 gain = 1;
    for (u32 i = 0; i < ARRAY_COUNT(sExpansions); i++)
        gain += FlagGet(sExpansions[i].built);
    return gain;
}

u32 PartyBath_GetPendingExpansionCall(void)
{
    for (u32 i = 0; i < ARRAY_COUNT(sExpansions); i++)
        if (!FlagGet(sExpansions[i].built) && FrontierStory_IsGuestRescued(sExpansions[i].trainer))
            return i;
    return PARTY_BATH_EXPANSION_COUNT;
}

bool32 PartyBath_TryStartExpansionCall(void)
{
    u32 map = (gSaveBlock1Ptr->location.mapGroup << 8) | gSaveBlock1Ptr->location.mapNum;
    // Calls wait for a quiet walk in the hub or camp, never interrupting a cure or challenge.
    if ((map != MAP_ULTRA_FRONTIE_HUB && map != MAP_PARTY_ROAMER_PARK)
     || !FlagGet(FLAG_SYS_POKENAV_GET) || PartyBath_GetPendingExpansionCall() == PARTY_BATH_EXPANSION_COUNT)
    {
        sCallSteps = 0;
        return FALSE;
    }
    if (++sCallSteps < PARTY_BATH_CALL_STEPS)
        return FALSE;
    sCallSteps = 0;
    if (RandomUniform(RNG_NONE, 0, PARTY_BATH_CALL_CHANCE - 1) != 0)
        return FALSE;
    ScriptContext_SetupScript(PartyBathExpansion_EventScript_Call);
    return TRUE;
}

void PartyBath_GetPendingCall(void)
{
    gSpecialVar_Result = PartyBath_GetPendingExpansionCall();
}

void PartyBath_CompleteCall(void)
{
    u32 i = gSpecialVar_0x8004;
    gSpecialVar_Result = FALSE;
    if (i >= ARRAY_COUNT(sExpansions) || FlagGet(sExpansions[i].built)
     || !FrontierStory_IsGuestRescued(sExpansions[i].trainer))
        return;
    FlagSet(sExpansions[i].built);
    StringCopy(gStringVar1, sExpansions[i].name);
    ConvertIntToDecimalStringN(gStringVar2, PartyBath_GetGain(), STR_CONV_MODE_LEFT_ALIGN, 1);
    gSpecialVar_Result = TRUE;
}

void PartyBath_CheckExpansion(void)
{
    u32 i = gSpecialVar_0x8004;
    gSpecialVar_Result = 0;
    if (i >= ARRAY_COUNT(sExpansions))
        return;
    StringCopy(gStringVar1, sExpansions[i].name);
    if (FlagGet(sExpansions[i].built))
        gSpecialVar_Result = TRUE;
}
