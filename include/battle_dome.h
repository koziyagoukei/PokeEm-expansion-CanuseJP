#ifndef GUARD_BATTLE_DOME_H
#define GUARD_BATTLE_DOME_H

#include "constants/battle_dome.h"

int GetDomeTrainerSelectedMons(u16 tournamentTrainerId);
int TrainerIdToDomeTournamentId(u16 trainerId);

#if TESTING
u32 Test_GetDomeBattleStyle(const enum Move *moves);
#endif

#endif // GUARD_BATTLE_DOME_H
