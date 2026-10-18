#include "global.h"
#include "battle.h"
#include "battle_pyramid.h"
#include "battle_setup.h"
#include "battle_z_move.h"
#include "item.h"
#include "pokemon.h"
#include "random.h"
#include "constants/battle_frontier.h"
#include "constants/items.h"
#include "constants/trainers.h"

static bool32 CanMonUseCrystal(struct Pokemon *mon, enum Item item)
{
    enum Species species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG);
    if (species == SPECIES_NONE || species == SPECIES_EGG || item == ITEM_ULTRANECROZIUM_Z)
        return FALSE;

    for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
    {
        enum Move move = GetMonData(mon, MON_DATA_MOVE1 + slot);
        if (move == MOVE_NONE)
            continue;
        if (item <= ITEM_FAIRIUM_Z && GetMoveType(move) == GetItemSecondaryId(item))
            return TRUE;
        if (GetSignatureZMove(move, species, item) != MOVE_NONE)
            return TRUE;
    }
    return FALSE;
}

enum Item GetPyramidZCrystalForMon(struct Pokemon *mon, u32 seed)
{
    enum Item items[ITEM_ULTRANECROZIUM_Z - ITEM_NORMALIUM_Z];
    u32 count = 0;
    for (enum Item item = ITEM_NORMALIUM_Z; item < ITEM_ULTRANECROZIUM_Z; item++)
        if (CanMonUseCrystal(mon, item))
            items[count++] = item;
    return count ? items[seed % count] : ITEM_NONE;
}

enum Item GetPyramidPartyZCrystal(u32 seed)
{
    enum Item items[ITEM_ULTRANECROZIUM_Z - ITEM_NORMALIUM_Z];
    u32 count = 0;

    // Use the saved entry party, not its current order, PP or held items. The
    // same floor seed must select the same loot after a failed pickup/resume.
    for (enum Item item = ITEM_NORMALIUM_Z; item < ITEM_ULTRANECROZIUM_Z; item++)
    {
        for (u32 slot = 0; slot < FRONTIER_PARTY_SIZE; slot++)
        {
            u32 index = gSaveBlock2Ptr->frontier.selectedPartyMons[slot];
            if (index > 0 && index <= PARTY_SIZE && CanMonUseCrystal(GetSavedPlayerPartyMon(index - 1), item))
            {
                items[count++] = item;
                break;
            }
        }
    }
    return count ? items[seed % count] : ITEM_NONE;
}

void BalancePyramidOpponentParties(void)
{
    static const enum BattleTrainer trainers[] = {B_TRAINER_OPPONENT_A, B_TRAINER_OPPONENT_B};
    static const enum Item berries[] = {ITEM_ORAN_BERRY, ITEM_CHERI_BERRY, ITEM_PECHA_BERRY, ITEM_RAWST_BERRY, ITEM_CHESTO_BERRY};
    bool32 zAssigned = FALSE, teraAssigned = FALSE;
    bool32 brain = TRAINER_BATTLE_PARAM.opponentA == TRAINER_FRONTIER_BRAIN;
    u32 chance = gSaveBlock2Ptr->frontier.curChallengeBattleNum < 3
        ? PYRAMID_EARLY_GIMMICK_CHANCE : PYRAMID_LATE_GIMMICK_CHANCE;

    if (!(gBattleTypeFlags & BATTLE_TYPE_PYRAMID))
        return;

    for (u32 trainer = 0; trainer < ARRAY_COUNT(trainers); trainer++)
    {
        for (u32 slot = 0; slot < PARTY_SIZE; slot++)
        {
            struct Pokemon *mon = &gParties[trainers[trainer]][slot];
            enum Species species = GetMonData(mon, MON_DATA_SPECIES_OR_EGG);
            u32 item = ITEM_NONE, tera = TYPE_MYSTERY, dynamax = BLOCK_AI_DYNAMAX;
            if (species == SPECIES_NONE || species == SPECIES_EGG)
                continue;

            if ((brain && slot < 2) || Random() % 100 < chance)
            {
                if (!zAssigned && ((brain && slot == 0) || Random() % 2 == 0))
                {
                    item = GetPyramidZCrystalForMon(mon, Random());
                    zAssigned = item != ITEM_NONE;
                }
                if (item == ITEM_NONE && !teraAssigned)
                {
                    tera = gSpeciesInfo[species].types[0];
                    teraAssigned = TRUE;
                }
            }
            if (item == ITEM_NONE && Random() % 100 < PYRAMID_BERRY_CHANCE)
                item = berries[Random() % ARRAY_COUNT(berries)];

            SetMonData(mon, MON_DATA_HELD_ITEM, &item);
            SetMonData(mon, MON_DATA_TERA_TYPE, &tera);
            SetMonData(mon, MON_DATA_DYNAMAX_LEVEL, &dynamax);
        }
    }
}
