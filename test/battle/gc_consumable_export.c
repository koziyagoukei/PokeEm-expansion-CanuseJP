#include "global.h"
#include "test/battle.h"
#include "battle_frontier.h"
#include "frontier_gc_battle.h"
#include "malloc.h"
#include "constants/battle_frontier_mons.h"

// GameCube battle request V19 (gba-gc-extension docs/items.md section 12):
// the exporter's own judgement of consumable items (FrontierGcBattle_Test*):
// the records of each item, the version, the abilities that turn every item
// off, strict rules, and the Battle Frontier sets (tally).

#define V18 BATTLE_APP_VERSION_END_TURN_ITEMS
#define V19 BATTLE_APP_VERSION_CONSUMABLE_ITEMS

static void MakeMon(struct Pokemon *mon, u16 species, u16 item, u16 move)
{
    CreateMonWithIVs(mon, species, 50, 0x4D0000 + species, OTID_STRUCT_PLAYER_ID, 31);
    SetMonMoveSlot(mon, move, 0);
    SetMonMoveSlot(mon, MOVE_NONE, 1);
    SetMonMoveSlot(mon, MOVE_NONE, 2);
    SetMonMoveSlot(mon, MOVE_NONE, 3);
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
}

static void SetAbilityNum(struct Pokemon *mon, u8 num)
{
    SetMonData(mon, MON_DATA_ABILITY_NUM, &num);
}

// A V19 (or older) Single: the player's Pokemon holds `item`, the foe a plain Snorlax.
static void ExportOne(struct FrontierGcExportResult *r, u8 roster, u16 species, u8 abilityNum, u16 item,
                      bool8 strict)
{
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    MakeMon(&mons[0], species, item, MOVE_TACKLE);
    SetAbilityNum(&mons[0], abilityNum);
    MakeMon(&mons[1], SPECIES_SNORLAX, ITEM_NONE, MOVE_TACKLE);
    SetAbilityNum(&mons[1], 0);
    const struct FrontierGcExportTest t = {.roster = roster, .format = BATTLE_FORMAT_SINGLE, .count = 1,
                                           .strictItems = strict, .mons = mons};
    FrontierGcBattle_TestExport(&t, r);
    Free(mons);
}

static const BattleItem *PlayerItem(const struct FrontierGcExportResult *r)
{
    return r->request != NULL ? &r->request->player[0].mon.item : NULL;
}

TEST("GC V19 export: HP berries, status berries and Focus Sash as records used up when they act")
{
    static const struct { u16 item; u8 kind, a, b; } cases[] = {
        // BITEM_HP: op, amount or divisor; BITEM_CURE: status mask, volatile mask.
        {ITEM_ORAN_BERRY, BITEM_HP, BIHP_HEAL_FIXED, 10},
        {ITEM_SITRUS_BERRY, BITEM_HP, BIHP_HEAL, 4},
        {ITEM_CHERI_BERRY, BITEM_CURE, 1u << BST_PARALYSIS, 0},
        {ITEM_CHESTO_BERRY, BITEM_CURE, 1u << BST_SLEEP, 0},
        {ITEM_PECHA_BERRY, BITEM_CURE, 1u << BST_POISON | 1u << BST_TOXIC, 0},
        {ITEM_RAWST_BERRY, BITEM_CURE, 1u << BST_BURN, 0},
        {ITEM_ASPEAR_BERRY, BITEM_CURE, 1u << BST_FREEZE | 1u << BST_FROSTBITE, 0},
        {ITEM_PERSIM_BERRY, BITEM_CURE, 0, BIVOL_CONFUSION},
        {ITEM_LUM_BERRY, BITEM_CURE, 0xfe, BIVOL_CONFUSION},
        {ITEM_FOCUS_SASH, BITEM_ENDURE, BIEND_FULL_HP, 0},
    };
    struct FrontierGcExportResult r;
    for (u32 i = 0; i < ARRAY_COUNT(cases); i++)
    {
        ExportOne(&r, V19, SPECIES_WOBBUFFET, 0, cases[i].item, FALSE);
        EXPECT(r.built);
        EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
        const BattleItem *it = PlayerItem(&r);
        EXPECT(it != NULL);
        if (it != NULL)
        {
            const BattleItemRecord *x = &it->records[0];
            EXPECT_EQ(it->cls, BICLASS_RECORDS);
            EXPECT_EQ(it->source, cases[i].item);
            EXPECT_EQ(it->count, 1);
            EXPECT_EQ(x->kind, cases[i].kind);
            EXPECT_EQ(x->flags, BIREC_CONSUMED);
            if (x->kind == BITEM_HP)
            {
                EXPECT_EQ(x->hp.timing, BITIME_HP_THRESHOLD);
                EXPECT_EQ(x->hp.op, cases[i].a);
                EXPECT_EQ(x->hp.divisor, cases[i].b);
                EXPECT_EQ(x->hp.pred.kind, BPRED_HP_AT_MOST);
                EXPECT_EQ(x->hp.pred.arg, 2);
            }
            else if (x->kind == BITEM_CURE)
            {
                EXPECT_EQ(x->cure.timing, BITIME_STATUS);
                EXPECT_EQ(x->cure.status, cases[i].a);
                EXPECT_EQ(x->cure.volatiles, cases[i].b);
            }
            else
            {
                EXPECT_EQ(x->endure.timing, BITIME_LETHAL_HIT);
                EXPECT_EQ(x->endure.condition, cases[i].a);
            }
        }
        TRY_FREE_AND_SET_NULL(r.request);
        // V18: not simulated (every item off).
        ExportOne(&r, V18, SPECIES_WOBBUFFET, 0, cases[i].item, FALSE);
        EXPECT(r.built);
        EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
        EXPECT(PlayerItem(&r) != NULL && PlayerItem(&r)->cls == BICLASS_UNSUPPORTED);
        TRY_FREE_AND_SET_NULL(r.request);
    }
}

