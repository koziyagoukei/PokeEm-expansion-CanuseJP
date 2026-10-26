#include "global.h"
#include "battle.h"
#include "battle_frontier.h"
#include "pokemon.h"
#include "string_util.h"
#include "test/test.h"
#include "constants/battle_frontier_mons.h"

TEST("Frontier nicknames: all three sets share the species nickname after facility creation")
{
    u32 firstMonId = FRONTIER_MON_AMPHAROS_1;
    u32 species = SPECIES_AMPHAROS;
    const u8 *expected = J_COMPOUND_STRING("モコまる");
    PARAMETRIZE { firstMonId = FRONTIER_MON_AMPHAROS_1; species = SPECIES_AMPHAROS; expected = J_COMPOUND_STRING("モコまる"); }
    PARAMETRIZE { firstMonId = FRONTIER_MON_FLYGON_1; species = SPECIES_FLYGON; expected = J_COMPOUND_STRING("トビスケ"); }
    PARAMETRIZE { firstMonId = FRONTIER_MON_BASTIODON_1; species = SPECIES_BASTIODON; expected = J_COMPOUND_STRING("ゲンさん"); }
    PARAMETRIZE { firstMonId = FRONTIER_MON_REUNICLUS_1; species = SPECIES_REUNICLUS; expected = J_COMPOUND_STRING("ぷるもち"); }
    PARAMETRIZE { firstMonId = FRONTIER_MON_MIMIKYU_DISGUISED_1; species = SPECIES_MIMIKYU_DISGUISED; expected = J_COMPOUND_STRING("つぎはぎ"); }
    PARAMETRIZE { firstMonId = FRONTIER_MON_CLODSIRE_1; species = SPECIES_CLODSIRE; expected = J_COMPOUND_STRING("おはぎ"); }

    for (u32 slot = 0; slot < 3; slot++)
    {
        const struct TrainerMon *fmon = &gBattleFrontierMons[firstMonId + slot];
        struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][0];
        u8 nickname[POKEMON_NAME_BUFFER_SIZE];

        EXPECT_EQ((u32)fmon->species, species);
        EXPECT(fmon->nickname != NULL);
        EXPECT_EQ(StringCompare(fmon->nickname, expected), 0);
        EXPECT_LT(StringLength(fmon->nickname), POKEMON_NAME_LENGTH);
        CreateFacilityMon(fmon, 50, 31, 0x12345678, 0, mon);
        GetMonData(mon, MON_DATA_NICKNAME, nickname);
        EXPECT_EQ(StringCompare(nickname, expected), 0);
        EXPECT_EQ(GetMonData(mon, MON_DATA_SPECIES), species);
    }
}
