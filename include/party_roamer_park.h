#ifndef GUARD_PARTY_ROAMER_PARK_H
#define GUARD_PARTY_ROAMER_PARK_H

#include "constants/party_roamer_park.h"

bool8 FieldCallback_PrepareEnterPartyRoamerPark(void);
void Special_SpawnPartyRoamerMons(void);
void Special_RefreshPartyRoamerMons(void);
void Special_BufferPartyRoamerMessage(void);
void Special_StartPartyBathTimeFriendship(void);
void Special_PreparePartyRoamerPark(void);
void Special_BufferPartyBathAdditive(void);
bool8 Special_SetPartyBathAdditive(void);
u16 PartyBath_GetAdditive(void);
void PartyBath_ApplyWaterPalette(bool8 skipFaded);

#endif // GUARD_PARTY_ROAMER_PARK_H
