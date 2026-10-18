#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_dynamax.h"
#include "battle_frontier.h"
#include "battle_gimmick.h"
#include "battle_gimmick_extra.h"
#include "battle_pyramid.h"
#include "battle_setup.h"
#include "battle_util.h"
#include "battle_z_move.h"
#include "event_data.h"
#include "item.h"
#include "malloc.h"
#include "pokemon.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/items.h"
#include "constants/trainers.h"

static void SetUpPyramid(void)
{
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_PYRAMID;
    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_PYRAMID);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_50;
    gSaveBlock2Ptr->frontier.pyramidWinStreaks[0] = 0;
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = 0;
    TRAINER_BATTLE_PARAM.opponentA = 0;
    for (u32 trainer = 0; trainer < MAX_BATTLE_TRAINERS; trainer++)
        for (u32 slot = 0; slot < PARTY_SIZE; slot++)
            ZeroMonData(&gParties[trainer][slot]);
}

static void CreateTestMon(struct Pokemon *mon, enum Species species)
{
    CreateMonWithIVs(mon, species, 50, 0x12345678, OTID_STRUCT_PRESET(1234), 31);
    for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
        SetMonMoveSlot(mon, slot == 0 ? MOVE_THUNDERBOLT : MOVE_NONE, slot);
}

TEST("Pyramid: Mega, Ultra Burst, Dynamax and Primal are blocked on both sides")
{
    SetUpPyramid();
    for (enum BattlerId battler = B_BATTLER_0; battler < MAX_BATTLERS_COUNT; battler++)
    {
        EXPECT(!CanActivateGimmick(battler, GIMMICK_MEGA));
        EXPECT(!CanActivateGimmick(battler, GIMMICK_ULTRA_BURST));
        EXPECT(!CanActivateGimmick(battler, GIMMICK_DYNAMAX));
        EXPECT(!CanMegaEvolve(battler));
        EXPECT(!CanUltraBurst(battler));
        EXPECT(!CanDynamax(battler));
        gBattleMons[battler].species = SPECIES_KYOGRE;
        gBattleMons[battler].item = ITEM_BLUE_ORB;
        EXPECT_EQ(GetBattleFormChangeTargetSpecies(battler, FORM_CHANGE_BATTLE_PRIMAL_REVERSION, ABILITY_DRIZZLE), SPECIES_KYOGRE);
    }
}

TEST("Pyramid: paired opponents share pending and used Z and Tera limits")
{
    enum Gimmick gimmick;
    PARAMETRIZE { gimmick = GIMMICK_Z_MOVE; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    SetUpPyramid();
    gBattleTypeFlags |= BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS;
    gBattlersCount = 4;
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));
    for (u32 battler = 0; battler < MAX_BATTLERS_COUNT; battler++)
    {
        gBattlerPositions[battler] = battler;
        gBattlerPartyIndexes[battler] = 0;
    }
    gBattleStruct->gimmick.usableGimmick[B_BATTLER_1] = gimmick;
    gBattleStruct->gimmick.toActivate = 1u << B_BATTLER_1;
    EXPECT(HasTrainerUsedGimmick(B_BATTLER_3, gimmick));
    EXPECT(!HasTrainerUsedGimmick(B_BATTLER_0, gimmick));
    gBattleStruct->gimmick.toActivate = 0;
    SetGimmickAsActivated(B_BATTLER_1, gimmick);
    EXPECT(HasTrainerUsedGimmick(B_BATTLER_3, gimmick));
    ClearGimmickAsActivated(B_BATTLER_1, gimmick);
    EXPECT(!HasTrainerUsedGimmick(B_BATTLER_3, gimmick));
    gBattleTypeFlags &= ~BATTLE_TYPE_PYRAMID;
    SetGimmickAsActivated(B_BATTLER_1, gimmick);
    EXPECT(!HasTrainerUsedGimmick(B_BATTLER_3, gimmick));
    Free(gBattleStruct);
    gBattleStruct = NULL;
}

TEST("Pyramid: crystals match moves and signature species without Ultra Burst")
{
    struct Pokemon mon;
    CreateTestMon(&mon, SPECIES_PIKACHU);
    for (u32 seed = 0; seed < 40; seed++)
        EXPECT_EQ(GetPyramidZCrystalForMon(&mon, seed), ITEM_ELECTRIUM_Z);
    CreateTestMon(&mon, SPECIES_PIKACHU_ORIGINAL);
    bool32 signature = FALSE;
    for (u32 seed = 0; seed < 40; seed++)
    {
        enum Item item = GetPyramidZCrystalForMon(&mon, seed);
        EXPECT(item == ITEM_ELECTRIUM_Z || item == ITEM_PIKASHUNIUM_Z);
        signature |= item == ITEM_PIKASHUNIUM_Z;
    }
    EXPECT(signature);
    CreateTestMon(&mon, SPECIES_NECROZMA_DUSK_MANE);
    SetMonMoveSlot(&mon, MOVE_PHOTON_GEYSER, 0);
    for (u32 seed = 0; seed < 40; seed++)
        EXPECT_EQ(GetPyramidZCrystalForMon(&mon, seed), ITEM_PSYCHIUM_Z);
    ZeroMonData(&mon);
    EXPECT_EQ(GetPyramidZCrystalForMon(&mon, 0), ITEM_NONE);
}

