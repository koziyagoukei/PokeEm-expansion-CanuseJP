#include "global.h"
#include "test/battle.h"
#include "battle_frontier.h"
#include "frontier_gc_battle.h"
#include "malloc.h"
#include "constants/battle_frontier_mons.h"

// M4-B1c-0c (gba-gc-extension docs/items.md section 15.6): what a GC battle
// of each Battle Frontier set can run, measured one indicator at a time, for
// request V19 and V20 under the same conditions. Every gBattleFrontierMons
// set is the player's lead (level 50, IVs 31, personality 0x12345678) in a
// Single against a plain Snorlax (Immunity, Tackle, no item). The exporter's
// default rules (non-strict) build the request; the strict figure asks for
// strict items, abilities and moves. Not a success rate of facility battles:
// those have three Pokemon a side and other foes.
//
//   built      the request was built (non-strict)
//   items      held items take part as they are (not normalised; no item counts)
//   itemNone   holds nothing
//   offItem    every item off: an item is not simulated
//   offAbility every item off: an ability that reads or moves items
//   ability    the lead's ability is run (not normalised)
//   moves      every move of the lead is run (none excluded)
//   strict     strict items + abilities + moves: the battle starts
//   orb        holds Life Orb
//   orbRun     Life Orb's final factor and HP loss run (V20 only)

#define V19 BATTLE_APP_VERSION_CONSUMABLE_ITEMS
#define V20 BATTLE_APP_VERSION_MOVE_ITEMS
#define COVERAGE_CHUNK 120

enum { C_BUILT, C_ITEMS, C_NONE, C_OFF_ITEM, C_OFF_ABILITY, C_ABILITY, C_MOVES, C_STRICT, C_ORB, C_ORB_RUN, C_COUNT };

static void Foe(struct Pokemon *mon)
{
    u8 abilityNum = 0;
    u16 item = ITEM_NONE;
    CreateMonWithIVs(mon, SPECIES_SNORLAX, 50, 0x4D0000 + SPECIES_SNORLAX, OTID_STRUCT_PLAYER_ID, 31);
    SetMonMoveSlot(mon, MOVE_TACKLE, 0);
    SetMonMoveSlot(mon, MOVE_NONE, 1);
    SetMonMoveSlot(mon, MOVE_NONE, 2);
    SetMonMoveSlot(mon, MOVE_NONE, 3);
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
    SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
}

static bool32 MovesRun(const BattleEntry *e)
{
    for (u32 m = 0; m < MAX_MON_MOVES; m++)
        if (e->record.moves[m] != MOVE_NONE && e->mon.moves[m].unsupported != 0)
            return FALSE;
    return TRUE;
}

static void Measure(struct Pokemon *mons, u8 roster, bool32 orb, u32 *c)
{
    struct FrontierGcExportResult r;
    const struct FrontierGcExportTest t = {.roster = roster, .format = BATTLE_FORMAT_SINGLE, .count = 1, .mons = mons};
    const struct FrontierGcExportTest ts = {.roster = roster, .format = BATTLE_FORMAT_SINGLE, .count = 1,
                                            .strictItems = TRUE, .strictAll = TRUE, .mons = mons};
    FrontierGcBattle_TestExport(&t, &r);
    c[C_ORB] += orb;
    if (r.built && r.request != NULL)
    {
        const BattleEntry *lead = &r.request->player[0];
        c[C_BUILT]++;
        c[C_ITEMS] += r.itemState == FGC_ITEMS_SIMULATED;
        c[C_NONE] += lead->record.held_item == ITEM_NONE;
        c[C_OFF_ITEM] += r.itemState == FGC_ITEMS_OFF_ITEM;
        c[C_OFF_ABILITY] += r.itemState == FGC_ITEMS_OFF_ABILITY;
        c[C_ABILITY] += lead->mon.ability.cls != BACLASS_UNSUPPORTED;
        c[C_MOVES] += MovesRun(lead);
        c[C_ORB_RUN] += orb && r.itemState == FGC_ITEMS_SIMULATED && lead->mon.item.cls == BICLASS_RECORDS
                     && lead->mon.item.count == 2 && lead->mon.item.records[1].hp.timing == BITIME_AFTER_MOVE;
    }
    TRY_FREE_AND_SET_NULL(r.request);
    FrontierGcBattle_TestExport(&ts, &r);
    if (r.built && r.request != NULL && r.request->player[0].mon.ability.cls != BACLASS_UNSUPPORTED
     && MovesRun(&r.request->player[0]) && r.request->opponent[0].mon.ability.cls != BACLASS_UNSUPPORTED)
        c[C_STRICT]++;
    TRY_FREE_AND_SET_NULL(r.request);
}

TEST("GC V20 export coverage: Battle Frontier sets, V19 and V20 by indicator")
{
    u32 chunk, v19[C_COUNT] = {0}, v20[C_COUNT] = {0}, sets = 0;
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    for (chunk = 0; chunk < (NUM_FRONTIER_MONS + COVERAGE_CHUNK - 1) / COVERAGE_CHUNK; chunk++)
        PARAMETRIZE_LABEL("%d", chunk) {}
    chunk = gFunctionTestRunnerState->runParameter;
    for (u32 set = chunk * COVERAGE_CHUNK; set < (chunk + 1) * COVERAGE_CHUNK && set < NUM_FRONTIER_MONS; set++)
    {
        const bool32 orb = gBattleFrontierMons[set].heldItem == ITEM_LIFE_ORB;
        CreateFacilityMon(&gBattleFrontierMons[set], 50, 31, 0x12345678, 0, &mons[0]);
        Foe(&mons[1]);
        sets++;
        Measure(mons, V19, orb, v19);
        Measure(mons, V20, orb, v20);
    }
    Free(mons);
    // V20 changes only Life Orb: nothing gets worse.
    EXPECT_GE(v20[C_ITEMS], v19[C_ITEMS]);
    EXPECT_EQ(v20[C_BUILT], v19[C_BUILT]);
    EXPECT_EQ(v20[C_ABILITY], v19[C_ABILITY]);
    EXPECT_EQ(v20[C_MOVES], v19[C_MOVES]);
    EXPECT_EQ(v19[C_ORB_RUN], 0);
    for (u32 v = 0; v < 2; v++)
    {
        const u32 *c = v ? v20 : v19;
        Test_MgbaPrintf("COVER V%d chunk=%d sets=%d built=%d items=%d itemNone=%d offItem=%d offAbility=%d ability=%d "
                        "moves=%d strict=%d orb=%d orbRun=%d", v ? 20 : 19, chunk, sets, c[C_BUILT], c[C_ITEMS],
                        c[C_NONE], c[C_OFF_ITEM], c[C_OFF_ABILITY], c[C_ABILITY], c[C_MOVES], c[C_STRICT],
                        c[C_ORB], c[C_ORB_RUN]);
    }
}
