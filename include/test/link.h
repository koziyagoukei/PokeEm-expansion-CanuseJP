#ifndef GUARD_TEST_LINK_H
#define GUARD_TEST_LINK_H

#include "contest.h"

bool32 Test_ContestGimmickCanSelect(u8 contestant, enum ContestGimmick selected, enum Move move);
void Test_CalculateAppealMoveImpact(u8 contestant);
enum Move Test_ContestGetAnimMove(u8 contestant);
void Test_ContestAdvanceRound(void);
void Test_ContestPrepareMoveAnimData(u8 contestant, enum Move effectMove);
void Test_ContestRefreshGimmickIcons(void);
void Test_ContestDestroyGimmickIcons(void);

#endif
