#ifndef GUARD_FRONTIER_PRACTICE_H
#define GUARD_FRONTIER_PRACTICE_H

struct BoxPokemon;

void FrontierPractice_Reset(void);
u32 FrontierPractice_AddBoxMon(u32 box, u32 slot);
bool32 FrontierPractice_RemoveMon(u32 index);
bool32 FrontierPractice_CanSelectBoxMon(struct BoxPokemon *mon);
bool32 FrontierPractice_SetOptions(u32 ai, bool32 doubles, bool32 level50);
u32 FrontierPractice_PrepareBattle(void);
void FrontierPractice_RestoreBattle(void);
bool32 FrontierPractice_IsBattle(void);
u32 FrontierPractice_GetAiLevel(void);
u32 FrontierPractice_GetTeamCount(void);

void Special_FrontierPracticeInit(void);
void Special_FrontierPracticeAddSelection(void);
void Special_FrontierPracticeRemove(void);
void Special_FrontierPracticeClear(void);
void Special_FrontierPracticeBufferSettings(void);
void Special_FrontierPracticeBufferTeam(void);
void Special_FrontierPracticeSetOption(void);
void Special_FrontierPracticeStart(void);

#endif
