#ifndef GUARD_PARTY_BATH_EXPANSION_H
#define GUARD_PARTY_BATH_EXPANSION_H

#include "constants/party_bath_expansion.h"

bool32 PartyBath_IsMap(void);
bool32 PartyBath_UsesWaterPalette(void);
u32 PartyBath_GetGain(void);
u32 PartyBath_GetPendingExpansionCall(void);
bool32 PartyBath_TryStartExpansionCall(void);
void PartyBath_GetPendingCall(void);
void PartyBath_CompleteCall(void);
void PartyBath_CheckExpansion(void);

#endif
