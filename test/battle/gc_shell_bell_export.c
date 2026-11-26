#include "global.h"
#include "test/battle.h"
#include "battle_frontier.h"
#include "frontier_gc_battle.h"
#include "item.h"
#include "malloc.h"
#include "constants/battle_frontier_mons.h"

// GameCube battle request V21 (gba-gc-extension docs/shell-bell.md): the
// exporter's Shell Bell (one after-move heal record), the versions before it,
// the abilities beside it, the moves the GC does not run, strict rules, Life
// Orb unchanged, and the Battle Frontier sets.

#define V19 BATTLE_APP_VERSION_CONSUMABLE_ITEMS
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

// The holder, then a plain Snorlax (or `foe`) as the opponent.
static void ExportPair(struct FrontierGcExportResult *r, u8 roster, u16 species, u8 abilityNum, u16 item,
                       u16 foe, u8 foeAbilityNum, bool8 strict, bool8 strictAll)
{
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    MakeMon(&mons[0], species, abilityNum, item);
    MakeMon(&mons[1], foe, foeAbilityNum, ITEM_NONE);
    const struct FrontierGcExportTest t = {.roster = roster, .format = BATTLE_FORMAT_SINGLE, .count = 1,
                                           .strictItems = strict, .strictAll = strictAll, .mons = mons};
    FrontierGcBattle_TestExport(&t, r);
    Free(mons);
}

static void ExportOne(struct FrontierGcExportResult *r, u8 roster, u16 species, u8 abilityNum, u16 item, bool8 strict)
{
    ExportPair(r, roster, species, abilityNum, item, SPECIES_SNORLAX, 0, strict, FALSE);
}

static bool32 IsShellBellBlock(const BattleItem *it)
{
    const BattleItemRecord *h = &it->records[0];
    return it->cls == BICLASS_RECORDS && it->source == ITEM_SHELL_BELL && it->count == 1
        && h->kind == BITEM_HP && h->flags == 0 && h->hp.timing == BITIME_AFTER_MOVE && h->hp.op == BIHP_HEAL_DEALT
        && h->hp.divisor == GetItemHoldEffectParam(ITEM_SHELL_BELL) && h->hp.pred.kind == BPRED_NONE
        && h->hp.pred.arg == 0 && h->hp.reserved == 0;
}

static bool32 IsLifeOrbBlock(const BattleItem *it)
{
    const BattleItemRecord *m = &it->records[0], *h = &it->records[1];
    return it->cls == BICLASS_RECORDS && it->source == ITEM_LIFE_ORB && it->count == 2
        && m->kind == BITEM_MODIFIER && m->mod.stage == (BMOD_FINAL | BMOD_FLOORED) && m->mod.percent == 130
        && h->kind == BITEM_HP && h->hp.timing == BITIME_AFTER_MOVE && h->hp.op == BIHP_DAMAGE && h->hp.divisor == 10;
}

TEST("GC V21 export: Shell Bell is one after-move heal of the item's own divisor, on both sides and in reserve")
{
    struct FrontierGcExportResult r;
    struct Pokemon *mons = AllocZeroed(4 * sizeof(struct Pokemon));
    EXPECT_EQ(GetItemHoldEffect(ITEM_SHELL_BELL), HOLD_EFFECT_SHELL_BELL);
    EXPECT_EQ(GetItemHoldEffectParam(ITEM_SHELL_BELL), 8);
    MakeMon(&mons[0], SPECIES_SNORLAX, 0, ITEM_SHELL_BELL);
    MakeMon(&mons[1], SPECIES_WOBBUFFET, 0, ITEM_SHELL_BELL);
    MakeMon(&mons[2], SPECIES_SNORLAX, 0, ITEM_SHELL_BELL);
    MakeMon(&mons[3], SPECIES_WOBBUFFET, 0, ITEM_NONE);
    const struct FrontierGcExportTest t = {.roster = V21, .format = BATTLE_FORMAT_DOUBLE, .count = 2, .mons = mons};
    FrontierGcBattle_TestExport(&t, &r);
    EXPECT(r.built);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    EXPECT(r.request != NULL);
    if (r.request != NULL)
    {
        EXPECT_EQ(r.request->version, V21);
        EXPECT(IsShellBellBlock(&r.request->player[0].mon.item));
        EXPECT(IsShellBellBlock(&r.request->player[1].mon.item));
        EXPECT(IsShellBellBlock(&r.request->opponent[0].mon.item));
        EXPECT_EQ(r.request->opponent[1].mon.item.cls, BICLASS_NONE);  // holds nothing
        EXPECT_EQ(r.request->opponent[1].record.held_item, ITEM_NONE);
        EXPECT_EQ(r.request->player[0].record.held_item, ITEM_SHELL_BELL);  // the item id is kept
        EXPECT(r.request->rules & RULE_NO_GIMMICKS);
        EXPECT(!(r.request->rules & RULE_IGNORE_ITEMS));
    }
    TRY_FREE_AND_SET_NULL(r.request);
    Free(mons);
}

