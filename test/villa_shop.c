#include "global.h"
#include "decoration.h"
#include "decoration_inventory.h"
#include "event_data.h"
#include "frontier_util.h"
#include "pokemon.h"
#include "special_doll.h"
#include "villa_shop.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/decorations.h"
#include "constants/pokeball.h"

TEST("Villa shop: all 27 balls have the requested BP prices")
{
    static const u8 expected[POKEBALL_COUNT] = {
        0, 1, 2, 3, 50, 3, 2, 5, 3, 5, 5, 5, 5, 5, 10,
        20, 20, 20, 20, 20, 20, 20, 30, 30, 30, 30, 30, 100,
    };
    for (u32 ball = 0; ball < POKEBALL_COUNT; ball++)
        EXPECT_EQ(VillaShop_GetBallPrice(ball), expected[ball]);
    EXPECT_EQ(VillaShop_GetBallPrice(POKEBALL_COUNT), 0);
    EXPECT_EQ(VillaShop_GetBallPrice(0xFFFF), 0);
}

TEST("Villa shop: every decoration can be purchased for exactly 10 BP")
{
    SetDecorationInventoriesPointers();
    for (u32 decor = 1; decor <= NUM_DECORATIONS; decor++)
    {
        ClearDecorationInventories();
        gSaveBlock2Ptr->frontier.battlePoints = 10;
        EXPECT_EQ(VillaShop_BuyDecoration(decor), VILLA_SHOP_OK);
        EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
        EXPECT_EQ(CheckHasDecoration(decor), TRUE);
    }
}

TEST("Villa shop: full furniture category does not consume BP")
{
    SetDecorationInventoriesPointers();
    ClearDecorationInventories();
    for (u32 i = 0; i < gDecorationInventories[DECORCAT_DESK].size; i++)
        EXPECT_EQ(DecorationAdd(DECOR_SMALL_DESK), TRUE);
    gSaveBlock2Ptr->frontier.battlePoints = 100;
    EXPECT_EQ(VillaShop_BuyDecoration(DECOR_POKEMON_DESK), VILLA_SHOP_STORAGE_FULL);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    EXPECT_EQ(CheckHasDecoration(DECOR_POKEMON_DESK), FALSE);
}

TEST("Villa shop: insufficient BP and invalid furniture ids leave inventory unchanged")
{
    SetDecorationInventoriesPointers();
    ClearDecorationInventories();
    gSaveBlock2Ptr->frontier.battlePoints = 9;
    EXPECT_EQ(VillaShop_BuyDecoration(DECOR_SMALL_DESK), VILLA_SHOP_NOT_ENOUGH_BP);
    EXPECT_EQ(VillaShop_BuyDecoration(DECOR_NONE), VILLA_SHOP_INVALID);
    EXPECT_EQ(VillaShop_BuyDecoration(NUM_DECORATIONS + 1), VILLA_SHOP_INVALID);
    EXPECT_EQ(VillaShop_BuyDecoration(0xFFFF), VILLA_SHOP_INVALID);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 9);
    EXPECT_EQ(CheckHasDecoration(DECOR_SMALL_DESK), FALSE);
}

TEST("Villa shop: changing each ball edits only the selected Pokemon ball data")
{
    struct Pokemon expected, other;
    u16 ball = BALL_POKE;
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 50, 100, OTID_STRUCT_PRESET(0x12345678));
    other = gParties[B_TRAINER_PLAYER][0];
    for (ball = 1; ball < POKEBALL_COUNT; ball++)
    {
        u16 oldBall = ball == BALL_POKE ? BALL_GREAT : BALL_POKE;
        CreateMon(&gParties[B_TRAINER_PLAYER][2], SPECIES_EEVEE, 50, 100, OTID_STRUCT_PRESET(0x12345678));
        SetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_POKEBALL, &oldBall);
        expected = gParties[B_TRAINER_PLAYER][2];
        SetMonData(&expected, MON_DATA_POKEBALL, &ball);
        gSaveBlock2Ptr->frontier.battlePoints = 100;
        EXPECT_EQ(VillaShop_ChangeBall(2, ball), VILLA_SHOP_OK);
        EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100 - VillaShop_GetBallPrice(ball));
        EXPECT_EQ(memcmp(&expected, &gParties[B_TRAINER_PLAYER][2], sizeof(expected)), 0);
        EXPECT_EQ(memcmp(&other, &gParties[B_TRAINER_PLAYER][0], sizeof(other)), 0);
    }
}

