#include "global.h"
#include "test/battle.h"
#include "battle_frontier.h"
#include "frontier_gc_battle.h"
#include "malloc.h"
#include "constants/battle_frontier_mons.h"

// GameCube battle request V20 (gba-gc-extension docs/items.md section 14):
// the exporter's Life Orb (two records), the versions before it, the abilities
// that turn every item off beside it, strict rules and the Battle Frontier sets.

#define V19 BATTLE_APP_VERSION_CONSUMABLE_ITEMS
#define V20 BATTLE_APP_VERSION_MOVE_ITEMS

static void MakeMon(struct Pokemon *mon, u16 species, u8 abilityNum, u16 item)
{
    CreateMonWithIVs(mon, species, 50, 0x4D0000 + species, OTID_STRUCT_PLAYER_ID, 31);
    SetMonMoveSlot(mon, MOVE_TACKLE, 0);
    SetMonMoveSlot(mon, MOVE_NONE, 1);
    SetMonMoveSlot(mon, MOVE_NONE, 2);
    SetMonMoveSlot(mon, MOVE_NONE, 3);
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
}

static void ExportOne(struct FrontierGcExportResult *r, u8 roster, u16 species, u8 abilityNum, u16 item, bool8 strict)
{
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    MakeMon(&mons[0], species, abilityNum, item);
    MakeMon(&mons[1], SPECIES_SNORLAX, 0, ITEM_NONE);
    const struct FrontierGcExportTest t = {.roster = roster, .format = BATTLE_FORMAT_SINGLE, .count = 1,
                                           .strictItems = strict, .mons = mons};
    FrontierGcBattle_TestExport(&t, r);
    Free(mons);
}

static bool32 IsLifeOrbBlock(const BattleItem *it)
{
    const BattleItemRecord *m = &it->records[0], *h = &it->records[1];
    return it->cls == BICLASS_RECORDS && it->source == ITEM_LIFE_ORB && it->count == 2
        && m->kind == BITEM_MODIFIER && m->flags == 0 && m->mod.stage == (BMOD_FINAL | BMOD_FLOORED)
        && m->mod.percent == 130 && m->mod.pred[0].kind == BPRED_NONE && m->mod.pred[1].kind == BPRED_NONE
        && h->kind == BITEM_HP && h->flags == 0 && h->hp.timing == BITIME_AFTER_MOVE && h->hp.op == BIHP_DAMAGE
        && h->hp.divisor == 10 && h->hp.pred.kind == BPRED_NONE && h->hp.pred.arg == 0 && h->hp.reserved == 0;
}

TEST("GC V20 export: Life Orb is a final factor and an after-move loss, on both sides and in reserve")
{
    struct FrontierGcExportResult r;
    struct Pokemon *mons = AllocZeroed(4 * sizeof(struct Pokemon));
    MakeMon(&mons[0], SPECIES_SNORLAX, 0, ITEM_LIFE_ORB);
    MakeMon(&mons[1], SPECIES_WOBBUFFET, 0, ITEM_LIFE_ORB);
    MakeMon(&mons[2], SPECIES_SNORLAX, 0, ITEM_LIFE_ORB);
    MakeMon(&mons[3], SPECIES_WOBBUFFET, 0, ITEM_NONE);
    const struct FrontierGcExportTest t = {.roster = V20, .format = BATTLE_FORMAT_DOUBLE, .count = 2, .mons = mons};
    FrontierGcBattle_TestExport(&t, &r);
    EXPECT(r.built);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    EXPECT(r.request != NULL);
    if (r.request != NULL)
    {
        EXPECT_EQ(r.request->version, V20);
        EXPECT(IsLifeOrbBlock(&r.request->player[0].mon.item));
        EXPECT(IsLifeOrbBlock(&r.request->player[1].mon.item));
        EXPECT(IsLifeOrbBlock(&r.request->opponent[0].mon.item));
        EXPECT_EQ(r.request->opponent[1].mon.item.cls, BICLASS_NONE);  // holds nothing
        EXPECT_EQ(r.request->opponent[1].record.held_item, ITEM_NONE);
        EXPECT_EQ(r.request->player[0].record.held_item, ITEM_LIFE_ORB);  // the item id is kept
    }
    TRY_FREE_AND_SET_NULL(r.request);
    Free(mons);
}