TEST("GC V21 export: Shell Bell before V21 stays unsupported (every item off; strict refused)")
{
    static const u8 rosters[] = {V19, V20};
    struct FrontierGcExportResult r;
    for (u32 i = 0; i < ARRAY_COUNT(rosters); i++)
    {
        ExportOne(&r, rosters[i], SPECIES_SNORLAX, 0, ITEM_SHELL_BELL, FALSE);
        EXPECT(r.built);
        EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
        EXPECT_EQ(r.itemReason, ITEM_SHELL_BELL);
        EXPECT(r.request != NULL && r.request->player[0].mon.item.cls == BICLASS_UNSUPPORTED
               && r.request->player[0].mon.item.count == 0 && (r.request->rules & RULE_IGNORE_ITEMS));
        TRY_FREE_AND_SET_NULL(r.request);
        ExportOne(&r, rosters[i], SPECIES_SNORLAX, 0, ITEM_SHELL_BELL, TRUE);
        EXPECT(!r.built);
        TRY_FREE_AND_SET_NULL(r.request);
    }
    ExportOne(&r, V21, SPECIES_SNORLAX, 0, ITEM_SHELL_BELL, TRUE);
    EXPECT(r.built);  // V21 strict: run in full
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    EXPECT(r.request != NULL && IsShellBellBlock(&r.request->player[0].mon.item));
    TRY_FREE_AND_SET_NULL(r.request);
}

