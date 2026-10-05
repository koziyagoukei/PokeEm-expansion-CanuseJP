#include "global.h"
#include "battle.h"
#include "battle_dynamax.h"
#include "battle_gimmick.h"
#include "battle_terastal.h"
#include "battle_z_move.h"
#include "test/test.h"

TEST("Mega and Primal forms cannot use Dynamax, Terastallization, or Z-Moves")
{
    enum Species species, originalSpecies;
    bool32 isMegaOrPrimal;
    enum BattlerId battler = B_BATTLER_0;

    PARAMETRIZE { species = SPECIES_VENUSAUR; isMegaOrPrimal = FALSE; }
    PARAMETRIZE { species = SPECIES_VENUSAUR_MEGA; isMegaOrPrimal = TRUE; }
    PARAMETRIZE { species = SPECIES_KYOGRE; isMegaOrPrimal = FALSE; }
    PARAMETRIZE { species = SPECIES_KYOGRE_PRIMAL; isMegaOrPrimal = TRUE; }
    PARAMETRIZE { species = SPECIES_GROUDON; isMegaOrPrimal = FALSE; }
    PARAMETRIZE { species = SPECIES_GROUDON_PRIMAL; isMegaOrPrimal = TRUE; }

    originalSpecies = gBattleMons[battler].species;
    gBattleMons[battler].species = species;

    EXPECT_EQ(IsBattlerInMegaOrPrimalForm(battler), isMegaOrPrimal);
    if (isMegaOrPrimal)
    {
        EXPECT(!CanDynamax(battler));
        EXPECT(!CanTerastallize(battler));
        EXPECT(!CanUseZMove(battler));
    }

    gBattleMons[battler].species = originalSpecies;
}
