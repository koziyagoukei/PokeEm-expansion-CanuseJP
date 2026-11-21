#ifndef GUARD_FRONTIER_GC_BATTLE_H
#define GUARD_FRONTIER_GC_BATTLE_H

// Frontier content: GC Battle test. Internal interface between the mode
// (frontier_gc_battle.c) and its presentation exporter (frontier_gc_battle_gfx.c).

#include "gc_ext/battle_protocol.h"

struct FrontierGcBattleScene
{
    struct Pokemon *player;    // the player's battler (party member)
    struct Pokemon *opponent;  // the opponent (mode-owned, export slot 0x80)
    u16 trainerId;
    u8 environment;
};

void FrontierGcBattle_GetScene(struct FrontierGcBattleScene *scene);
bool FrontierGcBattle_ResolveAsset(const AssetKey *key, AssetInfo *info, const u8 **data);
void FrontierGcBattle_FreeGfx(void);
// V17: the next GC battle refuses to start, instead of normalising every held
// item, if an item is not simulated (RULE_STRICT_ITEMS).
void FrontierGcBattle_SetStrictItems(bool8 strict);

// Whether the battle's held items take part (V17), as the GBA shows it.
enum FrontierGcItemState
{
    FGC_ITEMS_NOT_SENT,
    FGC_ITEMS_SIMULATED,
    FGC_ITEMS_OFF_ITEM,     // an item is not simulated: every item is off
    FGC_ITEMS_OFF_ABILITY,  // an item ability: every item is off
};

// M4-B1a+ (V18): what became of the battle's audited Z-Crystals and Mega
// Stones (AcceptGimmickItems). Accepted: held, with no effect (BICLASS_NONE).
enum FrontierGcGimmickItemState
{
    FGC_GIMMICK_ITEMS_NONE,         // no Pokemon holds an audited candidate
    FGC_GIMMICK_ITEMS_ACCEPTED,
    FGC_GIMMICK_ITEMS_OFF_VERSION,  // not a V18 request
    FGC_GIMMICK_ITEMS_OFF_RULES,    // the request does not forbid gimmicks
    FGC_GIMMICK_ITEMS_OFF_FORM,     // a Pokemon of the battle changes form by itself
    FGC_GIMMICK_ITEMS_OFF_MOVE,     // a Pokemon knows a move that reads or moves held items
};

#if TESTING
// Test hooks (test/battle/gc_gimmick_item_reference.c): the exporter's own
// judgement, through the code a real battle runs.
struct FrontierGcExportTest
{
    u8 roster;            // BATTLE_APP_VERSION_* of the battle
    u8 format;            // BATTLE_FORMAT_*
    u8 count;             // Pokemon per side, 1..3
    bool8 strictItems;    // FrontierGcBattle_SetStrictItems
    bool8 strictAll;      // also RULE_STRICT_ABILITIES and RULE_STRICT_MOVES
    bool8 allowGimmicks;  // a request without RULE_NO_GIMMICKS
    struct Pokemon *mons; // `count` player Pokemon, then `count` opponents
};
struct FrontierGcExportResult
{
    bool8 built;             // a request was built (else `refusal` says why)
    const u8 *refusal;
    u8 itemState;            // FGC_ITEMS_*
    u16 itemReason;          // the item or ability that turned every item off
    u8 gimmickState;         // FGC_GIMMICK_ITEMS_*
    BattleRequest *request;  // the request as the GC decodes it (caller frees; NULL if not built)
};
u8 FrontierGcBattle_TestMoveSupport(u8 roster, u8 format, u16 move);  // UNSUP_* reasons, 0 = supported
u8 FrontierGcBattle_TestGimmickItem(u16 item, u16 species);          // 0, or the audited candidate's kind
void FrontierGcBattle_TestExport(const struct FrontierGcExportTest *test, struct FrontierGcExportResult *result);
void FrontierGcBattle_TestExportFixture(u8 lab, u8 roster, u8 format, bool8 endurance, u16 trainer,
                                        struct FrontierGcExportResult *result);
u8 FrontierGcBattle_TestLabCount(void);
void FrontierGcBattle_TestSetLegacyExport(bool8 legacy);  // TRUE: as M4-B1a (no move flags, no gimmick items)
#endif

#endif // GUARD_FRONTIER_GC_BATTLE_H
