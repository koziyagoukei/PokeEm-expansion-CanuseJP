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

#endif // GUARD_FRONTIER_GC_BATTLE_H
