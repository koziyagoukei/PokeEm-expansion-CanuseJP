#ifndef GUARD_CONTEST_NPC_H
#define GUARD_CONTEST_NPC_H

#include "contest.h"

struct ContestNpcPreset
{
    enum Species species;
    enum Move moves[MAX_MON_MOVES];
    enum Item heldItem;
    enum Type teraType;
    enum ContestGimmick gimmick;
    u8 categories;
    u8 activationTurn;
    u8 condition;
    bool8 gmaxFactor;
};

const struct ContestNpcPreset *Contest_GetNpcPreset(u32 opponent);
const struct ContestPokemon *Contest_GetNpcTemplate(u32 opponent);
u32 Contest_GetNpcPresetCount(void);

#if TESTING
void Test_ContestLoadNpc(u8 contestant, u16 opponent, enum ContestCategories category);
u8 Test_ContestChooseNpcMove(u8 contestant);
void Test_ContestActivateGimmick(u8 contestant);
#endif

#endif
