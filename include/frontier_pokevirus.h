#ifndef GUARD_FRONTIER_POKEVIRUS_H
#define GUARD_FRONTIER_POKEVIRUS_H

// A display-only record, not a species or a national/regional Pokedex number.
#define POKEVIRUS_DEX_RECORD 0xFFFE

extern const u8 gPokevirusDexCategory[];
extern const u8 gPokevirusDexDescription[];

bool32 FrontierPokevirus_IsRegistered(void);
bool32 FrontierPokevirus_CanName(void);
bool32 FrontierPokevirus_RegisterName(const u8 *name);
bool32 FrontierPokevirus_IsDexRecord(u32 number);
u8 *FrontierPokevirus_CopyName(u8 *dest);
void FrontierPokevirus_CheckEpilogue(void);
void FrontierPokevirus_Name(void);
void FrontierPokevirus_Register(void);
void FrontierPokevirus_ShowRecord(void);

#endif
