#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "field_specials.h"
#include "frontier_util.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/field_specials.h"

static void SetUpShinyMeal(void)
{
    bool8 isShiny = FALSE;
    u32 i;

    for (i = 0; i < NUM_FRONTIER_FACILITIES; i++)
    {
        FlagClear(gFrontierBrainInfo[i].goldSymbolFlag);
        FlagClear(gFrontierBrainInfo[i].silverSymbolFlag);
    }
    FlagSet(FLAG_SYS_TOWER_GOLD);
    gSaveBlock2Ptr->frontier.battlePoints = 10;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        CreateMon(&gParties[B_TRAINER_PLAYER][i], SPECIES_PIKACHU, 50, 100 + i, OTID_STRUCT_PRESET(0x12345678));
        SetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_IS_SHINY, &isShiny);
    }
    gSpecialVar_0x8004 = 2;
}

TEST("Rocket Fast Food unlocks with a gold symbol from any facility")
{
    u32 facility = FRONTIER_FACILITY_TOWER;

    for (u32 i = 0; i < NUM_FRONTIER_FACILITIES; i++)
        PARAMETRIZE { facility = i; }

    SetUpShinyMeal();
    FlagClear(FLAG_SYS_TOWER_GOLD);
    EXPECT_EQ(Special_TeishokuyaHasGoldSymbol(), FALSE);
    FlagSet(gFrontierBrainInfo[facility].goldSymbolFlag);
    EXPECT_EQ(Special_TeishokuyaHasGoldSymbol(), TRUE);
    EXPECT_EQ(Special_TeishokuyaGetShinyMealStatus(), TEISHOKUYA_SHINY_MEAL_OK);
}

TEST("Rocket Fast Food stays locked with only silver symbols")
{
    u32 facility;

    SetUpShinyMeal();
    FlagClear(FLAG_SYS_TOWER_GOLD);
    for (facility = 0; facility < NUM_FRONTIER_FACILITIES; facility++)
        FlagSet(gFrontierBrainInfo[facility].silverSymbolFlag);

    EXPECT_EQ(Special_TeishokuyaHasGoldSymbol(), FALSE);
    EXPECT_EQ(Test_TeishokuyaTryMakeMonShiny(2), TEISHOKUYA_SHINY_MEAL_LOCKED);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 10);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_IS_SHINY), FALSE);
}

TEST("Rocket Fast Food status check does not consume BP or change the Pokemon")
{
    struct Pokemon before;

    SetUpShinyMeal();
    before = gParties[B_TRAINER_PLAYER][2];
    EXPECT_EQ(Special_TeishokuyaGetShinyMealStatus(), TEISHOKUYA_SHINY_MEAL_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 10);
    EXPECT_EQ(memcmp(&before, &gParties[B_TRAINER_PLAYER][2], sizeof(before)), 0);
}

TEST("Rocket Fast Food changes only the selected Pokemon's shininess and charges one BP once")
{
    struct Pokemon before[PARTY_SIZE];
    struct Pokemon expected;
    bool8 isShiny = TRUE;
    u32 i;

    SetUpShinyMeal();
    memcpy(before, gParties[B_TRAINER_PLAYER], sizeof(before));
    expected = before[2];
    SetMonData(&expected, MON_DATA_IS_SHINY, &isShiny);

    EXPECT_EQ(Test_TeishokuyaTryMakeMonShiny(2), TEISHOKUYA_SHINY_MEAL_OK);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_IS_SHINY), TRUE);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 9);
    EXPECT_EQ(memcmp(&expected, &gParties[B_TRAINER_PLAYER][2], sizeof(expected)), 0);
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (i != 2)
            EXPECT_EQ(memcmp(&before[i], &gParties[B_TRAINER_PLAYER][i], sizeof(before[i])), 0);
    }

    EXPECT_EQ(Test_TeishokuyaTryMakeMonShiny(2), TEISHOKUYA_SHINY_MEAL_ALREADY_SHINY);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 9);
}

TEST("Rocket Fast Food accepts exactly one BP")
{
    SetUpShinyMeal();
    gSaveBlock2Ptr->frontier.battlePoints = 1;

    EXPECT_EQ(Test_TeishokuyaTryMakeMonShiny(2), TEISHOKUYA_SHINY_MEAL_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_IS_SHINY), TRUE);
}

TEST("Rocket Fast Food rejects zero BP without changing the Pokemon")
{
    SetUpShinyMeal();
    gSaveBlock2Ptr->frontier.battlePoints = 0;

    EXPECT_EQ(Test_TeishokuyaTryMakeMonShiny(2), TEISHOKUYA_SHINY_MEAL_NOT_ENOUGH_BP);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_IS_SHINY), FALSE);
}

TEST("Rocket Fast Food rejects an already shiny Pokemon without consuming BP")
{
    bool8 isShiny = TRUE;

    SetUpShinyMeal();
    SetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_IS_SHINY, &isShiny);

    EXPECT_EQ(Test_TeishokuyaTryMakeMonShiny(2), TEISHOKUYA_SHINY_MEAL_ALREADY_SHINY);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 10);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_IS_SHINY), TRUE);
}

TEST("Rocket Fast Food rejects eggs and empty party slots without consuming BP")
{
    bool8 isEgg = FALSE;
    struct Pokemon before;

    PARAMETRIZE { isEgg = FALSE; }
    PARAMETRIZE { isEgg = TRUE; }

    SetUpShinyMeal();
    if (isEgg)
        SetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_IS_EGG, &isEgg);
    else
        ZeroMonData(&gParties[B_TRAINER_PLAYER][2]);
    before = gParties[B_TRAINER_PLAYER][2];

    EXPECT_EQ(Test_TeishokuyaTryMakeMonShiny(2), TEISHOKUYA_SHINY_MEAL_INVALID_MON);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 10);
    EXPECT_EQ(memcmp(&before, &gParties[B_TRAINER_PLAYER][2], sizeof(before)), 0);
}

TEST("Rocket Fast Food rejects cancelled or invalid party selections without consuming BP")
{
    u16 partyIndex = PARTY_SIZE;

    PARAMETRIZE { partyIndex = PARTY_SIZE; }
    PARAMETRIZE { partyIndex = 0xFF; }
    PARAMETRIZE { partyIndex = 0xFFFF; }

    SetUpShinyMeal();
    gSpecialVar_0x8004 = partyIndex;
    EXPECT_EQ(Special_TeishokuyaGetShinyMealStatus(), TEISHOKUYA_SHINY_MEAL_INVALID_MON);
    EXPECT_EQ(Test_TeishokuyaTryMakeMonShiny(partyIndex), TEISHOKUYA_SHINY_MEAL_INVALID_MON);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 10);
}