TEST("GC V19 export: every other berry and Berry Juice stay unsupported")
{
    struct FrontierGcExportResult r;
    u32 berries = 0;
    for (u16 item = 1; item < ITEMS_COUNT; item++)
    {
        const enum HoldEffect hold = GetItemHoldEffect(item);
        const bool32 consumable = hold == HOLD_EFFECT_RESTORE_HP || hold == HOLD_EFFECT_RESTORE_PCT_HP
                               || hold == HOLD_EFFECT_CURE_PAR || hold == HOLD_EFFECT_CURE_SLP
                               || hold == HOLD_EFFECT_CURE_PSN || hold == HOLD_EFFECT_CURE_BRN
                               || hold == HOLD_EFFECT_CURE_FRZ || hold == HOLD_EFFECT_CURE_CONFUSION
                               || hold == HOLD_EFFECT_CURE_STATUS;
        if (GetItemPocket(item) != POCKET_BERRIES && item != ITEM_BERRY_JUICE)
            continue;
        if (item == ITEM_ENIGMA_BERRY_E_READER)
            continue;
        ExportOne(&r, V19, SPECIES_WOBBUFFET, 0, item, FALSE);
        EXPECT(r.built);
        if (consumable && GetItemPocket(item) == POCKET_BERRIES)
        {
            EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
            berries++;
        }
        else
        {
            EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
            EXPECT_EQ(r.itemReason, item);
        }
        TRY_FREE_AND_SET_NULL(r.request);
    }
    EXPECT_EQ(berries, 9);
}

