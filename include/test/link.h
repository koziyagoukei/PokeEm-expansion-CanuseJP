#ifndef GUARD_TEST_LINK_H
#define GUARD_TEST_LINK_H

#include "contest.h"

void Test_InitLinkBattleBufferTasks(u8 *sendTaskId, u8 *receiveTaskId);
void Test_RunLinkBattleStartState(bool32 preview);
bool32 Test_ContestGimmickCanSelect(u8 contestant, enum ContestGimmick selected, enum Move move);
void Test_CalculateAppealMoveImpact(u8 contestant);

#endif
