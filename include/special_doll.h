#ifndef GUARD_SPECIAL_DOLL_H
#define GUARD_SPECIAL_DOLL_H

#include "constants/special_dolls.h"

struct Decoration;
struct ObjectEventGraphicsInfo;

extern const u8 gText_SpecialDolls[];
const struct Decoration *SpecialDoll_GetDecoration(u16 doll);
const struct ObjectEventGraphicsInfo *SpecialDoll_GetGraphicsInfo(u16 graphicsId);
bool8 SpecialDoll_IsShiny(u16 doll);
void SpecialDoll_EnsureSaveData(void);
void SpecialDoll_UpdateChecksum(void);
bool8 SpecialDoll_Add(u16 doll);
u8 SpecialDoll_GetOwnedCount(void);
void SpecialDoll_CondenseInventory(void);
u8 SpecialDoll_GetPlacedCount(void);
u8 LocalRoom_GetSpriteDecorationCount(void);
void SpecialDoll_InitObjects(void);
void SpecialDoll_ClearPlacements(void);
void SpecialDoll_ShowDecorationMenu(void);

#endif
