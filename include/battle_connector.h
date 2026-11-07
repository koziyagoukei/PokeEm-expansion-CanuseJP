#ifndef GUARD_BATTLE_CONNECTOR_H
#define GUARD_BATTLE_CONNECTOR_H

#include "config/battle_connector.h"
#include "constants/battle_connector.h"

struct Pokemon;

extern const u8 gText_BattleConnectorCantChangeItems[];
extern const u8 gText_BattleConnectorCantChangeMoves[];

// State
bool32 BattleConnector_IsChallengeActive(void);
bool32 BattleConnector_IsBattleActive(void);
bool32 BattleConnector_ShadowMovesAreNeutral(void);
void BattleConnector_CreatePanicMon(struct Pokemon *mon, u16 set, u32 level);
u16 BattleConnector_GetBattleBGM(void);
void BattleConnector_CreateShadowMon(struct Pokemon *mon, u16 species, u32 level);
void BattleConnector_CreateShadowMonFromSet(struct Pokemon *mon, u16 set, u32 level);
bool32 BattleConnector_IsRoomSpecies(u16 species);
u32 BattleConnector_GetSpeciesTier(u16 species);

// Specials (data/specials.inc). VAR_0x8004 carries a party slot or a path.
u16 BattleConnector_HasRoomToTakeHome(void);
u16 BattleConnector_StartChallenge(void);
void BattleConnector_PrepareBranch(void);
void BattleConnector_BufferPathHint(void);
void BattleConnector_EnterPath(void);
void BattleConnector_SetUpRoomObjects(void);
u16 BattleConnector_GetRoomType(void);
void BattleConnector_BufferRoomText(void);
void BattleConnector_StartRoomBattle(void);
void BattleConnector_BufferTrainerAfterText(void);
u16 BattleConnector_BufferDefeatedShadowMon(void);
u16 BattleConnector_ProtectDefeatedMon(void);
u16 BattleConnector_SwapIntoParty(void);
void BattleConnector_HealParty(void);
u16 BattleConnector_CalmParty(void);
void BattleConnector_PickRoomItem(void);
u16 BattleConnector_LeaveRoom(void);
void BattleConnector_BufferStatus(void);
void BattleConnector_FailChallenge(void);
void BattleConnector_RetireChallenge(void);
u16 BattleConnector_GetReturnState(void);
u16 BattleConnector_GetCandidateCount(void);
void BattleConnector_ShowCandidates(void);
void BattleConnector_BufferCandidateDetails(void);
u16 BattleConnector_TakeHomeCandidate(void);
void BattleConnector_TakeNothingHome(void);
u16 BattleConnector_CanStartPanicRush(void);
void BattleConnector_PanicResetPicks(void);
u16 BattleConnector_PanicPick(void);
u16 BattleConnector_StartPanicRush(void);
u16 BattleConnector_IsPanicRush(void);
void BattleConnector_StartPanicBattle(void);
void BattleConnector_BufferPanicChain(void);
void BattleConnector_EndPanicRush(void);

#if TESTING
u32 BattleConnector_Test_GetMode(void);
u16 BattleConnector_Test_GetPanicChain(void);
void BattleConnector_Test_SetPanicChain(u16 chain);
void BattleConnector_Test_SetMode(u32 mode);
u16 BattleConnector_Test_PickPanicSet(void);
void BattleConnector_Test_ApplyBattleOutcome(bool32 won);
void BattleConnector_Test_SetBattleActive(bool32 active, u32 roomType);
u32 BattleConnector_Test_GetPathRoomType(u32 path);
u32 BattleConnector_Test_GetCandidateCount(void);
struct Pokemon *BattleConnector_Test_GetCandidate(u32 index);
u16 BattleConnector_Test_GetRoomSpecies(void);
u16 BattleConnector_Test_GetRoomSet(void);
void BattleConnector_Test_SetUpRoom(u32 roomType);
void BattleConnector_Test_NewBattle(void);
#endif

#endif // GUARD_BATTLE_CONNECTOR_H