TEST("Pyramid: fixed loot uses saved entry party, not current moves or order")
{
    SetUpPyramid();
    memset(gSaveBlock2Ptr->frontier.selectedPartyMons, 0, sizeof(gSaveBlock2Ptr->frontier.selectedPartyMons));
    gSaveBlock2Ptr->frontier.selectedPartyMons[0] = 1;
    CreateTestMon(GetSavedPlayerPartyMon(0), SPECIES_PIKACHU);
    EXPECT_EQ(GetPyramidPartyZCrystal(123), ITEM_ELECTRIUM_Z);
    CreateTestMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_CHARMANDER);
    SetMonMoveSlot(&gParties[B_TRAINER_PLAYER][0], MOVE_EMBER, 0);
    EXPECT_EQ(GetPyramidPartyZCrystal(123), ITEM_ELECTRIUM_Z);
    gSaveBlock2Ptr->frontier.selectedPartyMons[0] = PARTY_SIZE + 1;
    EXPECT_EQ(GetPyramidPartyZCrystal(123), ITEM_NONE);
}

TEST("Pyramid: trainer preparation normalizes items and applies early IV penalty")
{
    u32 floor;
    PARAMETRIZE { floor = 0; }
    PARAMETRIZE { floor = 3; }
    PARAMETRIZE { floor = 6; }
    for (u32 trial = 0; trial < 32; trial++)
    {
        SetUpPyramid();
        gSaveBlock2Ptr->frontier.curChallengeBattleNum = floor;
        for (u32 trainer = B_TRAINER_OPPONENT_A; trainer <= B_TRAINER_OPPONENT_B; trainer += 2)
        {
            CreateTestMon(&gParties[trainer][0], SPECIES_PIKACHU);
            u32 item = ITEM_CHARIZARDITE_X;
            SetMonData(&gParties[trainer][0], MON_DATA_HELD_ITEM, &item);
        }
        PreparePyramidTrainerBattle();
        u32 zCount = 0, teraCount = 0;
        for (u32 trainer = B_TRAINER_OPPONENT_A; trainer <= B_TRAINER_OPPONENT_B; trainer += 2)
        {
            struct Pokemon *mon = &gParties[trainer][0];
            enum Item item = GetMonData(mon, MON_DATA_HELD_ITEM);
            EXPECT(item == ITEM_NONE || GetItemPocket(item) == POCKET_BERRIES || GetItemHoldEffect(item) == HOLD_EFFECT_Z_CRYSTAL);
            zCount += GetItemHoldEffect(item) == HOLD_EFFECT_Z_CRYSTAL;
            teraCount += GetMonData(mon, MON_DATA_TERA_TYPE) != TYPE_MYSTERY;
            for (u32 stat = 0; stat < NUM_STATS; stat++)
                EXPECT_EQ(GetMonData(mon, MON_DATA_HP_IV + stat), 1);
        }
        EXPECT(zCount <= 1);
        EXPECT(teraCount <= 1);
    }
}

TEST("Pyramid: first-floor crystal survives a full bag and repeated pickup")
{
    u32 mode = 0, round = 0;
    for (u32 m = 0; m < FRONTIER_LVL_MODE_COUNT; m++)
        for (u32 r = 0; r < TOTAL_PYRAMID_ROUNDS + 1; r++)
            PARAMETRIZE { mode = m; round = r; }
    SetUpPyramid();
    gSaveBlock2Ptr->frontier.lvlMode = mode;
    gSaveBlock2Ptr->frontier.pyramidWinStreaks[mode] = round * FRONTIER_STAGES_PER_CHALLENGE;
    memset(gSaveBlock2Ptr->frontier.pyramidRandoms, 0, sizeof(gSaveBlock2Ptr->frontier.pyramidRandoms));
    memset(gSaveBlock2Ptr->frontier.selectedPartyMons, 0, sizeof(gSaveBlock2Ptr->frontier.selectedPartyMons));
    gSaveBlock2Ptr->frontier.selectedPartyMons[0] = 1;
    CreateTestMon(GetSavedPlayerPartyMon(0), SPECIES_PIKACHU);
    // Seed 0 selects floor template 0: three trainers, then the first item.
    gSpecialVar_LastTalked = 4;
    gSpecialVar_0x8004 = BATTLE_PYRAMID_FUNC_SET_ITEM;
    CallBattlePyramidFunction();
    EXPECT_EQ(gSpecialVar_0x8000, ITEM_ELECTRIUM_Z);
    EXPECT_EQ(gSpecialVar_0x8001, 1);

    for (u32 slot = 0; slot < PYRAMID_BAG_ITEMS_COUNT; slot++)
    {
        gSaveBlock2Ptr->frontier.pyramidBag.itemId[mode][slot] = ITEM_ORAN_BERRY + slot;
        gSaveBlock2Ptr->frontier.pyramidBag.quantity[mode][slot] = 1;
    }
    u8 before[sizeof(gSaveBlock2Ptr->frontier.pyramidBag)];
    memcpy(before, &gSaveBlock2Ptr->frontier.pyramidBag, sizeof(before));
    EXPECT(!AddPyramidBagItem(gSpecialVar_0x8000, gSpecialVar_0x8001));
    EXPECT_EQ(memcmp(before, &gSaveBlock2Ptr->frontier.pyramidBag, sizeof(before)), 0);
    CallBattlePyramidFunction();
    EXPECT_EQ(gSpecialVar_0x8000, ITEM_ELECTRIUM_Z);
    gSaveBlock2Ptr->frontier.pyramidBag.itemId[mode][0] = ITEM_NONE;
    gSaveBlock2Ptr->frontier.pyramidBag.quantity[mode][0] = 0;
    EXPECT(AddPyramidBagItem(gSpecialVar_0x8000, gSpecialVar_0x8001));
    EXPECT_EQ(gSaveBlock2Ptr->frontier.pyramidBag.itemId[mode][0], ITEM_ELECTRIUM_Z);
}

