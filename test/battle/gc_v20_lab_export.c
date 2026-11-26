#include "global.h"
#include "test/battle.h"
#include "frontier_gc_battle.h"
#include "malloc.h"

// M4-B1c-0c (gba-gc-extension docs/items.md section 15): the V20 Lab
// fixtures L73-L83 as the GBA builds them for the GC (Init, the same code a
// Dolphin run takes). Each fixture's request length and CRC-32 are printed
// beside the stats the Dolphin comparison and the Expansion reference use;
// the GC's PH9 REQ dump of a Dolphin run must give the same bytes.

#define V20 BATTLE_APP_VERSION_MOVE_ITEMS
#define FIRST_V20_LAB 73
#define LAST_V20_LAB 83
#define REFUSED_V20_LAB 82

static u32 Crc32(const u8 *data, u32 length)
{
    u32 crc = 0xFFFFFFFF;
    for (u32 i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (u32 b = 0; b < 8; b++)
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
    }
    return ~crc;
}

static bool32 IsLifeOrbBlock(const BattleItem *it)
{
    const BattleItemRecord *m = &it->records[0], *h = &it->records[1];
    return it->cls == BICLASS_RECORDS && it->source == ITEM_LIFE_ORB && it->count == 2
        && m->kind == BITEM_MODIFIER && m->flags == 0 && m->mod.stage == (BMOD_FINAL | BMOD_FLOORED)
        && m->mod.percent == 130 && h->kind == BITEM_HP && h->flags == 0 && h->hp.timing == BITIME_AFTER_MOVE
        && h->hp.op == BIHP_DAMAGE && h->hp.divisor == 10 && h->hp.pred.kind == BPRED_NONE && h->hp.reserved == 0;
}

TEST("GC V20 export: Lab fixtures L73-L83 (Life Orb on the GBA -> GC path)")
{
    enum { CAPACITY = BATTLE_REQUEST_MAX_BYTES(PARTY_SIZE, NUMBER_OF_MON_TYPES) };
    struct FrontierGcExportResult r;
    u32 lab;
    EXPECT_EQ(FrontierGcBattle_TestLabCount(), LAST_V20_LAB);
    for (lab = FIRST_V20_LAB; lab <= LAST_V20_LAB; lab++)
        PARAMETRIZE_LABEL("L%d", lab) {}
    lab = FIRST_V20_LAB + gFunctionTestRunnerState->runParameter;
    FrontierGcBattle_TestExportFixture(lab, 0, 0, FALSE, TRAINER_CALVIN_1, &r);
    if (lab == REFUSED_V20_LAB)
    {
        // Klutz is an item ability: strict item rules refuse on the GBA.
        EXPECT(!r.built);
        EXPECT(r.request == NULL);
        return;
    }
    EXPECT(r.built);
    EXPECT(r.request != NULL);
    if (r.request == NULL)
        return;
    {
        u8 *bytes = AllocZeroed(CAPACITY);
        const u32 length = battle_request_encode(r.request, bytes, CAPACITY);
        u32 holders = 0;
        EXPECT_EQ(r.request->version, V20);
        EXPECT(r.request->rules & RULE_FIXED_SEED);
        EXPECT_EQ(r.request->battle_id, 0x4C414200u | lab);
        Test_MgbaPrintf("V20LAB L%d bytes=%d crc=%d items=%d reason=%d rules=%d", lab, length, Crc32(bytes, length),
                        r.itemState, r.itemReason, r.request->rules);
        // The bytes, as the GC's PH9 REQ dump prints them (the save's trainer
        // id and the map's environment differ from a Dolphin run).
        for (u32 at = 0; lab == FIRST_V20_LAB && at < length; at += 16)
        {
            u32 w[4] = {0};
            for (u32 k = 0; k < 16 && at + k < length; k++)
                w[k / 4] |= (u32)bytes[at + k] << (8 * (3 - k % 4));
            Test_MgbaPrintf("V20LABHEX %d %d %d %d %d", at, w[0], w[1], w[2], w[3]);
        }
        for (u32 side = 0; side < 2; side++)
        {
            const BattleEntry *e = side ? r.request->opponent : r.request->player;
            const u32 count = side ? r.request->opponent_count : r.request->player_count;
            for (u32 i = 0; i < count; i++)
            {
                const PokemonRecord *p = &e[i].record;
                Test_MgbaPrintf("V20LAB L%d side=%d entry=%d species=%d level=%d hp=%d/%d atk=%d def=%d spa=%d spd=%d spe=%d "
                                "item=%d records=%d cls=%d", lab, side, i, p->species, p->level, p->hp, p->max_hp,
                                p->attack, p->defense, p->sp_attack, p->sp_defense, p->speed, p->held_item,
                                e[i].mon.item.count, e[i].mon.item.cls);
                EXPECT_EQ(e[i].mon.item.source, p->held_item);  // the id is kept with its records
                for (u32 m = 0; m < MAX_MON_MOVES; m++)
                    if (p->moves[m] != MOVE_NONE)
                        EXPECT_EQ(e[i].mon.moves[m].unsupported, 0);
                if (p->held_item == ITEM_LIFE_ORB)
                {
                    holders++;
                    if (lab != 81)
                        EXPECT(IsLifeOrbBlock(&e[i].mon.item));
                }
            }
        }
        EXPECT_GT(holders, 0);
        if (lab == 81)
        {
            // Sheer Force and Magic Guard are not simulated: every item is off.
            EXPECT_EQ(r.itemState, FGC_ITEMS_OFF_ABILITY);
            EXPECT(r.request->rules & RULE_IGNORE_ITEMS);
        }
        else
        {
            EXPECT_EQ(r.itemState, FGC_ITEMS_SIMULATED);
            EXPECT(r.request->rules & RULE_STRICT_ITEMS);
            EXPECT(!(r.request->rules & RULE_IGNORE_ITEMS));
            if (lab != 74 && lab != 83)
                EXPECT(r.request->rules & RULE_STRICT_ABILITIES);
        }
        Free(bytes);
    }
    TRY_FREE_AND_SET_NULL(r.request);
}
