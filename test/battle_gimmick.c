#include "global.h"
#include "battle.h"
#include "battle_dynamax.h"
#include "battle_gimmick.h"
#include "battle_terastal.h"
#include "battle_z_move.h"
#include "test/test.h"

TEST("Tera form restrictions: Eternamax and Ultra Necrozma cannot Terastallize in any battler slot")
{
    enum Species species;
    enum BattlerId battler;
    PARAMETRIZE { species = SPECIES_ETERNATUS_ETERNAMAX; battler = B_BATTLER_0; }
    PARAMETRIZE { species = SPECIES_ETERNATUS_ETERNAMAX; battler = B_BATTLER_1; }
    PARAMETRIZE { species = SPECIES_ETERNATUS_ETERNAMAX; battler = B_BATTLER_2; }
    PARAMETRIZE { species = SPECIES_ETERNATUS_ETERNAMAX; battler = B_BATTLER_3; }
    PARAMETRIZE { species = SPECIES_NECROZMA_ULTRA; battler = B_BATTLER_0; }
    PARAMETRIZE { species = SPECIES_NECROZMA_ULTRA; battler = B_BATTLER_1; }
    PARAMETRIZE { species = SPECIES_NECROZMA_ULTRA; battler = B_BATTLER_2; }
    PARAMETRIZE { species = SPECIES_NECROZMA_ULTRA; battler = B_BATTLER_3; }

    enum Species originalSpecies = gBattleMons[battler].species;
    gBattleMons[battler].species = species;

    EXPECT(!CanTerastallize(battler));
    EXPECT(!CanActivateGimmick(battler, GIMMICK_TERA));

    gBattleMons[battler].species = originalSpecies;
}

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
