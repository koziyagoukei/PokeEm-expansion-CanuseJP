#ifndef GUARD_BATTLE_AI_FRONTIER_H
#define GUARD_BATTLE_AI_FRONTIER_H

// Extra judgement for the hub's さいきょう setting (AI_FLAG_FRONTIER_EXPERT).
s32 AI_FrontierExpert(enum BattlerId battlerAtk, enum BattlerId battlerDef, enum Move move, s32 score);
// Called after a move was chosen; may hold Dynamax or Terastallization for a later turn.
void FrontierExpert_ReconsiderGimmick(enum BattlerId battlerAtk, enum BattlerId battlerDef, enum Move move);

// A Frontier opponent forfeits when it has certainly lost (every AI setting).
bool32 IsAiCheckmated(enum BattlerId battler);
bool32 ShouldAiForfeitFrontierBattle(enum BattlerId battler);

#endif // GUARD_BATTLE_AI_FRONTIER_H
