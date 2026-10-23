#include "global.h"
#include "battle.h"
#include "party_training.h"
#include "pokemon.h"
#include "test/test.h"

static struct Pokemon *CreateTrainingMon(u32 slot)
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
    CreateMonWithIVs(mon, SPECIES_PIKACHU, 50, 1234, OTID_STRUCT_PLAYER_ID, 31);
    return mon;
}

TEST("Party training: all six EV fields can be edited, and only the chosen Pokemon changes")
{
    const u16 evs[NUM_STATS] = {100, 120, 80, 60, 70, 80};
    struct Pokemon *mon = CreateTrainingMon(0);
    struct Pokemon *other = CreateTrainingMon(1);
    EXPECT(PartyTraining_ApplyEVs(0, evs));
    for (u32 stat = 0; stat < NUM_STATS; stat++)
    {
        EXPECT_EQ(GetMonData(mon, MON_DATA_HP_EV + stat), evs[stat]);
        EXPECT_EQ(GetMonData(other, MON_DATA_HP_EV + stat), 0);
    }
    const u16 zero[NUM_STATS] = {0};
    EXPECT(PartyTraining_ApplyEVs(0, zero));
    EXPECT_EQ(GetMonEVCount(mon), 0);
}

TEST("Party training: exceeding any per-stat EV cap fails without a partial write")
{
    u32 stat = 0;
    for (u32 i = 0; i < NUM_STATS; i++)
        PARAMETRIZE { stat = i; }
    u16 evs[NUM_STATS] = {10, 10, 10, 10, 10, 10};
    struct Pokemon *mon = CreateTrainingMon(0);
    evs[stat] = MAX_PER_STAT_EVS + 1;
    EXPECT(!PartyTraining_ApplyEVs(0, evs));
    EXPECT_EQ(GetMonEVCount(mon), 0);
}

TEST("Party training: total EV cap is enforced before changing the Pokemon")
{
    const u16 valid[NUM_STATS] = {252, 252, 6, 0, 0, 0};
    const u16 invalid[NUM_STATS] = {252, 252, 7, 0, 0, 0};
    struct Pokemon *mon = CreateTrainingMon(0);
    EXPECT(PartyTraining_ApplyEVs(0, valid));
    EXPECT_EQ(GetMonEVCount(mon), MAX_TOTAL_EVS);
    EXPECT(!PartyTraining_ApplyEVs(0, invalid));
    for (u32 stat = 0; stat < NUM_STATS; stat++)
        EXPECT_EQ(GetMonData(mon, MON_DATA_HP_EV + stat), valid[stat]);
}

TEST("Party training: eggs, empty slots, invalid slots and null input are rejected")
{
    const u16 evs[NUM_STATS] = {252, 0, 0, 0, 0, 0};
    u8 egg = TRUE;
    ZeroPlayerPartyMons();
    EXPECT(!PartyTraining_ApplyEVs(0, evs));
    EXPECT(!PartyTraining_ApplyEVs(PARTY_SIZE, evs));
    EXPECT(!PartyTraining_ApplyEVs(0xFFFF, evs));
    struct Pokemon *mon = CreateTrainingMon(0);
    EXPECT(!PartyTraining_ApplyEVs(0, NULL));
    SetMonData(mon, MON_DATA_IS_EGG, &egg);
    EXPECT(!PartyTraining_ApplyEVs(0, evs));
    EXPECT_EQ(GetMonEVCount(mon), 0);
}

TEST("Party training: stats are recalculated, full HP stays full and fainted Pokemon stay fainted")
{
    const u16 evs[NUM_STATS] = {252, 252, 0, 0, 0, 0};
    const u16 zero[NUM_STATS] = {0};
    struct Pokemon *mon = CreateTrainingMon(0);
    u16 oldHP = GetMonData(mon, MON_DATA_MAX_HP);
    u16 oldAttack = GetMonData(mon, MON_DATA_ATK);
    EXPECT(PartyTraining_ApplyEVs(0, evs));
    EXPECT_GT(GetMonData(mon, MON_DATA_MAX_HP), oldHP);
    EXPECT_GT(GetMonData(mon, MON_DATA_ATK), oldAttack);
    EXPECT_EQ(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP));
    EXPECT(PartyTraining_ApplyEVs(0, zero));
    EXPECT_EQ(GetMonData(mon, MON_DATA_HP), oldHP);
    u16 fainted = 0;
    SetMonData(mon, MON_DATA_HP, &fainted);
    EXPECT(PartyTraining_ApplyEVs(0, evs));
    EXPECT_EQ(GetMonData(mon, MON_DATA_HP), 0);
}