TEST("GC V21 export: Magic Guard keeps Shell Bell; Sheer Force, Klutz, the item takers and Trace turn every item off")
{
    static const struct { u16 species; u8 slot; u16 ability; } off[] = {
        {SPECIES_NIDOKING, 2, ABILITY_SHEER_FORCE},  // skips the after-move stage for its boosted moves
        {SPECIES_LOPUNNY, 1, ABILITY_KLUTZ},
        {SPECIES_SNEASEL, 2, ABILITY_PICKPOCKET},
        {SPECIES_FENNEKIN, 2, ABILITY_MAGICIAN},
        {SPECIES_PORYGON, 0, ABILITY_TRACE},         // could copy Sheer Force
    };
    struct FrontierGcExportResult r;
    for (u32 i = 0; i < ARRAY_COUNT(off); i++)
    {
        ExportOne(&r, V21, off[i].species, off[i].slot, ITEM_SHELL_BELL, FALSE);
        EXPECT(r.built);
        EXPECT(r.request != NULL && r.request->player[0].record.ability == off[i].ability);
        EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
        EXPECT_EQ(r.itemReason, off[i].ability);
        EXPECT(r.request != NULL && (r.request->rules & RULE_IGNORE_ITEMS));
        TRY_FREE_AND_SET_NULL(r.request);
        ExportOne(&r, V21, off[i].species, off[i].slot, ITEM_SHELL_BELL, TRUE);
        EXPECT(!r.built);
        TRY_FREE_AND_SET_NULL(r.request);
    }
    // Magic Guard stops damage, not a heal (TryShellBell has no Magic Guard check): simulated, strict too.
    ExportOne(&r, V21, SPECIES_CLEFABLE, 1, ITEM_SHELL_BELL, FALSE);
    EXPECT(r.request != NULL && r.request->player[0].record.ability == ABILITY_MAGIC_GUARD);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    EXPECT(r.request != NULL && IsShellBellBlock(&r.request->player[0].mon.item));
    TRY_FREE_AND_SET_NULL(r.request);
    ExportOne(&r, V21, SPECIES_CLEFABLE, 1, ITEM_SHELL_BELL, TRUE);
    EXPECT(r.built);
    TRY_FREE_AND_SET_NULL(r.request);
    // Life Orb with Magic Guard is still off in V21 (its loss is damage).
    ExportOne(&r, V21, SPECIES_CLEFABLE, 1, ITEM_LIFE_ORB, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
    TRY_FREE_AND_SET_NULL(r.request);
    // The gate is the after-move record's: Sheer Force or Trace with Leftovers stays simulated.
    ExportOne(&r, V21, SPECIES_NIDOKING, 2, ITEM_LEFTOVERS, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    TRY_FREE_AND_SET_NULL(r.request);
    ExportOne(&r, V21, SPECIES_PORYGON, 0, ITEM_LEFTOVERS, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    TRY_FREE_AND_SET_NULL(r.request);
    // Another ability beside Shell Bell (Poison Point): simulated.
    ExportOne(&r, V21, SPECIES_NIDOKING, 0, ITEM_SHELL_BELL, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    EXPECT(r.request != NULL && IsShellBellBlock(&r.request->player[0].mon.item));
    TRY_FREE_AND_SET_NULL(r.request);
    // A foe's Sheer Force does not touch the holder's bell: simulated.
    ExportPair(&r, V21, SPECIES_SNORLAX, 0, ITEM_SHELL_BELL, SPECIES_NIDOKING, 2, FALSE, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    TRY_FREE_AND_SET_NULL(r.request);
}

// Neutralizing Gas is an ability the GC does not run: the existing ability
// policy sends it UNSUPPORTED (shown as not simulated; it does nothing), and
// under strict abilities the GC refuses the request (battle_init:
// BI_UNSUPPORTED_ABILITY). The item stays as it is either way.
TEST("GC V21 export: Neutralizing Gas beside Shell Bell follows the ability policy, the item is kept")
{
    struct FrontierGcExportResult r;
    for (u32 strict = 0; strict < 2; strict++)
    {
        ExportPair(&r, V21, SPECIES_SNORLAX, 0, ITEM_SHELL_BELL, SPECIES_KOFFING, 1, strict, strict);
        EXPECT(r.built);
        EXPECT(r.request != NULL);
        if (r.request != NULL)
        {
            EXPECT_EQ(r.request->opponent[0].record.ability, ABILITY_NEUTRALIZING_GAS);
            EXPECT_EQ(r.request->opponent[0].mon.ability.cls, BACLASS_UNSUPPORTED);
            EXPECT_EQ(!!(r.request->rules & RULE_STRICT_ABILITIES), strict);
            EXPECT(IsShellBellBlock(&r.request->player[0].mon.item));
        }
        EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
        TRY_FREE_AND_SET_NULL(r.request);
    }
}

// The moves that would change Shell Bell (Heal Block, Psychic Noise), take or
// block items (Embargo, Thief, Covet, Knock Off, Trick), suppress abilities
// (Gastro Acid) or hit later (Future Sight) are moves the GC does not run.
TEST("GC V21 export: moves that would change Shell Bell are not run by the GC")
{
    static const u16 moves[] = {MOVE_HEAL_BLOCK, MOVE_PSYCHIC_NOISE, MOVE_EMBARGO, MOVE_GASTRO_ACID, MOVE_THIEF,
                                MOVE_COVET, MOVE_KNOCK_OFF, MOVE_TRICK, MOVE_SWITCHEROO, MOVE_FUTURE_SIGHT};
    for (u32 i = 0; i < ARRAY_COUNT(moves); i++)
        for (u8 format = BATTLE_FORMAT_SINGLE; format <= BATTLE_FORMAT_ROTATION; format++)
            EXPECT_NE(FrontierGcBattle_TestMoveSupport(V21, format, moves[i]), 0);
}

TEST("GC V21 export: Life Orb in V21 is V20's")
{
    struct FrontierGcExportResult r20, r21;
    ExportOne(&r20, V20, SPECIES_SNORLAX, 0, ITEM_LIFE_ORB, FALSE);
    ExportOne(&r21, V21, SPECIES_SNORLAX, 0, ITEM_LIFE_ORB, FALSE);
    EXPECT(r20.built && r21.built);
    EXPECT_EQ(r20.itemState, r21.itemState);
    EXPECT(r20.request != NULL && r21.request != NULL);
    if (r20.request != NULL && r21.request != NULL)
    {
        EXPECT(IsLifeOrbBlock(&r21.request->player[0].mon.item));
        EXPECT_EQ(memcmp(&r20.request->player[0].mon.item, &r21.request->player[0].mon.item, sizeof(BattleItem)), 0);
        EXPECT_EQ(r21.request->version, V21);
    }
    TRY_FREE_AND_SET_NULL(r20.request);
    TRY_FREE_AND_SET_NULL(r21.request);
}

#define TALLY_CHUNK 120

// Battle Frontier sets against a plain Snorlax: what V21 adds (Shell Bell
// holders: none in this build's sets), and that nothing else changes.
TEST("GC V21 export tally: Battle Frontier sets with Shell Bell")
{
    u32 chunk, built20 = 0, built21 = 0, bell = 0, orb = 0, sim20 = 0, sim21 = 0, orbSim21 = 0, off21 = 0,
        strict20 = 0, strict21 = 0, same = 0;
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    for (chunk = 0; chunk < (NUM_FRONTIER_MONS + TALLY_CHUNK - 1) / TALLY_CHUNK; chunk++)
        PARAMETRIZE_LABEL("%d", chunk) {}
    chunk = gFunctionTestRunnerState->runParameter;
    for (u32 set = chunk * TALLY_CHUNK; set < (chunk + 1) * TALLY_CHUNK && set < NUM_FRONTIER_MONS; set++)
    {
        struct FrontierGcExportResult r20, r21;
        const bool32 isBell = gBattleFrontierMons[set].heldItem == ITEM_SHELL_BELL;
        const bool32 isOrb = gBattleFrontierMons[set].heldItem == ITEM_LIFE_ORB;
        CreateFacilityMon(&gBattleFrontierMons[set], 50, 31, 0x12345678, 0, &mons[0]);
        MakeMon(&mons[1], SPECIES_SNORLAX, 0, ITEM_NONE);
        const struct FrontierGcExportTest t20 = {.roster = V20, .format = BATTLE_FORMAT_SINGLE, .count = 1, .mons = mons};
        const struct FrontierGcExportTest t21 = {.roster = V21, .format = BATTLE_FORMAT_SINGLE, .count = 1, .mons = mons};
        FrontierGcBattle_TestExport(&t20, &r20);
        FrontierGcBattle_TestExport(&t21, &r21);
        built20 += r20.built;
        built21 += r21.built;
        if (r21.built && r21.request != NULL)
        {
            bell += isBell;
            orb += isOrb;
            sim20 += r20.itemState == FGC_ITEMS_SIMULATED;
            sim21 += r21.itemState == FGC_ITEMS_SIMULATED;
            off21 += r21.itemState != FGC_ITEMS_SIMULATED;
            orbSim21 += isOrb && r21.itemState == FGC_ITEMS_SIMULATED;
            if (isBell && r21.itemState == FGC_ITEMS_SIMULATED)
                EXPECT(IsShellBellBlock(&r21.request->player[0].mon.item));
            // Nothing V20 exported changes in V21 but Shell Bell itself (the item block and its state).
            if (!isBell && r20.request != NULL)
            {
                EXPECT_EQ(r20.itemState, r21.itemState);
                EXPECT_EQ(memcmp(&r20.request->player[0].mon.item, &r21.request->player[0].mon.item,
                                 sizeof(BattleItem)), 0);
                same++;
            }
        }
        TRY_FREE_AND_SET_NULL(r20.request);
        TRY_FREE_AND_SET_NULL(r21.request);
        const struct FrontierGcExportTest s20 = {.roster = V20, .format = BATTLE_FORMAT_SINGLE, .count = 1,
                                                 .strictItems = TRUE, .mons = mons};
        const struct FrontierGcExportTest s21 = {.roster = V21, .format = BATTLE_FORMAT_SINGLE, .count = 1,
                                                 .strictItems = TRUE, .mons = mons};
        FrontierGcBattle_TestExport(&s20, &r20);
        FrontierGcBattle_TestExport(&s21, &r21);
        strict20 += r20.built;
        strict21 += r21.built;
        TRY_FREE_AND_SET_NULL(r20.request);
        TRY_FREE_AND_SET_NULL(r21.request);
    }
    Free(mons);
    EXPECT_EQ(built20, built21);
    EXPECT_EQ(sim20, sim21);
    EXPECT_EQ(strict20, strict21);
    Test_MgbaPrintf("TALLY21 chunk=%d built20=%d built21=%d bell=%d orb=%d sim20=%d sim21=%d off21=%d orbSim21=%d "
                    "strict20=%d strict21=%d same=%d",
                    chunk, built20, built21, bell, orb, sim20, sim21, off21, orbSim21, strict20, strict21, same);
}
