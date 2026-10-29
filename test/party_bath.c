#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "fieldmap.h"
#include "overworld.h"
#include "palette.h"
#include "party_roamer_park.h"
#include "party_bath_expansion.h"
#include "pokemon.h"
#include "task.h"
#include "test/test.h"
#include "constants/event_objects.h"
#include "constants/maps.h"
#include "constants/metatile_labels.h"

static const s32 sConditionFields[] = {MON_DATA_COOL, MON_DATA_TOUGH, MON_DATA_SMART, MON_DATA_CUTE, MON_DATA_BEAUTY};

static void EnterBath(u16 additive)
{
    for (u32 flag = FLAG_BATH_ROCK_BUILT; flag <= FLAG_BATH_DEEP_BUILT; flag++)
        FlagClear(flag);
    ResetTasks();
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_PARTYBATHTIME);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_PARTYBATHTIME);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_PARTYBATHTIME), MAP_NUM(MAP_PARTYBATHTIME));
    VarSet(VAR_PARTY_BATH_ADDITIVE, additive);
    ZeroPlayerPartyMons();
    for (u32 i = 0; i < 3; i++)
    {
        u8 value = 10, friendship = 80, egg = i == 2;
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        CreateMonWithIVs(mon, SPECIES_PIKACHU, 50, 1234, OTID_STRUCT_PLAYER_ID, 31);
        SetMonData(mon, MON_DATA_FRIENDSHIP, &friendship);
        SetMonData(mon, MON_DATA_SHEEN, &value);
        for (u32 field = 0; field < ARRAY_COUNT(sConditionFields); field++)
            SetMonData(mon, sConditionFields[field], &value);
        SetMonData(mon, MON_DATA_IS_EGG, &egg);
    }
    Special_StartPartyBathTimeFriendship();
}

static void WaitBath(u32 frames)
{
    while (frames--)
        RunTasks();
}

TEST("Party bath: each additive changes only its condition and sheen every 300 frames")
{
    u32 additive = PARTY_BATH_FUEN;
    for (u32 i = 0; i < PARTY_BATH_COUNT; i++)
        PARAMETRIZE { additive = i; }
    EnterBath(additive);
    WaitBath(PARTY_BATH_INTERVAL - 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_FRIENDSHIP), 80);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SHEEN), 10);
    for (u32 field = 0; field < ARRAY_COUNT(sConditionFields); field++)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], sConditionFields[field]), 10);
    WaitBath(1);
    for (u32 slot = 0; slot < 3; slot++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
        EXPECT_EQ(GetMonData(mon, MON_DATA_FRIENDSHIP), 80 + (slot != 2 && additive == PARTY_BATH_FUEN));
        EXPECT_EQ(GetMonData(mon, MON_DATA_SHEEN), 10 + (slot != 2 && additive != PARTY_BATH_FUEN));
        for (u32 field = 0; field < ARRAY_COUNT(sConditionFields); field++)
            EXPECT_EQ(GetMonData(mon, sConditionFields[field]), 10 + (slot != 2 && additive == field + 1));
    }
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][3], MON_DATA_SPECIES), SPECIES_NONE);
    ResetTasks();
}

TEST("Party bath: each completed expansion adds one to the five-second stat gain")
{
    u32 count = 0, additive = 0;
    for (u32 n = 0; n <= PARTY_BATH_EXPANSION_COUNT; n++)
        for (u32 a = 0; a < PARTY_BATH_COUNT; a++)
            PARAMETRIZE { count = n; additive = a; }
    EnterBath(additive);
    for (u32 i = 0; i < count; i++)
        FlagSet(FLAG_BATH_ROCK_BUILT + i);
    EXPECT_EQ(PartyBath_GetGain(), count + 1);
    WaitBath(PARTY_BATH_INTERVAL);
    for (u32 slot = 0; slot < 3; slot++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
        u32 gain = slot == 2 ? 0 : count + 1;
        EXPECT_EQ(GetMonData(mon, MON_DATA_FRIENDSHIP), 80 + (additive == PARTY_BATH_FUEN ? gain : 0));
        for (u32 field = 0; field < ARRAY_COUNT(sConditionFields); field++)
            EXPECT_EQ(GetMonData(mon, sConditionFields[field]), 10 + (additive == field + 1 ? gain : 0));
        EXPECT_EQ(GetMonData(mon, MON_DATA_SHEEN), 10 + (slot != 2 && additive != PARTY_BATH_FUEN));
    }
    ResetTasks();
}

TEST("Party bath: five-point gains saturate instead of wrapping past 255")
{
    u32 additive = PARTY_BATH_FUEN;
    PARAMETRIZE { additive = PARTY_BATH_FUEN; }
    PARAMETRIZE { additive = PARTY_BATH_SCOVILLAIN; }
    EnterBath(additive);
    for (u32 i = 0; i < PARTY_BATH_EXPANSION_COUNT; i++)
        FlagSet(FLAG_BATH_ROCK_BUILT + i);
    u8 value = 253;
    s32 field = additive == PARTY_BATH_FUEN ? MON_DATA_FRIENDSHIP : MON_DATA_COOL;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], field, &value);
    WaitBath(2 * PARTY_BATH_INTERVAL);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], field), 255);
    ResetTasks();
}