TEST("Villa shop: same ball and insufficient BP do not charge or change the Pokemon")
{
    struct Pokemon before;
    u16 ball = BALL_POKE;
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 50, 100, OTID_STRUCT_PRESET(0x12345678));
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_POKEBALL, &ball);
    before = gParties[B_TRAINER_PLAYER][0];
    gSaveBlock2Ptr->frontier.battlePoints = 49;
    EXPECT_EQ(VillaShop_ChangeBall(0, BALL_POKE), VILLA_SHOP_SAME_BALL);
    EXPECT_EQ(VillaShop_ChangeBall(0, BALL_MASTER), VILLA_SHOP_NOT_ENOUGH_BP);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 49);
    EXPECT_EQ(memcmp(&before, &gParties[B_TRAINER_PLAYER][0], sizeof(before)), 0);
}

TEST("Villa shop: eggs empty slots cancel slots and invalid balls are rejected")
{
    bool8 isEgg = TRUE;
    struct Pokemon before;
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 50, 100, OTID_STRUCT_PRESET(0x12345678));
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_IS_EGG, &isEgg);
    before = gParties[B_TRAINER_PLAYER][0];
    ZeroMonData(&gParties[B_TRAINER_PLAYER][1]);
    gSaveBlock2Ptr->frontier.battlePoints = 100;
    EXPECT_EQ(VillaShop_ChangeBall(0, BALL_MASTER), VILLA_SHOP_INVALID);
    EXPECT_EQ(VillaShop_ChangeBall(1, BALL_MASTER), VILLA_SHOP_INVALID);
    EXPECT_EQ(VillaShop_ChangeBall(PARTY_SIZE, BALL_MASTER), VILLA_SHOP_INVALID);
    EXPECT_EQ(VillaShop_ChangeBall(0xFFFF, BALL_MASTER), VILLA_SHOP_INVALID);
    EXPECT_EQ(VillaShop_ChangeBall(1, BALL_STRANGE), VILLA_SHOP_INVALID);
    EXPECT_EQ(VillaShop_ChangeBall(1, 0xFFFF), VILLA_SHOP_INVALID);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
    EXPECT_EQ(memcmp(&before, &gParties[B_TRAINER_PLAYER][0], sizeof(before)), 0);
}

TEST("Villa shop: script dispatcher cannot treat unknown services as purchases")
{
    gSaveBlock2Ptr->frontier.battlePoints = 100;
    gSpecialVar_0x8006 = 0xFFFF;
    gSpecialVar_0x8005 = DECOR_SMALL_DESK;
    VillaShop_Purchase();
    EXPECT_EQ(gSpecialVar_Result, VILLA_SHOP_INVALID);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
}

TEST("Villa shop: every decoration belongs to exactly one of the eight ordinary categories")
{
    for (u32 decor = 1; decor <= NUM_DECORATIONS; decor++)
    {
        u32 matches = 0;
        for (u32 category = 0; category < DECORCAT_COUNT; category++)
            matches += VillaShop_IsInCategory(category, decor);
        EXPECT_EQ(matches, 1);
    }
    EXPECT(!VillaShop_IsInCategory(DECORCAT_DESK, DECOR_PIKACHU_DOLL));
    EXPECT(!VillaShop_IsInCategory(DECORCAT_DOLL, DECOR_SMALL_DESK));
    EXPECT(!VillaShop_IsInCategory(0xFFFF, DECOR_SMALL_DESK));
}

