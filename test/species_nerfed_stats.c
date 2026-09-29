#include "global.h"
#include "test/test.h"

TEST("Previously nerfed species keep their highest released base stats")
{
    EXPECT_EQ(gSpeciesInfo[SPECIES_AEGISLASH_SHIELD].baseDefense, 150);
    EXPECT_EQ(gSpeciesInfo[SPECIES_AEGISLASH_SHIELD].baseSpDefense, 150);
    EXPECT_EQ(gSpeciesInfo[SPECIES_AEGISLASH_BLADE].baseAttack, 150);
    EXPECT_EQ(gSpeciesInfo[SPECIES_AEGISLASH_BLADE].baseSpAttack, 150);

    EXPECT_EQ(gSpeciesInfo[SPECIES_CRESSELIA].baseDefense, 120);
    EXPECT_EQ(gSpeciesInfo[SPECIES_CRESSELIA].baseSpDefense, 130);

    EXPECT_EQ(gSpeciesInfo[SPECIES_ZACIAN_HERO].baseAttack, 130);
    EXPECT_EQ(gSpeciesInfo[SPECIES_ZACIAN_CROWNED].baseAttack, 170);

    EXPECT_EQ(gSpeciesInfo[SPECIES_ZAMAZENTA_HERO].baseAttack, 130);
    EXPECT_EQ(gSpeciesInfo[SPECIES_ZAMAZENTA_CROWNED].baseAttack, 130);
    EXPECT_EQ(gSpeciesInfo[SPECIES_ZAMAZENTA_CROWNED].baseDefense, 145);
    EXPECT_EQ(gSpeciesInfo[SPECIES_ZAMAZENTA_CROWNED].baseSpDefense, 145);
}
