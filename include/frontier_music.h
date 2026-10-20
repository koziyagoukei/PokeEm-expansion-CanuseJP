#ifndef GUARD_FRONTIER_MUSIC_H
#define GUARD_FRONTIER_MUSIC_H

#include "constants/frontier_music.h"

void FrontierMusic_EnsureSaveData(void);
u16 FrontierMusic_GetSong(u32 index);
bool8 FrontierMusic_IsUnlocked(u16 song);
u16 FrontierMusic_Unlock(u16 song);
u16 FrontierMusic_SetMode(u8 mode, u16 song);
u16 FrontierMusic_ChooseBattleBGM(u16 defaultSong);
void FrontierMusic_OpenCatalog(void);
void FrontierMusic_ApplySelection(void);
void FrontierMusic_UnlockSelection(void);
void FrontierMusic_BufferSelection(void);

#endif