TEST("GC V19 export: Cud Chew with a berry, and the item abilities, turn every item off")
{
    struct FrontierGcExportResult r;
    // Cud Chew (Farigiraf, slot 0) holding Sitrus Berry: off; with Leftovers (never used up): simulated.
    ExportOne(&r, V19, SPECIES_FARIGIRAF, 0, ITEM_SITRUS_BERRY, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
    EXPECT_EQ(r.itemReason, ABILITY_CUD_CHEW);
    TRY_FREE_AND_SET_NULL(r.request);
    ExportOne(&r, V19, SPECIES_FARIGIRAF, 0, ITEM_SITRUS_BERRY, TRUE);
    EXPECT(!r.built);
    TRY_FREE_AND_SET_NULL(r.request);
    ExportOne(&r, V19, SPECIES_FARIGIRAF, 0, ITEM_LEFTOVERS, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    TRY_FREE_AND_SET_NULL(r.request);
    // Gluttony (Snorlax, slot 2), Unnerve, Ripen, Cheek Pouch, Harvest: off (as from M4-B0).
    static const u16 holders[][2] = {
        {SPECIES_SNORLAX, 2}, {SPECIES_AERODACTYL, 2}, {SPECIES_APPLETUN, 0}, {SPECIES_DEDENNE, 0}, {SPECIES_EXEGGUTOR, 2},
    };
    for (u32 i = 0; i < ARRAY_COUNT(holders); i++)
    {
        ExportOne(&r, V19, holders[i][0], holders[i][1], ITEM_SITRUS_BERRY, FALSE);
        EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
        TRY_FREE_AND_SET_NULL(r.request);
    }
    // Power Construct (a max HP change) with an HP berry: off (HpItemAbility).
    ExportOne(&r, V19, SPECIES_ZYGARDE_50_POWER_CONSTRUCT, 0, ITEM_SITRUS_BERRY, FALSE);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
    TRY_FREE_AND_SET_NULL(r.request);
}

TEST("GC V19 export: strict items refuse an unsupported berry; a Z-Crystal beside a berry is accepted")
{
    struct FrontierGcExportResult r;
    ExportOne(&r, V19, SPECIES_SNORLAX, 0, ITEM_FIGY_BERRY, TRUE);
    EXPECT(!r.built);
    TRY_FREE_AND_SET_NULL(r.request);
    ExportOne(&r, V19, SPECIES_SNORLAX, 0, ITEM_FIGY_BERRY, FALSE);
    EXPECT(r.built);
    EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ITEM);
    EXPECT_EQ(r.itemReason, ITEM_FIGY_BERRY);
    TRY_FREE_AND_SET_NULL(r.request);
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    MakeMon(&mons[0], SPECIES_SNORLAX, ITEM_SNORLIUM_Z, MOVE_TACKLE);
    SetAbilityNum(&mons[0], 0);
    MakeMon(&mons[1], SPECIES_WOBBUFFET, ITEM_SITRUS_BERRY, MOVE_TACKLE);
    const struct FrontierGcExportTest t = {.roster = V19, .format = BATTLE_FORMAT_SINGLE, .count = 1, .strictItems = TRUE,
                                           .mons = mons};
    FrontierGcBattle_TestExport(&t, &r);
    EXPECT(r.built);
    EXPECT_EQ(r.gimmickState, FGC_GIMMICK_ITEMS_ACCEPTED);
    EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
    TRY_FREE_AND_SET_NULL(r.request);
    Free(mons);
}

// Every gBattleFrontierMons set as the player's lead in a Single against a
// plain Snorlax (Immunity, Tackle), exported as V18 and as V19. One TALLY19
// line per chunk; the increments are by the set's own item.
#define TALLY_CHUNK 120
enum { GROUP_OTHER, GROUP_HP_BERRY, GROUP_STATUS_BERRY, GROUP_SASH, GROUP_COUNT };
static u32 ItemGroup(u16 item)
{
    switch (GetItemHoldEffect(item))
    {
    case HOLD_EFFECT_RESTORE_HP:
    case HOLD_EFFECT_RESTORE_PCT_HP:
        return GetItemPocket(item) == POCKET_BERRIES ? GROUP_HP_BERRY : GROUP_OTHER;
    case HOLD_EFFECT_CURE_PAR: case HOLD_EFFECT_CURE_SLP: case HOLD_EFFECT_CURE_PSN: case HOLD_EFFECT_CURE_BRN:
    case HOLD_EFFECT_CURE_FRZ: case HOLD_EFFECT_CURE_CONFUSION: case HOLD_EFFECT_CURE_STATUS:
        return GROUP_STATUS_BERRY;
    case HOLD_EFFECT_FOCUS_SASH:
        return GROUP_SASH;
    default:
        return GROUP_OTHER;
    }
}

static bool32 StrictOk(const BattleRequest *q)
{
    for (u32 side = 0; side < 2; side++)
    {
        const BattleEntry *e = side ? &q->opponent[0] : &q->player[0];
        if (e->mon.ability.cls == BACLASS_UNSUPPORTED)
            return FALSE;
        for (u32 m = 0; m < MAX_MON_MOVES; m++)
            if (e->record.moves[m] != MOVE_NONE && e->mon.moves[m].unsupported != 0)
                return FALSE;
    }
    return TRUE;
}

TEST("GC V19 export tally: Battle Frontier sets")
{
    u32 chunk, built = 0, sim18 = 0, sim19 = 0, full18 = 0, full19 = 0, strict19 = 0;
    u32 held[GROUP_COUNT] = {0}, gained[GROUP_COUNT] = {0}, gainedFull[GROUP_COUNT] = {0}, off19[GROUP_COUNT] = {0};
    u32 offAbility19 = 0;
    struct Pokemon *mons = AllocZeroed(2 * sizeof(struct Pokemon));
    for (chunk = 0; chunk < (NUM_FRONTIER_MONS + TALLY_CHUNK - 1) / TALLY_CHUNK; chunk++)
        PARAMETRIZE_LABEL("%d", chunk) {}
    chunk = gFunctionTestRunnerState->runParameter;
    for (u32 set = chunk * TALLY_CHUNK; set < (chunk + 1) * TALLY_CHUNK && set < NUM_FRONTIER_MONS; set++)
    {
        struct FrontierGcExportResult r18, r19;
        const u32 group = ItemGroup(gBattleFrontierMons[set].heldItem);
        CreateFacilityMon(&gBattleFrontierMons[set], 50, 31, 0x12345678, 0, &mons[0]);
        MakeMon(&mons[1], SPECIES_SNORLAX, ITEM_NONE, MOVE_TACKLE);
        SetAbilityNum(&mons[1], 0);
        held[group]++;
        const struct FrontierGcExportTest t18 = {.roster = V18, .format = BATTLE_FORMAT_SINGLE, .count = 1, .mons = mons};
        const struct FrontierGcExportTest t19 = {.roster = V19, .format = BATTLE_FORMAT_SINGLE, .count = 1, .mons = mons};
        FrontierGcBattle_TestExport(&t18, &r18);
        FrontierGcBattle_TestExport(&t19, &r19);
        if (r19.built && r19.request != NULL)
        {
            const bool32 s18 = r18.itemState == FGC_ITEMS_SIMULATED, s19 = r19.itemState == FGC_ITEMS_SIMULATED;
            const bool32 f18 = s18 && r18.request != NULL && StrictOk(r18.request);
            const bool32 f19 = s19 && StrictOk(r19.request);
            built++;
            sim18 += s18;
            sim19 += s19;
            full18 += f18;
            full19 += f19;
            if (s19 && !s18)
                gained[group]++;
            if (f19 && !f18)
                gainedFull[group]++;
            if (!s19 && r19.itemState == FGC_ITEMS_OFF_ITEM)
                off19[ItemGroup(r19.itemReason)]++;
            offAbility19 += r19.itemState == FGC_ITEMS_OFF_ABILITY;
        }
        TRY_FREE_AND_SET_NULL(r18.request);
        TRY_FREE_AND_SET_NULL(r19.request);
        // Strict (items, abilities, moves) as V19.
        const struct FrontierGcExportTest ts = {.roster = V19, .format = BATTLE_FORMAT_SINGLE, .count = 1,
                                                .strictItems = TRUE, .strictAll = TRUE, .mons = mons};
        FrontierGcBattle_TestExport(&ts, &r19);
        if (r19.built && r19.request != NULL && StrictOk(r19.request))
            strict19++;
        TRY_FREE_AND_SET_NULL(r19.request);
    }
    Free(mons);
    Test_MgbaPrintf("TALLY19 chunk=%d built=%d sim18=%d sim19=%d full18=%d full19=%d strict19=%d offAbility19=%d "
                    "heldHp=%d heldStatus=%d heldSash=%d gainHp=%d gainStatus=%d gainSash=%d gainOther=%d "
                    "fullHp=%d fullStatus=%d fullSash=%d fullOther=%d offOther=%d offHp=%d offStatus=%d offSash=%d",
                    chunk, built, sim18, sim19, full18, full19, strict19, offAbility19,
                    held[GROUP_HP_BERRY], held[GROUP_STATUS_BERRY], held[GROUP_SASH],
                    gained[GROUP_HP_BERRY], gained[GROUP_STATUS_BERRY], gained[GROUP_SASH], gained[GROUP_OTHER],
                    gainedFull[GROUP_HP_BERRY], gainedFull[GROUP_STATUS_BERRY], gainedFull[GROUP_SASH],
                    gainedFull[GROUP_OTHER], off19[GROUP_OTHER], off19[GROUP_HP_BERRY], off19[GROUP_STATUS_BERRY],
                    off19[GROUP_SASH]);
}