TEST("Pyramid: other facilities do not receive Pyramid item or IV changes")
{
    SetUpPyramid();
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    CreateTestMon(&gParties[B_TRAINER_OPPONENT_A][0], SPECIES_CHARIZARD);
    u32 item = ITEM_CHARIZARDITE_X;
    SetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_HELD_ITEM, &item);
    struct Pokemon before = gParties[B_TRAINER_OPPONENT_A][0];
    PreparePyramidTrainerBattle();
    BalancePyramidOpponentParties();
    EXPECT_EQ(memcmp(&before, &gParties[B_TRAINER_OPPONENT_A][0], sizeof(before)), 0);
}

TEST("Pyramid: Brandon has one Z and one Tera candidate, not Mega items")
{
    SetUpPyramid();
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    for (u32 slot = 0; slot < FRONTIER_PARTY_SIZE; slot++)
        CreateTestMon(&gParties[B_TRAINER_OPPONENT_A][slot], SPECIES_PIKACHU);
    BalancePyramidOpponentParties();
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_HELD_ITEM), ITEM_ELECTRIUM_Z);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_TERA_TYPE), TYPE_MYSTERY);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][1], MON_DATA_TERA_TYPE), TYPE_ELECTRIC);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][2], MON_DATA_TERA_TYPE), TYPE_MYSTERY);
}

TEST("Pyramid: all wild rounds and level modes produce ordinary healthy legal Pokemon")
{
    u32 mode = 0, round = 0, slot = 0;
    for (u32 m = 0; m < FRONTIER_LVL_MODE_COUNT; m++)
        for (u32 r = 0; r < TOTAL_PYRAMID_ROUNDS + 1; r++)
            for (u32 s = 0; s < PYRAMID_WILD_MON_COUNT; s++)
                PARAMETRIZE { mode = m; round = r; slot = s; }
    SetUpPyramid();
    gSaveBlock2Ptr->frontier.lvlMode = mode;
    gSaveBlock2Ptr->frontier.pyramidWinStreaks[mode] = round * FRONTIER_STAGES_PER_CHALLENGE;
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 100, 0, OTID_STRUCT_PRESET(1), 31);
    struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][0];
    CreateTestMon(mon, slot + 1);
    GenerateBattlePyramidWildMon(SPECIES_NONE);
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    u32 level = GetMonData(mon, MON_DATA_LEVEL);
    const struct SpeciesInfo *info = &gSpeciesInfo[species];
    EXPECT(IsSpeciesEnabled(species));
    EXPECT(!info->isRestrictedLegendary && !info->isSubLegendary && !info->isMythical && !info->isUltraBeast && !info->isParadox);
    EXPECT(!info->isMegaEvolution && !info->isPrimalReversion && !info->isUltraBurst && !info->isGigantamax && !info->isTeraForm);
    EXPECT(level >= (mode == FRONTIER_LVL_50 ? 30 : 80) && level <= MAX_LEVEL);
    EXPECT_EQ(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP));
    EXPECT(GetSpeciesAbility(species, GetMonData(mon, MON_DATA_ABILITY_NUM)) != ABILITY_NONE);
    bool32 attack = FALSE;
    const struct LevelUpMove *learnset = GetSpeciesLevelUpLearnset(species);
    for (u32 s = 0; s < MAX_MON_MOVES; s++)
    {
        enum Move move = GetMonData(mon, MON_DATA_MOVE1 + s);
        if (move == MOVE_NONE)
            continue;
        bool32 legal = FALSE;
        for (u32 i = 0; learnset[i].move != LEVEL_UP_MOVE_END; i++)
            legal |= learnset[i].move == move && learnset[i].level <= level;
        if (!legal)
            Test_MgbaPrintf("mode=%d round=%d slot=%d species=%d level=%d move=%d", mode, round, slot, species, level, move);
        EXPECT(legal);
        attack |= GetMoveCategory(move) != DAMAGE_CATEGORY_STATUS;
    }
    EXPECT(attack);
}
