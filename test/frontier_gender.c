#include "global.h"
#include "battle.h"
#include "battle_dome.h"
#include "battle_factory.h"
#include "battle_frontier.h"
#include "battle_setup.h"
#include "battle_tower.h"
#include "event_data.h"
#include "frontier_util.h"
#include "pokemon.h"
#include "random.h"
#include "test/test.h"
#include "constants/battle_dome.h"
#include "constants/battle_factory.h"
#include "constants/battle_frontier.h"
#include "constants/battle_frontier_mons.h"
#include "constants/battle_tower.h"

void CallBattleDomeFunction(void);

struct GenderSample
{
    u32 mixed;
    u32 female;
    u32 femaleRatioSum;
};

static void SetUpChallenge(u32 facility, u32 battleFlags, u32 mode)
{
    VarSet(VAR_FRONTIER_FACILITY, facility);
    VarSet(VAR_FRONTIER_BATTLE_MODE, mode);
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_50;
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = 0;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | battleFlags;
    TRAINER_BATTLE_PARAM.opponentA = 0;
    TRAINER_BATTLE_PARAM.opponentB = 1;
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    SeedRng(0x12345678);
}

static void SampleParty(struct GenderSample *sample, enum BattleTrainer trainer, u32 count)
{
    for (u32 i = 0; i < count; i++)
    {
        struct Pokemon *mon = &gParties[trainer][i];
        u32 species = GetMonData(mon, MON_DATA_SPECIES);
        u32 ratio = gSpeciesInfo[species].genderRatio;
        u32 gender = GetMonGender(mon);
        EXPECT_NE(species, SPECIES_NONE);
        if (ratio == MON_MALE || ratio == MON_FEMALE || ratio == MON_GENDERLESS)
            EXPECT_EQ(gender, ratio);
        else
        {
            EXPECT(gender == MON_MALE || gender == MON_FEMALE);
            sample->mixed++;
            sample->female += gender == MON_FEMALE;
            sample->femaleRatioSum += ratio;
        }
    }
}

static void ExpectMixedGenders(const struct GenderSample *sample)
{
    u32 expectedFemale = sample->femaleRatioSum / 256;
    u32 tolerance = sample->mixed / 4 + 4;
    EXPECT_GT(sample->female, 0);
    EXPECT_LT(sample->female, sample->mixed);
    EXPECT_LE(sample->female, expectedFemale + tolerance);
    EXPECT_GE(sample->female + tolerance, expectedFemale);
}

TEST("Frontier gender: all pool records request random gender")
{
    for (u32 id = 0; id < NUM_FRONTIER_MONS; id++)
        EXPECT_EQ((u32)gBattleFrontierMons[id].gender, TRAINER_MON_RANDOM_GENDER);
}

TEST("Frontier gender: actual pool generation produces both genders at the species ratio")
{
    u32 monId = FRONTIER_MON_CHARIZARD_1;
    PARAMETRIZE { monId = FRONTIER_MON_CHARIZARD_1; }
    PARAMETRIZE { monId = FRONTIER_MON_PIKACHU_1; }
    PARAMETRIZE { monId = FRONTIER_MON_MACHAMP_1; }
    PARAMETRIZE { monId = FRONTIER_MON_MACHAMP_2; }
    PARAMETRIZE { monId = FRONTIER_MON_MACHAMP_3; }
    const struct TrainerMon *fmon = &gBattleFrontierMons[monId];
    struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][0];
    u32 male = 0, female = 0;
    SeedRng(0x12345678);
    for (u32 i = 0; i < 512; i++)
    {
        CreateFacilityMon(fmon, 50, 31, 0x12345678, 0, mon);
        if (GetMonGender(mon) == MON_MALE)
            male++;
        else if (GetMonGender(mon) == MON_FEMALE)
            female++;
        EXPECT_EQ(GetNature(mon), fmon->nature);
    }
    EXPECT_EQ(male + female, 512);
    EXPECT_GT(male, 0);
    EXPECT_GT(female, 0);
    u32 expectedFemale = gSpeciesInfo[fmon->species].genderRatio * 2;
    EXPECT_GE(female + 64, expectedFemale);
    EXPECT_LE(female, expectedFemale + 64);
}

TEST("Frontier gender: single-gender and genderless species keep their only valid gender")
{
    enum Species species = SPECIES_TAUROS;
    PARAMETRIZE { species = SPECIES_TAUROS; }
    PARAMETRIZE { species = SPECIES_KANGASKHAN; }
    PARAMETRIZE { species = SPECIES_METAGROSS; }
    struct TrainerMon fmon = {
        .species = species,
        .nature = NATURE_MODEST,
        .gender = TRAINER_MON_RANDOM_GENDER,
        .ball = BALL_POKE,
    };
    struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][0];
    SeedRng(0x12345678);
    for (u32 i = 0; i < 32; i++)
    {
        CreateFacilityMon(&fmon, 50, 31, 0x12345678, 0, mon);
        EXPECT_EQ(GetMonGender(mon), gSpeciesInfo[species].genderRatio);
    }
}

