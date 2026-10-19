#include "global.h"
#include "decoration.h"
#include "decoration_inventory.h"
#include "event_data.h"
#include "pokemon.h"
#include "special_doll.h"
#include "villa_shop.h"
#include "test/test.h"
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
    memset(gSaveBlock1Ptr->decorationDolls, DECOR_PIKACHU_DOLL, sizeof(ordinary));
    memcpy(ordinary, gSaveBlock1Ptr->decorationDolls, sizeof(ordinary));
    for (u32 doll = 1; doll < SPECIAL_DOLL_COUNT; doll++)
    {
        gSaveBlock2Ptr->frontier.battlePoints = 10;
        EXPECT(VillaShop_IsInCategory(VILLA_SHOP_SPECIAL_DOLL_CATEGORY, doll));
        EXPECT_EQ(VillaShop_BuySpecialDoll(doll), VILLA_SHOP_OK);
        EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 0);
        EXPECT_EQ(SpecialDoll_GetOwnedCount(), doll);
    }
    EXPECT_EQ(memcmp(ordinary, gSaveBlock1Ptr->decorationDolls, sizeof(ordinary)), 0);
}

TEST("Villa shop: invalid full or unaffordable special doll purchases do not spend BP")
{
    gSaveBlock2Ptr->frontier.battlePoints = 9;
    EXPECT_EQ(VillaShop_BuySpecialDoll(SPECIAL_DOLL_VENUSAUR_MEGA), VILLA_SHOP_NOT_ENOUGH_BP);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.battlePoints, 9);
    EXPECT_EQ(VillaShop_BuySpecialDoll(0), VILLA_SHOP_INVALID);
    EXPECT_EQ(VillaShop_BuySpecialDoll(255), VILLA_SHOP_INVALID);
    for (u32 i = 0; i < SPECIAL_DOLL_CAPACITY; i++)
        EXPECT(SpecialDoll_Add(SPECIAL_DOLL_VENUSAUR_MEGA));
    gSaveBlock2Ptr->frontier.battlePoints = 100;
    EXPECT_EQ(VillaShop_BuySpecialDoll(SPECIAL_DOLL_NECROZMA_ULTRA), VILLA_SHOP_STORAGE_FULL);
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