TEST("Villa shop: all special dolls cost ten BP and use separate inventory")
{
    u8 ordinary[sizeof(gSaveBlock1Ptr->decorationDolls)];
    for (u32 facility = 0; facility < NUM_FRONTIER_FACILITIES; facility++)
        FlagSet(gFrontierBrainInfo[facility].goldSymbolFlag);
    memset(gSaveBlock1Ptr->decorationDolls, DECOR_PIKACHU_DOLL, sizeof(ordinary));
    memcpy(ordinary, gSaveBlock1Ptr->decorationDolls, sizeof(ordinary));
    for (u32 doll = 1; doll < SPECIAL_DOLL_COUNT; doll++)
    {
        memset(&gSaveBlock3Ptr->specialDolls, 0, sizeof(struct SpecialDollSave));
        gSaveBlock2Ptr->frontier.battlePoints = 10;
        EXPECT(VillaShop_IsInCategory(SpecialDoll_IsShiny(doll)
            ? VILLA_SHOP_SHINY_DOLL_CATEGORY : VILLA_SHOP_SPECIAL_DOLL_CATEGORY, doll));
        EXPECT_EQ(VillaShop_BuySpecialDoll(doll), VILLA_SHOP_OK);
        EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
        EXPECT_EQ(SpecialDoll_GetOwnedCount(), 1);
        EXPECT_EQ(gSaveBlock3Ptr->specialDolls.inventory[0], doll);
    }
    EXPECT_EQ(memcmp(ordinary, gSaveBlock1Ptr->decorationDolls, sizeof(ordinary)), 0);
}

TEST("Villa shop: invalid full or unaffordable special doll purchases do not spend BP")
{
    u16 doll = SPECIAL_DOLL_VENUSAUR_MEGA;
    PARAMETRIZE { doll = SPECIAL_DOLL_VENUSAUR_MEGA; }
    PARAMETRIZE { doll = SPECIAL_DOLL_VENUSAUR_MEGA_SHINY; }
    memset(&gSaveBlock3Ptr->specialDolls, 0, sizeof(struct SpecialDollSave));
    for (u32 facility = 0; facility < NUM_FRONTIER_FACILITIES; facility++)
        FlagSet(gFrontierBrainInfo[facility].goldSymbolFlag);
    gSaveBlock2Ptr->frontier.battlePoints = 9;
    EXPECT_EQ(VillaShop_BuySpecialDoll(doll), VILLA_SHOP_NOT_ENOUGH_BP);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 9);
    EXPECT_EQ(VillaShop_BuySpecialDoll(0), VILLA_SHOP_INVALID);
    EXPECT_EQ(VillaShop_BuySpecialDoll(255), VILLA_SHOP_INVALID);
    for (u32 i = 0; i < SPECIAL_DOLL_CAPACITY; i++)
        EXPECT(SpecialDoll_Add(SPECIAL_DOLL_VENUSAUR_MEGA));
    gSaveBlock2Ptr->frontier.battlePoints = 100;
    EXPECT_EQ(VillaShop_BuySpecialDoll(doll), VILLA_SHOP_STORAGE_FULL);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 100);
}

