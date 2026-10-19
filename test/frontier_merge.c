#include "global.h"
#include "battle.h"
#include "battle_factory.h"
#include "battle_frontier.h"
#include "event_data.h"
#include "pokemon.h"
#include "string_util.h"
#include "test/test.h"
#include "constants/abilities.h"
#include "constants/battle_factory.h"
#include "constants/battle_frontier.h"

TEST("Frontier merge: facility creation preserves custom IVs and nickname without enabling unrequested gimmicks")
{
    static const struct TrainerMon fmon = {
        .species = SPECIES_CHARIZARD,
        .moves = {MOVE_FLAMETHROWER, MOVE_AIR_SLASH, MOVE_HIDDEN_POWER, MOVE_ROOST},
        .iv = TRAINER_PARTY_IVS(31, 30, 31, 30, 30, 31),
        .nature = NATURE_MODEST,
        .ability = ABILITY_SOLAR_POWER,
        .nickname = COMPOUND_STRING("{JPN}テスト"),
        .dynamaxLevel = 10,
        .shouldUseDynamax = FALSE,
    };
    struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][0];
    u8 nickname[POKEMON_NAME_BUFFER_SIZE];

    CreateFacilityMon(&fmon, 50, 0, 0x12345678, 0, mon);
    EXPECT_EQ(GetMonData(mon, MON_DATA_IVS), fmon.iv);
    EXPECT_EQ(GetMonAbility(mon), ABILITY_SOLAR_POWER);
    EXPECT_EQ(GetNature(mon), NATURE_MODEST);
    EXPECT_EQ(GetMonData(mon, MON_DATA_DYNAMAX_LEVEL), BLOCK_AI_DYNAMAX);
    EXPECT_EQ(GetMonData(mon, MON_DATA_TERA_TYPE), TYPE_MYSTERY);
    GetMonData(mon, MON_DATA_NICKNAME, nickname);
    EXPECT_EQ(StringCompare(nickname, fmon.nickname), 0);
}

TEST("Frontier merge: Factory reconstruction keeps rental personality and ability and registers opponent gimmick limits")
{
    u32 levelMode = FRONTIER_LVL_50;
    PARAMETRIZE { levelMode = FRONTIER_LVL_50; }
    PARAMETRIZE { levelMode = FRONTIER_LVL_OPEN; }
    PARAMETRIZE { levelMode = FRONTIER_LVL_TENT; }

    gSaveBlock2Ptr->frontier.lvlMode = levelMode;
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE * 2; i++)
    {
        struct RentalMon *rental = &gSaveBlock2Ptr->frontier.rentalMons[i];
        rental->monId = i;
        rental->ivs = 13 + i;
        rental->personality = 0x12345678 + i;
        rental->abilityNum = i % 2;
    }
    gSpecialVar_0x8004 = BATTLE_FACTORY_FUNC_SET_PARTIES;
    CallBattleFactoryFunction();

    for (u32 i = 0; i < FRONTIER_PARTY_SIZE * 2; i++)
    {
        struct RentalMon *rental = &gSaveBlock2Ptr->frontier.rentalMons[i];
        const struct TrainerMon *fmon = &gFacilityTrainerMons[rental->monId];
        struct Pokemon *mon = i < FRONTIER_PARTY_SIZE
            ? &gParties[B_TRAINER_PLAYER][i]
            : &gParties[B_TRAINER_OPPONENT_A][i - FRONTIER_PARTY_SIZE];
        EXPECT_EQ(GetMonData(mon, MON_DATA_SPECIES), fmon->species);
        EXPECT_EQ(GetMonData(mon, MON_DATA_PERSONALITY), rental->personality);
        EXPECT_EQ(GetMonData(mon, MON_DATA_ABILITY_NUM), rental->abilityNum);
        if (fmon->iv)
            EXPECT_EQ(GetMonData(mon, MON_DATA_IVS), fmon->iv);
        else
            EXPECT_EQ(GetMonData(mon, MON_DATA_HP_IV), rental->ivs);
        if (fmon->nickname != NULL)
        {
            u8 nickname[POKEMON_NAME_BUFFER_SIZE];
            GetMonData(mon, MON_DATA_NICKNAME, nickname);
            EXPECT_EQ(StringCompare(nickname, fmon->nickname), 0);
        }
        if (i >= FRONTIER_PARTY_SIZE)
        {
            EXPECT_EQ(GetMonData(mon, MON_DATA_DYNAMAX_LEVEL),
                      fmon->shouldUseDynamax && fmon->dynamaxLevel > 0 ? fmon->dynamaxLevel : BLOCK_AI_DYNAMAX);
            EXPECT_EQ(GetMonData(mon, MON_DATA_TERA_TYPE), fmon->teraType ? fmon->teraType : TYPE_MYSTERY);
        }
    }
}
