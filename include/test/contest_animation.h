#ifndef GUARD_TEST_CONTEST_ANIMATION_H
#define GUARD_TEST_CONTEST_ANIMATION_H

#include "contest.h"

bool32 Test_ContestBeginCutaway(u8 contestant, enum Move move);
void Test_ContestSetupCutaway(u8 contestant, enum Move move, bool32 targetSelf);
void Test_ContestEndCutaway(u8 contestant);
void Test_ContestFreeResources(void);

#endif
