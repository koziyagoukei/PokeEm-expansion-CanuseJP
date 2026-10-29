#include "global.h"
#include "battle.h"
#include "battle_main.h"
#include "party_menu.h"
#include "pokemon.h"
#include "test/test.h"

#define TEST_MENU_DIR_DOWN     1
#define TEST_MENU_DIR_UP      -1
#define TEST_MENU_DIR_RIGHT    2

static void SetTestPartySize(enum BattleTrainer trainer, u8 partySize)
{
    for (u32 i = 0; i < PARTY_SIZE; i++)
        ZeroMonData(&gParties[trainer][i]);

    for (u32 i = 0; i < partySize; i++)
        CreateMon(&gParties[trainer][i], SPECIES_WOBBUFFET, 50, 0, OTID_STRUCT_PRESET(0));

    gPartiesCount[trainer] = partySize;
}

TEST("Multi party preview uses one page for three allies each even against a six-Pokemon opponent")
{
    struct MultiPartnerMenuPokemon partner[PARTY_SIZE] = {0};
    struct MultiPartnerMenuPokemon *savedPartner = gMultiPartnerParty;
    gMultiPartnerParty = partner;
    SetTestPartySize(B_TRAINER_PLAYER, MULTI_PARTY_SIZE);
    SetTestPartySize(B_TRAINER_OPPONENT_A, PARTY_SIZE);
    for (u32 slot = 0; slot < MULTI_PARTY_SIZE; slot++)
        partner[slot].species = SPECIES_WOBBUFFET;

    EXPECT(!Test_MultiPartyPreviewNeedsFullLayout());
    gMultiPartnerParty = savedPartner;
}

TEST("Multi party preview preserves additional player or partner Pokemon")
{
    u32 playerSize = 3;
    u32 partnerSize = 3;
    PARAMETRIZE { playerSize = 4; partnerSize = 3; }
    PARAMETRIZE { playerSize = 3; partnerSize = 4; }
    PARAMETRIZE { playerSize = 6; partnerSize = 6; }
    struct MultiPartnerMenuPokemon partner[PARTY_SIZE] = {0};
    struct MultiPartnerMenuPokemon *savedPartner = gMultiPartnerParty;
    gMultiPartnerParty = partner;
    SetTestPartySize(B_TRAINER_PLAYER, playerSize);
    for (u32 slot = 0; slot < partnerSize; slot++)
        partner[slot].species = SPECIES_WOBBUFFET;

    EXPECT(Test_MultiPartyPreviewNeedsFullLayout());
    gMultiPartnerParty = savedPartner;
}

TEST("Multi party preview also detects a nonempty last slot after a gap")
{
    struct MultiPartnerMenuPokemon partner[PARTY_SIZE] = {0};
    struct MultiPartnerMenuPokemon *savedPartner = gMultiPartnerParty;
    gMultiPartnerParty = partner;
    SetTestPartySize(B_TRAINER_PLAYER, MULTI_PARTY_SIZE);
    partner[PARTY_SIZE - 1].species = SPECIES_WOBBUFFET;

    EXPECT(Test_MultiPartyPreviewNeedsFullLayout());
    gMultiPartnerParty = savedPartner;
}

TEST("Full multi partner party menu stops down navigation at partner party count")
{
    SetTestPartySize(B_TRAINER_PLAYER, PARTY_SIZE);
    SetTestPartySize(B_TRAINER_PARTNER, 2);
    gPartyMenu.layout = PARTY_LAYOUT_MULTI_FULL_PARTNER;

    EXPECT_EQ(Test_UpdatePartySelectionSingleLayout(1, TEST_MENU_DIR_DOWN, FALSE, 0), PARTY_SIZE + 1);
}

TEST("Full multi partner party menu allows down navigation through partner party count")
{
    SetTestPartySize(B_TRAINER_PLAYER, 2);
    SetTestPartySize(B_TRAINER_PARTNER, PARTY_SIZE);
    gPartyMenu.layout = PARTY_LAYOUT_MULTI_FULL_PARTNER;

    EXPECT_EQ(Test_UpdatePartySelectionSingleLayout(1, TEST_MENU_DIR_DOWN, FALSE, 0), 2);
}

TEST("Full multi partner party menu wraps cancel up to partner party count")
{
    SetTestPartySize(B_TRAINER_PLAYER, PARTY_SIZE);
    SetTestPartySize(B_TRAINER_PARTNER, 2);
    gPartyMenu.layout = PARTY_LAYOUT_MULTI_FULL_PARTNER;

    EXPECT_EQ(Test_UpdatePartySelectionSingleLayout(PARTY_SIZE + 1, TEST_MENU_DIR_UP, FALSE, 0), 1);
}
