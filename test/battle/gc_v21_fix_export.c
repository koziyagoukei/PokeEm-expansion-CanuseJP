#include "global.h"
#include "test/battle.h"
#include "frontier_gc_battle.h"
#include "malloc.h"

// M4-B1c-1b-fix (gba-gc-extension docs/shell-bell.md section 12): Imposter
// copies its target's ability on entry, Sheer Force included, and Sheer Force
// skips Shell Bell after the moves it boosts. The GC runs neither, so a V21
// Shell Bell holder with Imposter turns every item off (strict items:
// refused). Imposter with no item or another item keeps its V20 verdict.

#define V20 BATTLE_APP_VERSION_MOVE_ITEMS
#define V21 BATTLE_APP_VERSION_AFTER_MOVE_HEAL

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

// Single: the lead (`species`, `abilityNum`, `item`) against `foe` (`foeAbilityNum`, no item).
static void Export(struct FrontierGcExportResult *r, u8 roster, u16 species, u8 abilityNum, u16 item, u16 foe,
                   u8 foeAbilityNum, bool8 strict)
{
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    MakeMon(&mons[0], species, abilityNum, item);
    MakeMon(&mons[1], foe, foeAbilityNum, ITEM_NONE);
    const struct FrontierGcExportTest t = {.roster = roster, .format = BATTLE_FORMAT_SINGLE, .count = 1,
                                           .strictItems = strict, .mons = mons};
    FrontierGcBattle_TestExport(&t, r);
    Free(mons);
}

TEST("GC V21 fix export: Imposter with Shell Bell turns every item off; strict items refuse it")
{
    static const struct { u16 species; u8 slot; } foes[] = {
        {SPECIES_NIDOKING, 2},  // Sheer Force, which Imposter would copy
        {SPECIES_SNORLAX, 0},   // the gate is the holder's, whatever the foe
    };
    struct FrontierGcExportResult r;
    for (u32 i = 0; i < ARRAY_COUNT(foes); i++)
    {
        Export(&r, V21, SPECIES_DITTO, 2, ITEM_SHELL_BELL, foes[i].species, foes[i].slot, FALSE);
        EXPECT(r.built);
        EXPECT(r.request != NULL && r.request->player[0].record.ability == ABILITY_IMPOSTER);
        EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
        EXPECT_EQ(r.itemReason, ABILITY_IMPOSTER);
        EXPECT(r.request != NULL && (r.request->rules & RULE_IGNORE_ITEMS));  // normalised and announced, not lost
        TRY_FREE_AND_SET_NULL(r.request);
        Export(&r, V21, SPECIES_DITTO, 2, ITEM_SHELL_BELL, foes[i].species, foes[i].slot, TRUE);
        EXPECT(!r.built);  // strict items alone refuse it
        TRY_FREE_AND_SET_NULL(r.request);
    }
}

TEST("GC V21 fix export: Imposter with no item, Leftovers or Life Orb keeps its verdict")
{
    static const u16 items[] = {ITEM_NONE, ITEM_LEFTOVERS, ITEM_LIFE_ORB};
    struct FrontierGcExportResult r, r20;
    for (u32 i = 0; i < ARRAY_COUNT(items); i++)
    {
        Export(&r, V21, SPECIES_DITTO, 2, items[i], SPECIES_NIDOKING, 2, FALSE);
        Export(&r20, V20, SPECIES_DITTO, 2, items[i], SPECIES_NIDOKING, 2, FALSE);
        EXPECT(r.built && r20.built);
        EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
        EXPECT_EQ(r.itemState, r20.itemState);
        TRY_FREE_AND_SET_NULL(r.request);
        TRY_FREE_AND_SET_NULL(r20.request);
    }
}

TEST("GC V21 fix export: Shell Bell in V20 stays unsupported with Imposter; Magic Guard keeps the bell")
{
    struct FrontierGcExportResult r;
    Export(&r, V20, SPECIES_DITTO, 2, ITEM_SHELL_BELL, SPECIES_SNORLAX, 0, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
    EXPECT_EQ(r.itemReason, ITEM_SHELL_BELL);
    TRY_FREE_AND_SET_NULL(r.request);
    Export(&r, V21, SPECIES_CLEFABLE, 1, ITEM_SHELL_BELL, SPECIES_SNORLAX, 0, TRUE);
    EXPECT(r.built);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    TRY_FREE_AND_SET_NULL(r.request);
    // The other gates beside Shell Bell are kept: Sheer Force and Trace.
    Export(&r, V21, SPECIES_NIDOKING, 2, ITEM_SHELL_BELL, SPECIES_SNORLAX, 0, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
    TRY_FREE_AND_SET_NULL(r.request);
    Export(&r, V21, SPECIES_PORYGON, 0, ITEM_SHELL_BELL, SPECIES_SNORLAX, 0, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
    TRY_FREE_AND_SET_NULL(r.request);
}

// The moves that copy or replace abilities could also give a holder Sheer
// Force; the GC does not run them.
TEST("GC V21 fix export: Transform, Skill Swap, Role Play, Entrainment and Doodle are not run by the GC")
{
    static const u16 moves[] = {MOVE_TRANSFORM, MOVE_SKILL_SWAP, MOVE_ROLE_PLAY, MOVE_ENTRAINMENT, MOVE_DOODLE};
    for (u32 i = 0; i < ARRAY_COUNT(moves); i++)
        for (u8 format = BATTLE_FORMAT_SINGLE; format <= BATTLE_FORMAT_ROTATION; format++)
            EXPECT_NE(FrontierGcBattle_TestMoveSupport(V21, format, moves[i]), 0);
}