TEST("GC V20 export: Life Orb before V20 stays unsupported (every item off; strict refused)")
{
    struct FrontierGcExportResult r;
    ExportOne(&r, V19, SPECIES_SNORLAX, 0, ITEM_LIFE_ORB, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
    EXPECT_EQ(r.itemReason, ITEM_LIFE_ORB);
    EXPECT(r.request != NULL && r.request->player[0].mon.item.cls == BICLASS_UNSUPPORTED);
    TRY_FREE_AND_SET_NULL(r.request);
    ExportOne(&r, V19, SPECIES_SNORLAX, 0, ITEM_LIFE_ORB, TRUE);
    EXPECT(!r.built);
    TRY_FREE_AND_SET_NULL(r.request);
    ExportOne(&r, V20, SPECIES_SNORLAX, 0, ITEM_LIFE_ORB, TRUE);
    EXPECT(r.built);  // V20 strict: run in full
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    TRY_FREE_AND_SET_NULL(r.request);
}

TEST("GC V20 export: Magic Guard, Sheer Force and Klutz beside Life Orb turn every item off")
{
    static const struct { u16 species; u8 slot; u16 ability; } holders[] = {
        {SPECIES_CLEFABLE, 1, ABILITY_MAGIC_GUARD},
        {SPECIES_NIDOKING, 2, ABILITY_SHEER_FORCE},
        {SPECIES_LOPUNNY, 1, ABILITY_KLUTZ},
    };
    struct FrontierGcExportResult r;
    for (u32 i = 0; i < ARRAY_COUNT(holders); i++)
    {
        ExportOne(&r, V20, holders[i].species, holders[i].slot, ITEM_LIFE_ORB, FALSE);
        EXPECT(r.built);
        EXPECT(r.request != NULL && r.request->player[0].record.ability == holders[i].ability);
        EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
        EXPECT_EQ(r.itemReason, holders[i].ability);
        TRY_FREE_AND_SET_NULL(r.request);
        ExportOne(&r, V20, holders[i].species, holders[i].slot, ITEM_LIFE_ORB, TRUE);
        EXPECT(!r.built);
        TRY_FREE_AND_SET_NULL(r.request);
    }
    // The gate is the after-move record's: Sheer Force with Leftovers stays simulated.
    ExportOne(&r, V20, SPECIES_NIDOKING, 2, ITEM_LEFTOVERS, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    TRY_FREE_AND_SET_NULL(r.request);
    // Another ability beside Life Orb (Poison Point): simulated.
    ExportOne(&r, V20, SPECIES_NIDOKING, 0, ITEM_LIFE_ORB, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    EXPECT(r.request != NULL && IsLifeOrbBlock(&r.request->player[0].mon.item));
    TRY_FREE_AND_SET_NULL(r.request);
}

#define TALLY_CHUNK 120

// Battle Frontier sets against a plain Snorlax: what V20 adds (Life Orb holders).
TEST("GC V20 export tally: Battle Frontier sets with Life Orb")
{
    u32 chunk, built = 0, lifeOrb = 0, sim19 = 0, sim20 = 0, orbSim20 = 0, orbOffAbility = 0, strict20 = 0;
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    for (chunk = 0; chunk < (NUM_FRONTIER_MONS + TALLY_CHUNK - 1) / TALLY_CHUNK; chunk++)
        PARAMETRIZE_LABEL("%d", chunk) {}
    chunk = gFunctionTestRunnerState->runParameter;
    for (u32 set = chunk * TALLY_CHUNK; set < (chunk + 1) * TALLY_CHUNK && set < NUM_FRONTIER_MONS; set++)
    {
        struct FrontierGcExportResult r19, r20;
        const bool32 orb = gBattleFrontierMons[set].heldItem == ITEM_LIFE_ORB;
        CreateFacilityMon(&gBattleFrontierMons[set], 50, 31, 0x12345678, 0, &mons[0]);
        MakeMon(&mons[1], SPECIES_SNORLAX, 0, ITEM_NONE);
        const struct FrontierGcExportTest t19 = {.roster = V19, .format = BATTLE_FORMAT_SINGLE, .count = 1, .mons = mons};
        const struct FrontierGcExportTest t20 = {.roster = V20, .format = BATTLE_FORMAT_SINGLE, .count = 1, .mons = mons};
        FrontierGcBattle_TestExport(&t19, &r19);
        FrontierGcBattle_TestExport(&t20, &r20);
        if (r20.built && r20.request != NULL)
        {
            built++;
            lifeOrb += orb;
            sim19 += r19.itemState == FGC_ITEMS_SIMULATED;
            sim20 += r20.itemState == FGC_ITEMS_SIMULATED;
            orbSim20 += orb && r20.itemState == FGC_ITEMS_SIMULATED;
            orbOffAbility += orb && r20.itemState == FGC_ITEMS_OFF_ABILITY;
            if (orb && r20.itemState == FGC_ITEMS_SIMULATED)
                EXPECT(IsLifeOrbBlock(&r20.request->player[0].mon.item));
            // Nothing V19 simulated changes in V20 but Life Orb itself.
            if (!orb)
                EXPECT_EQ(r19.itemState, r20.itemState);
        }
        TRY_FREE_AND_SET_NULL(r19.request);
        TRY_FREE_AND_SET_NULL(r20.request);
        const struct FrontierGcExportTest ts = {.roster = V20, .format = BATTLE_FORMAT_SINGLE, .count = 1,
                                                .strictItems = TRUE, .mons = mons};
        FrontierGcBattle_TestExport(&ts, &r20);
        strict20 += r20.built;
        TRY_FREE_AND_SET_NULL(r20.request);
    }
    Free(mons);
    Test_MgbaPrintf("TALLY20 chunk=%d built=%d lifeOrb=%d sim19=%d sim20=%d orbSim20=%d orbOffAbility=%d strict20=%d",
                    chunk, built, lifeOrb, sim19, sim20, orbSim20, orbOffAbility, strict20);
}