TEST("Villa shop: script dispatcher purchases special dolls without treating their IDs as ordinary furniture")
{
    gSaveBlock2Ptr->frontier.battlePoints = 10;
    gSpecialVar_0x8006 = VILLA_SHOP_SPECIAL_DOLLS;
    gSpecialVar_0x8005 = SPECIAL_DOLL_CHARIZARD_MEGA_X;
    VillaShop_Purchase();
    EXPECT_EQ(gSpecialVar_Result, VILLA_SHOP_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(SpecialDoll_GetOwnedCount(), 1);
}

TEST("Villa shop: shiny category requires three distinct gold symbols for every facility combination")
{
    u32 symbols = 0, goldCount = 0;
    for (u32 mask = 0; mask < (1 << NUM_FRONTIER_FACILITIES); mask++)
        PARAMETRIZE { symbols = mask; }
    for (u32 facility = 0; facility < NUM_FRONTIER_FACILITIES; facility++)
    {
        FlagSet(gFrontierBrainInfo[facility].silverSymbolFlag);
        if (symbols & (1 << facility))
        {
            FlagSet(gFrontierBrainInfo[facility].goldSymbolFlag);
            goldCount++;
        }
        else
        {
            FlagClear(gFrontierBrainInfo[facility].goldSymbolFlag);
        }
    }
    FlagSet(FLAG_COLLECTED_ALL_GOLD_SYMBOLS);
    EXPECT_EQ(VillaShop_IsCategoryAvailable(VILLA_SHOP_SHINY_DOLL_CATEGORY), goldCount >= 3);
    for (u32 category = 0; category < VILLA_SHOP_SHINY_DOLL_CATEGORY; category++)
        EXPECT(VillaShop_IsCategoryAvailable(category));
    EXPECT(!VillaShop_IsCategoryAvailable(VILLA_SHOP_CATEGORY_COUNT));
    EXPECT(!VillaShop_IsCategoryAvailable(0xFFFF));
}

TEST("Villa shop: normal and shiny dolls appear only in their respective categories")
{
    u32 normalCount = 0, shinyCount = 0;
    for (u32 doll = 1; doll < SPECIAL_DOLL_COUNT; doll++)
    {
        bool8 normal = VillaShop_IsInCategory(VILLA_SHOP_SPECIAL_DOLL_CATEGORY, doll);
        bool8 shiny = VillaShop_IsInCategory(VILLA_SHOP_SHINY_DOLL_CATEGORY, doll);
        EXPECT_EQ(normal + shiny, 1);
        normalCount += normal;
        shinyCount += shiny;
    }
    EXPECT_EQ(normalCount, 51);
    EXPECT_EQ(shinyCount, 51);
    EXPECT(!VillaShop_IsInCategory(VILLA_SHOP_SHINY_DOLL_CATEGORY, SPECIAL_DOLL_NONE));
    EXPECT(!VillaShop_IsInCategory(VILLA_SHOP_SHINY_DOLL_CATEGORY, 0xFFFF));
}

TEST("Villa shop: locked shiny purchases cannot spend BP or bypass the check through script dispatch")
{
    for (u32 facility = 0; facility < NUM_FRONTIER_FACILITIES; facility++)
        FlagClear(gFrontierBrainInfo[facility].goldSymbolFlag);
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_TOWER].goldSymbolFlag);
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_DOME].goldSymbolFlag);
    gSaveBlock2Ptr->frontier.battlePoints = 20;
    EXPECT_EQ(VillaShop_BuySpecialDoll(SPECIAL_DOLL_VENUSAUR_MEGA_SHINY), VILLA_SHOP_INVALID);
    gSpecialVar_0x8006 = VILLA_SHOP_SPECIAL_DOLLS;
    gSpecialVar_0x8005 = SPECIAL_DOLL_NECROZMA_ULTRA_SHINY;
    VillaShop_Purchase();
    EXPECT_EQ(gSpecialVar_Result, VILLA_SHOP_INVALID);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 20);
    EXPECT_EQ(SpecialDoll_GetOwnedCount(), 0);
    EXPECT_EQ(VillaShop_BuySpecialDoll(SPECIAL_DOLL_VENUSAUR_MEGA), VILLA_SHOP_OK);
    FlagSet(gFrontierBrainInfo[FRONTIER_FACILITY_PYRAMID].goldSymbolFlag);
    VillaShop_Purchase();
    EXPECT_EQ(gSpecialVar_Result, VILLA_SHOP_OK);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
    EXPECT_EQ(SpecialDoll_GetOwnedCount(), 2);
    EXPECT_EQ(gSaveBlock3Ptr->specialDolls.inventory[1], SPECIAL_DOLL_NECROZMA_ULTRA_SHINY);
}
