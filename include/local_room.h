#ifndef GUARD_LOCAL_ROOM_H
#define GUARD_LOCAL_ROOM_H

#include "constants/local_room.h"

struct Decoration;

bool8 LocalRoom_IsCurrentMap(void);
void LocalRoom_EnsureSaveData(void);
void LocalRoom_UpdateChecksum(void);
bool8 LocalRoom_IsPlacementAllowed(u8 decor, s16 x, s16 y);
bool8 LocalRoom_IsPlacementAllowedForDecoration(const struct Decoration *decoration, s16 x, s16 y);
bool8 LocalRoom_IsPlainFloor(s16 mapX, s16 mapY);
bool8 LocalRoom_HasAccessPathAfterPlacement(u8 decor, s16 x, s16 y);
bool8 LocalRoom_HasAccessPathForDecoration(const struct Decoration *decoration, s16 x, s16 y);
void LocalRoom_PrepareMap(void);
void LocalRoom_LoadDecorations(void);
void LocalRoom_InitObjects(void);
void LocalRoom_Resume(void);
void LocalRoom_ReturnToField(void);
void LocalRoom_ApplyStyle(void);
void LocalRoom_ShowDecorationMenu(void);
void LocalRoom_BeginDecorating(void);
void LocalRoom_EndDecorating(void);
void LocalRoom_SetFloorStyle(void);
void LocalRoom_SetWallStyle(void);
void LocalRoom_SetRoamerCount(void);
void LocalRoom_ClearDecorations(void);
void LocalRoom_ClearDecorationData(void);
void LocalRoom_BufferMonMessage(void);
void LocalRoom_PerStepCallback(u8 taskId);

extern const u8 LocalRoom_EventScript_PC[];
extern const u8 LocalRoom_EventScript_PCMainMenu[];
extern const u8 LocalRoom_EventScript_InitDecorations[];
extern const u8 LocalRoom_EventScript_PutAwayDecoration[];
extern const u8 LocalRoom_EventScript_Ornament[];
extern const u8 LocalRoom_EventScript_SandOrnament[];

#endif
