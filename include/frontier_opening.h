#ifndef GUARD_FRONTIER_OPENING_H
#define GUARD_FRONTIER_OPENING_H

enum FrontierOpeningPhase
{
    FRONTIER_OPENING_FOREST,
    FRONTIER_OPENING_HANDS,
    FRONTIER_OPENING_NETWORK,
    FRONTIER_OPENING_GUARDIANS,
    FRONTIER_OPENING_ATTACK,
    FRONTIER_OPENING_WHITE,
    FRONTIER_OPENING_DONE,
};

// Sprite tile and palette tags: the Pokevirus, then the three guardians' back
// sprites, the four infected trainers and the light, spore and chain effects.
#define FRONTIER_OPENING_ACTOR_TAG    0x7000
#define FRONTIER_OPENING_GUARDIAN_TAG (FRONTIER_OPENING_ACTOR_TAG + 1)
#define FRONTIER_OPENING_TRAINER_TAG  (FRONTIER_OPENING_ACTOR_TAG + 4)
#define FRONTIER_OPENING_TAG_COUNT    11

void Task_FrontierOpeningStart(u8 taskId);
void FrontierOpening_Stop(void);
bool32 FrontierOpening_IsActive(void);
enum FrontierOpeningPhase FrontierOpening_GetPhase(void);
u16 FrontierOpening_GetFrame(void);

#endif
