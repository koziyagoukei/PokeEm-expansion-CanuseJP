#ifndef GUARD_FRONTIER_GUEST_H
#define GUARD_FRONTIER_GUEST_H

#include "constants/frontier_guest.h"

struct FrontierGuestTrainer
{
    u8 name[PLAYER_NAME_LENGTH + 1];
    u16 objectGfx;
    enum TrainerPicID portrait;
    enum TrainerClassID trainerClass;
    enum Type type;
    bool8 female;
};

const struct FrontierGuestTrainer *GetFrontierGuestTrainer(u16 trainerId);
bool8 IsFrontierGuestBattle(u8 battleNum);
u16 GetRandomFrontierGuestTrainer(void);
void SetFrontierGuestMultiPair(u16 *trainerIds);
u8 GetFrontierGuestMonFilter(u16 trainerId, u8 slot);
void CopyFrontierGuestText(u8 whichText, u16 trainerId);

#endif