TEST("Party bath: all expansion floors share the timer and leaving stops it")
{
    u32 map = MAP_PARTY_BATH_ROCK;
    PARAMETRIZE { map = MAP_PARTY_BATH_ROCK; }
    PARAMETRIZE { map = MAP_PARTY_BATH_WATERFALL; }
    PARAMETRIZE { map = MAP_PARTY_BATH_SAUNA; }
    PARAMETRIZE { map = MAP_PARTY_BATH_DEEP; }
    PARAMETRIZE { map = MAP_PARTY_BATH_UNDERWATER; }
    EnterBath(PARTY_BATH_FUEN);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(map);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(map);
    EXPECT(PartyBath_IsMap());
    Special_StartPartyBathTimeFriendship();
    WaitBath(PARTY_BATH_INTERVAL);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_FRIENDSHIP), 81);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_ULTRA_FRONTIE_HUB);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_ULTRA_FRONTIE_HUB);
    WaitBath(PARTY_BATH_INTERVAL);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_FRIENDSHIP), 81);
    ResetTasks();
}

TEST("Party bath: all capped stats stay at 255, and full sheen does not block a condition gain")
{
    u32 additive = 0;
    for (u32 i = 0; i < PARTY_BATH_COUNT; i++)
        PARAMETRIZE { additive = i; }
    EnterBath(additive);
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    u8 almost = 254, full = 255;
    s32 field = additive == PARTY_BATH_FUEN ? MON_DATA_FRIENDSHIP : sConditionFields[additive - 1];
    SetMonData(mon, field, &almost);
    SetMonData(mon, MON_DATA_SHEEN, &full);
    WaitBath(2 * PARTY_BATH_INTERVAL);
    EXPECT_EQ(GetMonData(mon, field), 255);
    EXPECT_EQ(GetMonData(mon, MON_DATA_SHEEN), 255);
    ResetTasks();
}

TEST("Party bath: duplicate starts, menu resume, and leaving the map do not duplicate effects")
{
    EnterBath(PARTY_BATH_FUEN);
    Special_StartPartyBathTimeFriendship();
    Special_StartPartyBathTimeFriendship();
    WaitBath(PARTY_BATH_INTERVAL);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_FRIENDSHIP), 81);
    ResetTasks();
    Special_StartPartyBathTimeFriendship();
    WaitBath(PARTY_BATH_INTERVAL);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_FRIENDSHIP), 82);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_PARTY_ROAMER_PARK);
    WaitBath(2 * PARTY_BATH_INTERVAL);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_FRIENDSHIP), 82);
    ResetTasks();
}

TEST("Party bath: menu selection maps all six entries and cancellation preserves the saved choice")
{
    for (u32 selection = 0; selection < PARTY_BATH_COUNT; selection++)
    {
        gSpecialVar_0x8004 = selection;
        EXPECT(Special_SetPartyBathAdditive());
        EXPECT_EQ(PartyBath_GetAdditive(), (selection + 1) % PARTY_BATH_COUNT);
    }
    VarSet(VAR_PARTY_BATH_ADDITIVE, PARTY_BATH_HERBAL);
    gSpecialVar_0x8004 = 127;
    EXPECT(!Special_SetPartyBathAdditive());
    EXPECT_EQ(PartyBath_GetAdditive(), PARTY_BATH_HERBAL);
    VarSet(VAR_PARTY_BATH_ADDITIVE, 0xFFFF);
    EXPECT_EQ(PartyBath_GetAdditive(), PARTY_BATH_FUEN);
}

TEST("Party bath: only water palette entries change, and skipFaded is respected")
{
    u32 additive = 0;
    for (u32 i = 0; i < PARTY_BATH_COUNT; i++)
        PARAMETRIZE { additive = i; }
    EnterBath(additive);
    for (u32 i = 0; i < 512; i++)
        gPlttBufferUnfaded[i] = gPlttBufferFaded[i] = 0x1234;
    PartyBath_ApplyWaterPalette(TRUE);
    for (u32 i = 0; i < 512; i++)
    {
        EXPECT_EQ(gPlttBufferFaded[i], 0x1234);
        if (i == BG_PLTT_ID(8) + 9 || i == BG_PLTT_ID(8) + 10)
            EXPECT_NE(gPlttBufferUnfaded[i], 0x1234);
        else
            EXPECT_EQ(gPlttBufferUnfaded[i], 0x1234);
    }
    PartyBath_ApplyWaterPalette(FALSE);
    EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(8) + 9], gPlttBufferFaded[BG_PLTT_ID(8) + 9]);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_PARTY_ROAMER_PARK);
    gPlttBufferUnfaded[BG_PLTT_ID(8) + 9] = 0x1234;
    PartyBath_ApplyWaterPalette(FALSE);
    EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(8) + 9], 0x1234);
    ResetTasks();
}

TEST("Party bath: camp plots and legacy berry objects are replaced while the exit is retained")
{
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_PARTY_ROAMER_PARK);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_PARTY_ROAMER_PARK);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_PARTY_ROAMER_PARK), MAP_NUM(MAP_PARTY_ROAMER_PARK));
    LoadObjEventTemplatesFromHeader();
    gSaveBlock1Ptr->objectEventTemplates[8].graphicsId = OBJ_EVENT_GFX_BERRY_TREE;
    gSaveBlock1Ptr->objectEventTemplates[9].graphicsId = OBJ_EVENT_GFX_BERRY_TREE;
    InitMap();
    for (u32 y = 7; y <= 9; y++)
        for (u32 x = 1; x <= 4; x++)
            EXPECT_EQ(MapGridGetMetatileIdAt(x + MAP_OFFSET, y + MAP_OFFSET), METATILE_General_Grass);
    EXPECT_EQ(gSaveBlock1Ptr->objectEventTemplates[8].graphicsId, OBJ_EVENT_GFX_OLD_MAN);
    EXPECT_NE(gSaveBlock1Ptr->objectEventTemplates[9].graphicsId, OBJ_EVENT_GFX_BERRY_TREE);
    EXPECT_EQ(gMapHeader.events->warps[1].x, 24);
    EXPECT_EQ(gMapHeader.events->warps[1].y, 4);
}