TEST("Frontier gender: challenge party selection does not fix random genders across facilities")
{
    u32 facility = FRONTIER_FACILITY_TOWER;
    u32 flags = BATTLE_TYPE_BATTLE_TOWER;
    u32 mode = FRONTIER_MODE_SINGLES;
    PARAMETRIZE { facility = FRONTIER_FACILITY_TOWER; flags = BATTLE_TYPE_BATTLE_TOWER; mode = FRONTIER_MODE_SINGLES; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_TOWER; flags = BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_DOUBLE; mode = FRONTIER_MODE_DOUBLES; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_TOWER; flags = BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_TWO_OPPONENTS; mode = FRONTIER_MODE_MULTIS; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_PALACE; flags = BATTLE_TYPE_PALACE; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_ARENA; flags = BATTLE_TYPE_ARENA; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_PIKE; flags = BATTLE_TYPE_BATTLE_TOWER; }
    PARAMETRIZE { facility = FRONTIER_FACILITY_PYRAMID; flags = BATTLE_TYPE_PYRAMID; }
    struct GenderSample sample = {0};
    u32 count = mode == FRONTIER_MODE_MULTIS ? FRONTIER_MULTI_PARTY_SIZE
        : mode == FRONTIER_MODE_DOUBLES ? FRONTIER_DOUBLES_PARTY_SIZE : FRONTIER_PARTY_SIZE;
    SetUpChallenge(facility, flags, mode);
    for (u32 battle = 0; battle < 32; battle++)
    {
        gSaveBlock2Ptr->frontier.curChallengeBattleNum = battle % FRONTIER_STAGES_PER_CHALLENGE;
        if (mode == FRONTIER_MODE_MULTIS)
        {
            FillFrontierTrainersParties(count);
            SampleParty(&sample, B_TRAINER_OPPONENT_B, count);
        }
        else
        {
            if (facility == FRONTIER_FACILITY_TOWER)
            {
                gSpecialVar_0x8004 = BATTLE_TOWER_FUNC_SET_OPPONENT;
                CallBattleTowerFunc();
            }
            FillFrontierTrainerParty(count);
        }
        SampleParty(&sample, B_TRAINER_OPPONENT_A, count);
    }
    ExpectMixedGenders(&sample);
}

TEST("Frontier gender: Factory opponent selection and rental reconstruction preserve random genders")
{
    struct GenderSample sample = {0};
    SetUpChallenge(FRONTIER_FACILITY_FACTORY, BATTLE_TYPE_FACTORY, FRONTIER_MODE_SINGLES);
    memset(gSaveBlock2Ptr->frontier.rentalMons, 0, sizeof(gSaveBlock2Ptr->frontier.rentalMons));
    for (u32 battle = 0; battle < 32; battle++)
    {
        u32 personalities[FRONTIER_PARTY_SIZE], genders[FRONTIER_PARTY_SIZE];
        gSaveBlock2Ptr->frontier.curChallengeBattleNum = battle % FRONTIER_STAGES_PER_CHALLENGE;
        gSpecialVar_0x8004 = BATTLE_FACTORY_FUNC_GENERATE_OPPONENT_MONS;
        CallBattleFactoryFunction();
        FillFactoryTrainerParty();
        SampleParty(&sample, B_TRAINER_OPPONENT_A, FRONTIER_PARTY_SIZE);
        for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
        {
            personalities[i] = GetMonData(&gParties[B_TRAINER_OPPONENT_A][i], MON_DATA_PERSONALITY);
            genders[i] = GetMonGender(&gParties[B_TRAINER_OPPONENT_A][i]);
        }
        gSpecialVar_0x8004 = BATTLE_FACTORY_FUNC_SET_OPPONENT_MONS;
        CallBattleFactoryFunction();
        gSpecialVar_0x8004 = BATTLE_FACTORY_FUNC_SET_PARTIES;
        CallBattleFactoryFunction();
        for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
        {
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][i], MON_DATA_PERSONALITY), personalities[i]);
            EXPECT_EQ(GetMonGender(&gParties[B_TRAINER_OPPONENT_A][i]), genders[i]);
        }
    }
    ExpectMixedGenders(&sample);
}

TEST("Frontier gender: Dome selection generates both genders independently of tournament set IDs")
{
    struct GenderSample sample = {0};
    SetUpChallenge(FRONTIER_FACILITY_DOME, BATTLE_TYPE_DOME, FRONTIER_MODE_SINGLES);
    SetFacilityPtrsGetLevel();
    memset(gSaveBlock2Ptr->frontier.domeTrainers, 0, sizeof(gSaveBlock2Ptr->frontier.domeTrainers));
    gSaveBlock2Ptr->frontier.domeTrainers[1].trainerId = TRAINER_BATTLE_PARAM.opponentA;
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
        CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][i], SPECIES_PIKACHU, 50,
            0x12345678 + i, OTID_STRUCT_PLAYER_ID, 31);
    for (u32 battle = 0; battle < 32; battle++)
    {
        u16 *mons = gSaveBlock2Ptr->frontier.domeMonIds[1];
        for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
            mons[i] = GetRandomFrontierMonFromFullPool(mons, i, NULL, 0, NULL, 0);
        gSpecialVar_0x8004 = BATTLE_DOME_FUNC_INIT_OPPONENT_PARTY;
        CallBattleDomeFunction();
        SampleParty(&sample, B_TRAINER_OPPONENT_A, DOME_BATTLE_PARTY_SIZE);
    }
    ExpectMixedGenders(&sample);
}
