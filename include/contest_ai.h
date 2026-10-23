#ifndef GUARD_CONTESTAI_H
#define GUARD_CONTESTAI_H

// AI states
enum
{
    CONTESTAI_SETTING_UP,
    CONTESTAI_PROCESSING,
    CONTESTAI_FINISHED,
    CONTESTAI_DO_NOT_PROCESS
};

void ContestAI_ResetAI(u8 contestantAI);
u8 ContestAI_GetActionToUse(void);
s16 ContestAI_GetTacticalScore(u8 contestant, enum Move move, u8 category, s8 excitement);

#endif // GUARD_CONTESTAI_H
