#ifndef GUARD_PARTY_BATH_VISITORS_H
#define GUARD_PARTY_BATH_VISITORS_H

#include "constants/party_bath_visitors.h"

void PartyBath_PickVisitors(void);
void PartyBath_BufferVisitorText(void);

#if TESTING
bool32 Test_PartyBathVisitEligible(u32 visit);
void Test_PartyBathApplyVisit(u32 visit);
#endif

#endif
