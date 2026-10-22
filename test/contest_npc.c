#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_gimmick.h"
#include "battle_interface.h"
#include "contest.h"
#include "contest_effect.h"
#include "contest_npc.h"
#include "contest_util.h"
#include "event_data.h"
#include "frontier_tutor.h"
#include "frontier_util.h"
#include "gpu_regs.h"
#include "malloc.h"
#include "move.h"
#include "pokemon.h"
#include "sprite.h"
#include "string_util.h"
#include "text.h"
#include "util.h"
#include "constants/form_change_types.h"
#include "test/link.h"
#include "test/test.h"

extern const struct ContestPokemon gContestOpponents[];

static void InitContest(void)
{
    gContestResources = AllocZeroed(sizeof(*gContestResources));
    gContestResources->contest = AllocZeroed(sizeof(struct Contest));
    gContestResources->status = AllocZeroed(CONTESTANT_COUNT * sizeof(struct ContestantStatus));
    gContestResources->gimmickStatus = AllocZeroed(CONTESTANT_COUNT * sizeof(struct ContestGimmickStatus));
    gContestResources->appealResults = AllocZeroed(sizeof(struct ContestAppealMoveResults));
    gContestResources->excitement = AllocZeroed(sizeof(struct ContestExcitement));
    gContestResources->aiData = AllocZeroed(sizeof(struct ContestAIInfo));
    gContestResources->moveAnim = AllocZeroed(sizeof(struct ContestMoveAnimData));
    gLinkContestFlags = 0;
    gContestPlayerMonIndex = 3;
    gSpecialVar_ContestCategory = CONTEST_CATEGORY_COOL;
    for (u32 i = 0; i < CONTESTANT_COUNT; i++)
    {
        gContestMons[i] = (struct ContestPokemon){.species = SPECIES_PIKACHU};
        gContestantTurnOrder[i] = i;
        eContest.prevTurnOrder[i] = i;
        eContestAppealResults.turnOrder[i] = i;
        eContestantStatus[i].effectStringId = CONTEST_STRING_NONE;
        eContestantStatus[i].effectStringId2 = CONTEST_STRING_NONE;
    }
}

static void FreeContest(void)
{
    FREE_AND_SET_NULL(gContestResources->contest);
    FREE_AND_SET_NULL(gContestResources->status);
    FREE_AND_SET_NULL(gContestResources->gimmickStatus);
    FREE_AND_SET_NULL(gContestResources->appealResults);
    FREE_AND_SET_NULL(gContestResources->excitement);
    FREE_AND_SET_NULL(gContestResources->aiData);
    FREE_AND_SET_NULL(gContestResources->moveAnim);
    FREE_AND_SET_NULL(gContestResources);
    gLinkContestFlags = 0;
}

static bool32 CanLearn(enum Species species, enum Move move)
{
    const u16 *learnsets[] = {
        GetFrontierFullLearnset(species),
        GetFrontierEventLearnset(species),
        GetFrontierZaDlcLearnset(species),
        GetSpeciesTeachableLearnset(species),
        GetSpeciesEggMoves(species),
    };
    const struct LevelUpMove *levelUp = GetSpeciesLevelUpLearnset(species);
    const struct Fusion *fusion = gFusionTablePointers[species];
    for (u32 set = 0; set < ARRAY_COUNT(learnsets); set++)
    {
        const u16 *learnset = learnsets[set];
        if (learnset == NULL)
            continue;
        for (u32 i = 0; learnset[i] != MOVE_UNAVAILABLE; i++)
            if (learnset[i] == move)
                return TRUE;
    }
    for (u32 i = 0; levelUp[i].move != MOVE_UNAVAILABLE; i++)
        if (levelUp[i].move == move)
            return TRUE;
    // The Rotom Catalog teaches this form move in CursorCb_CatalogWashing.
    if (species == SPECIES_ROTOM_WASH && move == MOVE_HYDRO_PUMP)
        return TRUE;
    // Fused Necrozma learns its signature move through the actual fusion table.
    if (fusion != NULL)
        for (u32 i = 0; fusion[i].fusionStorageIndex != FUSION_TERMINATOR; i++)
            if (fusion[i].fusingIntoMon == species && fusion[i].fusionMove == move)
                return TRUE;
    return FALSE;
}

static enum ContestCategories FirstCategory(const struct ContestNpcPreset *preset)
{
    for (u32 i = 0; i < CONTEST_CATEGORIES_COUNT; i++)
        if (preset->categories & (1 << i))
            return i;
    return CONTEST_CATEGORIES_COUNT;
}

static u32 FindPreset(enum Species species, enum ContestGimmick gimmick)
{
    for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
        if (Contest_GetNpcPreset(i)->species == species && Contest_GetNpcPreset(i)->gimmick == gimmick)
            return i;
    return Contest_GetNpcPresetCount();
}

TEST("Contest NPC: every preset has four legal base moves and an explicit gimmick")
{
    u32 opponent = 0;
    for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
        PARAMETRIZE { opponent = i; }
    const struct ContestNpcPreset *preset = Contest_GetNpcPreset(opponent);
    EXPECT_EQ(Contest_GetNpcPresetCount(), 208);
    EXPECT(Contest_GetNpcPreset(Contest_GetNpcPresetCount()) == NULL);
    EXPECT(Contest_GetNpcTemplate(Contest_GetNpcPresetCount()) == NULL);
    EXPECT_GT(preset->species, SPECIES_NONE);
    EXPECT_LT(preset->species, NUM_SPECIES);
    EXPECT_GT(preset->gimmick, CONTEST_GIMMICK_NONE);
    EXPECT_LT(preset->gimmick, CONTEST_GIMMICK_COUNT);
    EXPECT_GE(preset->activationTurn, 1);
    EXPECT_LE(preset->activationTurn, CONTEST_NUM_APPEALS);
    EXPECT_NE(preset->categories, 0);
    for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
    {
        enum Move move = preset->moves[slot];
        EXPECT_GT(move, MOVE_NONE);
        EXPECT_LT(move, MOVES_COUNT);
        EXPECT_EQ(CanLearn(preset->species, move) ? MOVE_NONE : move, MOVE_NONE);
        EXPECT_LT(GetMoveContestCategory(move), CONTEST_CATEGORIES_COUNT);
        for (u32 other = 0; other < slot; other++)
            EXPECT_NE(move, preset->moves[other]);
    }
}

TEST("Contest NPC: each category and rank has at least three opponents before and after the story")
{
    u32 rank = 0, category = 0, postgame = 0;
    for (u32 r = CONTEST_RANK_NORMAL; r <= CONTEST_RANK_MASTER; r++)
        for (u32 c = 0; c < CONTEST_CATEGORIES_COUNT; c++)
            for (u32 p = 0; p < 2; p++)
                PARAMETRIZE { rank = r; category = c; postgame = p; }
    u32 count = 0;
    for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
    {
        const struct ContestPokemon *identity = Contest_GetNpcTemplate(i);
        if (identity->whichRank != rank)
            continue;
        if ((postgame && identity->filter == CONTEST_FILTER_NO_POSTGAME)
            || (!postgame && identity->filter == CONTEST_FILTER_ONLY_POSTGAME))
            continue;
        if (Contest_GetNpcPreset(i)->categories & (1 << category))
            count++;
    }
    EXPECT_GE(count, CONTESTANT_COUNT - 1);
}

TEST("Contest NPC: metadata survives initialization and planned gimmick is selected on its turn")
{
    u32 opponent = 0;
    for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
        PARAMETRIZE { opponent = i; }
    const struct ContestNpcPreset *preset = Contest_GetNpcPreset(opponent);
    InitContest();
    gSpecialVar_ContestCategory = FirstCategory(preset);
    Test_ContestLoadNpc(0, opponent, gSpecialVar_ContestCategory);
    EXPECT_EQ(eContestGimmickStatus[0].heldItem, preset->heldItem);
    EXPECT_EQ(eContestGimmickStatus[0].teraType, gContestMons[0].teraType);
    EXPECT(eContestGimmickStatus[0].gmaxFactor == preset->gmaxFactor);
    EXPECT_EQ(eContestGimmickStatus[0].originalSpecies, preset->species);
    EXPECT_EQ(eContestGimmickStatus[0].dynamaxLevel, MAX_DYNAMAX_LEVEL);
    if (preset->activationTurn > 1)
    {
        Test_ContestChooseNpcMove(0);
        EXPECT_EQ(eContestGimmickStatus[0].selected, CONTEST_GIMMICK_NONE);
    }
    eContest.appealNumber = preset->activationTurn - 1;
    u8 move = Test_ContestChooseNpcMove(0);
    EXPECT_LT(move, MAX_MON_MOVES);
    EXPECT_EQ(eContestGimmickStatus[0].selected, preset->gimmick);
    eContestantStatus[0].currMove = gContestMons[0].moves[move];
    Test_ContestActivateGimmick(0);
    EXPECT_EQ(eContestGimmickStatus[0].moveGimmick, preset->gimmick);
    EXPECT_EQ(eContestGimmickStatus[0].usedGimmick, preset->gimmick);
    if (preset->gimmick == CONTEST_GIMMICK_Z_APPEAL)
        EXPECT(eContestGimmickStatus[0].zUsed);
    EXPECT(!Test_ContestGimmickCanSelect(0, CONTEST_GIMMICK_TERA, MOVE_TACKLE));
    EXPECT(!Test_ContestGimmickCanSelect(0, CONTEST_GIMMICK_DYNAMAX, MOVE_TACKLE));
    FreeContest();
}

TEST("Contest NPC: Tera category matches the entered contest even for multi-category entrants")
{
    u32 opponent = 0, category = 0;
    for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
    {
        const struct ContestNpcPreset *preset = Contest_GetNpcPreset(i);
        if (preset->gimmick != CONTEST_GIMMICK_TERA || preset->teraType == TYPE_STELLAR)
            continue;
        for (u32 c = 0; c < CONTEST_CATEGORIES_COUNT; c++)
            if (preset->categories & (1 << c))
                PARAMETRIZE { opponent = i; category = c; }
    }
    InitContest();
    gSpecialVar_ContestCategory = category;
    Test_ContestLoadNpc(0, opponent, category);
    eContest.appealNumber = Contest_GetNpcPreset(opponent)->activationTurn - 1;
    u8 move = Test_ContestChooseNpcMove(0);
    eContestantStatus[0].currMove = gContestMons[0].moves[move];
    Test_CalculateAppealMoveImpact(0);
    EXPECT(eContestGimmickStatus[0].teraActive);
    EXPECT_EQ(eContestantStatus[0].moveCategory, category);
    FreeContest();
}

TEST("Contest NPC: normal entry loads three distinct presets in every category and rank")
{
    u32 rank = 0, category = 0;
    for (u32 r = CONTEST_RANK_NORMAL; r <= CONTEST_RANK_MASTER; r++)
        for (u32 c = 0; c < CONTEST_CATEGORIES_COUNT; c++)
            PARAMETRIZE { rank = r; category = c; }
    InitContest();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 50, 0, OTID_STRUCT_PLAYER_ID);
    gContestMonPartyIndex = 0;
    gSpecialVar_ContestCategory = category;
    SetContestants(category, rank);
    EXPECT_EQ(gContestPlayerMonIndex, 3);
    for (u32 slot = 0; slot < CONTESTANT_COUNT - 1; slot++)
    {
        u32 opponent = Contest_GetNpcPresetCount();
        for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
            if (memcmp(gContestMons[slot].trainerName, Contest_GetNpcTemplate(i)->trainerName, sizeof(gContestMons[slot].trainerName)) == 0)
                opponent = i;
        EXPECT_LT(opponent, Contest_GetNpcPresetCount());
        const struct ContestNpcPreset *preset = Contest_GetNpcPreset(opponent);
        EXPECT_EQ(gContestMons[slot].species, preset->species);
        EXPECT_EQ(gContestMons[slot].heldItem, preset->heldItem);
        EXPECT(gContestMons[slot].whichRank == rank);
        EXPECT(preset->categories & (1 << category));
        EXPECT(memcmp(gContestMons[slot].moves, preset->moves, sizeof(preset->moves)) == 0);
        for (u32 other = 0; other < slot; other++)
            EXPECT(memcmp(gContestMons[slot].trainerName, gContestMons[other].trainerName, sizeof(gContestMons[slot].trainerName)) != 0);
    }
    FreeContest();
}

TEST("Contest NPC: every rank and category adds ten distinct allowed species with individual names")
{
    u32 rank = 0, category = 0;
    for (u32 r = CONTEST_RANK_NORMAL; r <= CONTEST_RANK_MASTER; r++)
        for (u32 c = 0; c < CONTEST_CATEGORIES_COUNT; c++)
            PARAMETRIZE { rank = r; category = c; }
    bool32 allowed = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    InitContest();
    u32 count = 0;
    for (u32 i = 96; i < Contest_GetNpcPresetCount(); i++)
    {
        const struct ContestNpcPreset *preset = Contest_GetNpcPreset(i);
        const struct ContestPokemon *identity = Contest_GetNpcTemplate(i);
        if (identity->whichRank != rank || !(preset->categories & (1 << category))
            || IsFrontierSpeciesBannedByCurrentRules(preset->species))
            continue;
        count++;
        Test_ContestLoadNpc(0, i, category);
        EXPECT_GT(StringLength(gContestMons[0].nickname), 0);
        EXPECT_LE(StringLength(gContestMons[0].nickname), POKEMON_NAME_LENGTH);
        EXPECT_NE(StringCompare(gContestMons[0].nickname, GetSpeciesName(preset->species)), 0);
        EXPECT_EQ(StringCompare(gContestMons[0].nickname, identity->nickname), 0);
        for (u32 j = 96; j < i; j++)
            if (Contest_GetNpcTemplate(j)->whichRank == rank)
                EXPECT_NE(preset->species, Contest_GetNpcPreset(j)->species);
    }
    EXPECT_GE(count, 10);
    if (allowed)
        FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    FreeContest();
}

TEST("Contest NPC: Frontier BAN setting controls restricted guests in actual normal selection")
{
    bool32 allow = FALSE;
    PARAMETRIZE { allow = FALSE; }
    PARAMETRIZE { allow = TRUE; }
    bool32 original = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    if (allow)
        FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    else
        FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    InitContest();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 50, 0, OTID_STRUCT_PLAYER_ID);
    gContestMonPartyIndex = 0;
    bool32 sawNecrozma = FALSE;
    for (u32 trial = 0; trial < 128; trial++)
    {
        SetContestants(CONTEST_CATEGORY_SMART, CONTEST_RANK_MASTER);
        for (u32 slot = 0; slot < CONTESTANT_COUNT - 1; slot++)
        {
            enum Species species = gContestMons[slot].species;
            EXPECT(!IsFrontierSpeciesBannedByCurrentRules(species));
            if (species == SPECIES_NECROZMA_DUSK_MANE || species == SPECIES_NECROZMA_DAWN_WINGS)
                sawNecrozma = TRUE;
        }
    }
    EXPECT_EQ(sawNecrozma, allow);
    if (original)
        FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    else
        FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    FreeContest();
}

TEST("Contest NPC: new strategies have a matching appeal in each specialty or convert it with Tera")
{
    u32 opponent = 96;
    for (u32 i = 96; i < Contest_GetNpcPresetCount(); i++)
        PARAMETRIZE { opponent = i; }
    const struct ContestNpcPreset *preset = Contest_GetNpcPreset(opponent);
    u32 categories = 0;
    for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
        categories |= 1 << GetMoveContestCategory(preset->moves[slot]);
    if (preset->gimmick != CONTEST_GIMMICK_TERA || preset->teraType == TYPE_STELLAR)
        EXPECT_EQ(categories & preset->categories, preset->categories);
}

TEST("Contest entry: player may enter restricted species with either NPC BAN setting")
{
    enum Species species = SPECIES_NECROZMA_DUSK_MANE;
    PARAMETRIZE { species = SPECIES_NECROZMA_DUSK_MANE; }
    PARAMETRIZE { species = SPECIES_NECROZMA_DAWN_WINGS; }
    PARAMETRIZE { species = SPECIES_DIANCIE; }
    PARAMETRIZE { species = SPECIES_PIKACHU; }
    bool32 original = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    gSpecialVar_ContestCategory = CONTEST_CATEGORY_COOL;
    gSpecialVar_ContestRank = CONTEST_RANK_NORMAL;
    CreateMon(&gParties[B_TRAINER_PLAYER][0], species, 50, 0, OTID_STRUCT_PLAYER_ID);
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP, &(u16){1});
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    EXPECT_EQ(GetContestEntryEligibility(&gParties[B_TRAINER_PLAYER][0]), CAN_ENTER_CONTEST_EQUAL_RANK);
    FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    EXPECT_EQ(GetContestEntryEligibility(&gParties[B_TRAINER_PLAYER][0]), CAN_ENTER_CONTEST_EQUAL_RANK);
    gSpecialVar_ContestRank = CONTEST_RANK_SUPER;
    EXPECT_EQ(GetContestEntryEligibility(&gParties[B_TRAINER_PLAYER][0]), CANT_ENTER_CONTEST);
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP, &(u16){0});
    EXPECT_EQ(GetContestEntryEligibility(&gParties[B_TRAINER_PLAYER][0]), CANT_ENTER_CONTEST_FAINTED);
    if (!original)
        FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
}

TEST("Contest entry: rejected registration preserves the roster and points")
{
    u8 rejection = CANT_ENTER_CONTEST_EGG;
    PARAMETRIZE { rejection = CANT_ENTER_CONTEST_EGG; }
    PARAMETRIZE { rejection = CANT_ENTER_CONTEST_FAINTED; }
    PARAMETRIZE { rejection = CANT_ENTER_CONTEST; }
    bool32 original = FlagGet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    InitContest();
    gContestMonPartyIndex = 0;
    gSpecialVar_ContestRank = rejection == CANT_ENTER_CONTEST ? CONTEST_RANK_SUPER : CONTEST_RANK_NORMAL;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    CreateMon(mon, SPECIES_NECROZMA, 50, 0, OTID_STRUCT_PLAYER_ID);
    SetMonData(mon, MON_DATA_HP, &(u16){rejection == CANT_ENTER_CONTEST_FAINTED ? 0 : 1});
    SetMonData(mon, MON_DATA_IS_EGG, &(bool8){rejection == CANT_ENTER_CONTEST_EGG});
    struct ContestPokemon roster[CONTESTANT_COUNT];
    memcpy(roster, gContestMons, sizeof(roster));
    for (u32 i = 0; i < CONTESTANT_COUNT; i++)
        gContestMonRound1Points[i] = 123;
    TryEnterContestMon();
    EXPECT_EQ(gSpecialVar_Result, rejection);
    EXPECT_EQ(memcmp(roster, gContestMons, sizeof(roster)), 0);
    for (u32 i = 0; i < CONTESTANT_COUNT; i++)
        EXPECT_EQ(gContestMonRound1Points[i], 123);
    if (original)
        FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    FreeContest();
}

TEST("Contest names: display controls restore Japanese and English after a slash and before suffixes")
{
    static const u8 names[][PLAYER_NAME_LENGTH + 1] = {
        {0x5B, 0x64, 0x5C, EOS},
        {1, 2, 3, 4, 5, 6, 7, EOS},
        {CHAR_A, CHAR_B, CHAR_C, CHAR_D, CHAR_E, CHAR_F, CHAR_G, EOS},
        {CHAR_A, 0x5B, 0x64, CHAR_B, EOS},
        {EXT_CTRL_CODE_BEGIN, EXT_CTRL_CODE_JPN, 0x5B, 0x64, 0x5C, EOS},
    };
    static const u8 *const expected[] = {
        COMPOUND_STRING("{ENG}X/{JPN}サトシのポケモン"),
        COMPOUND_STRING("{ENG}X/{JPN}あいうえおかきのポケモン"),
        COMPOUND_STRING("{ENG}X/ABCDEFG{JPN}のポケモン"),
        COMPOUND_STRING("{ENG}X/A{JPN}サト{ENG}B{JPN}のポケモン"),
        COMPOUND_STRING("{ENG}X/{JPN}サトシのポケモン"),
    };
    u32 entry = 0;
    for (u32 i = 0; i < ARRAY_COUNT(names); i++)
        PARAMETRIZE { entry = i; }
    u8 name[3 * PLAYER_NAME_LENGTH + 5];
    memset(name, 0xCC, sizeof(name));
    u8 *end = Contest_CopyDisplayName(name + 1, names[entry]);
    EXPECT_EQ(name[0], 0xCC);
    EXPECT_EQ(*end, EOS);
    EXPECT_LE(end - (name + 1), 3 * PLAYER_NAME_LENGTH + 2);
    EXPECT_EQ(end[1], 0xCC);
    u8 text[100];
    StringCopy(text, COMPOUND_STRING("{ENG}X/"));
    StringAppend(text, name + 1);
    StringAppend(text, COMPOUND_STRING("のポケモン"));
    EXPECT_EQ(GetStringWidth(FONT_NARROW, text, 0), GetStringWidth(FONT_NARROW, expected[entry], 0));
    EXPECT_EQ(GetStringWidth(FONT_SMALL, text, 0), GetStringWidth(FONT_SMALL, expected[entry], 0));
    u8 latinSuffix[64];
    Contest_CopyDisplayName(latinSuffix, names[entry]);
    StringAppend(latinSuffix, COMPOUND_STRING("ABC"));
    u8 explicitSuffix[64];
    Contest_CopyDisplayName(explicitSuffix, names[entry]);
    StringAppend(explicitSuffix, COMPOUND_STRING("{ENG}ABC"));
    EXPECT_EQ(GetStringWidth(FONT_NARROW, latinSuffix, 0), GetStringWidth(FONT_NARROW, explicitSuffix, 0));
}

TEST("Contest names: maximum-length Japanese player names remain intact in saved and contestant data")
{
    static const u8 name[] = {1, 2, 3, 4, 5, 6, 7, EOS};
    u8 original[PLAYER_NAME_LENGTH + 1];
    memcpy(original, gSaveBlock2Ptr->playerName, sizeof(original));
    memcpy(gSaveBlock2Ptr->playerName, name, sizeof(name));
    InitContest();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 50, 0, OTID_STRUCT_PLAYER_ID);
    CreateContestMonFromParty(0);
    gSpecialVar_0x8006 = gContestPlayerMonIndex;
    BufferContestantTrainerName();
    EXPECT_EQ(memcmp(gContestMons[gContestPlayerMonIndex].trainerName, name, sizeof(name)), 0);
    EXPECT_EQ(memcmp(gSaveBlock2Ptr->playerName, name, sizeof(name)), 0);
    EXPECT_EQ(GetStringWidth(FONT_NORMAL, gStringVar1, 0), GetStringWidth(FONT_NORMAL, COMPOUND_STRING("{JPN}あいうえおかき"), 0));
    memcpy(gSaveBlock2Ptr->playerName, original, sizeof(original));
    FreeContest();
}

TEST("Contest NPC: Mega rewards matching-category appeals unless the entrant was jammed")
{
    bool32 jammed = FALSE;
    PARAMETRIZE { jammed = FALSE; }
    PARAMETRIZE { jammed = TRUE; }
    InitContest();
    Test_ContestLoadNpc(0, FindPreset(SPECIES_ABSOL, CONTEST_GIMMICK_MEGA), CONTEST_CATEGORY_COOL);
    eContestantStatus[0].currMove = MOVE_SLASH;
    eContestGimmickStatus[0].selected = CONTEST_GIMMICK_MEGA;
    eContestGimmickStatus[0].jammedSinceLastAppeal = jammed;
    Test_CalculateAppealMoveImpact(0);
    EXPECT(eContestGimmickStatus[0].megaActive);
    EXPECT_EQ(eContestantStatus[0].appeal, gContestEffects[GetMoveContestEffect(MOVE_SLASH)].appeal + (jammed ? 0 : 20));
    FreeContest();
}

TEST("Contest NPC: Ultra Burst consumes a turn then allows one signature Z appeal")
{
    InitContest();
    Test_ContestLoadNpc(0, FindPreset(SPECIES_NECROZMA_DUSK_MANE, CONTEST_GIMMICK_ULTRA_BURST), CONTEST_CATEGORY_COOL);
    u8 move = Test_ContestChooseNpcMove(0);
    eContestantStatus[0].currMove = gContestMons[0].moves[move];
    Test_CalculateAppealMoveImpact(0);
    EXPECT(eContestGimmickStatus[0].ultraActive);
    EXPECT_EQ(eContestantStatus[0].currMove, MOVE_NONE);
    EXPECT_EQ(eContestantStatus[0].appeal, 0);
    // The cutaway commits the pending form once the transformation animation ends.
    gContestMons[0].species = SPECIES_NECROZMA_ULTRA;
    eContest.appealNumber = 1;
    Test_ContestChooseNpcMove(0);
    EXPECT_EQ(eContestGimmickStatus[0].selected, CONTEST_GIMMICK_NONE);
    eContest.appealNumber = 2;
    move = Test_ContestChooseNpcMove(0);
    EXPECT_EQ(gContestMons[0].moves[move], MOVE_PHOTON_GEYSER);
    eContestantStatus[0].currMove = MOVE_PHOTON_GEYSER;
    Test_CalculateAppealMoveImpact(0);
    EXPECT_EQ(eContestGimmickStatus[0].moveGimmick, CONTEST_GIMMICK_Z_APPEAL);
    EXPECT(eContestGimmickStatus[0].zUsed);
    EXPECT_EQ(eContestantStatus[0].currMove, MOVE_PHOTON_GEYSER);
    EXPECT_EQ(eContestantStatus[0].appeal, gContestEffects[GetMoveContestEffect(MOVE_PHOTON_GEYSER)].appeal + 40);
    EXPECT_EQ(eContestExcitement.moveExcitement, GetMoveContestCategory(MOVE_PHOTON_GEYSER) == gSpecialVar_ContestCategory ? 2 : 1);
    eContest.appealNumber = 3;
    Test_ContestChooseNpcMove(0);
    EXPECT_EQ(eContestGimmickStatus[0].selected, CONTEST_GIMMICK_NONE);
    FreeContest();
}

TEST("Contest NPC: generic and signature Z appeals preserve base moves and apply bonuses only once")
{
    enum Species species = SPECIES_POOCHYENA;
    enum Move baseMove = MOVE_BITE;
    enum ContestCategories category = CONTEST_CATEGORY_TOUGH;
    bool32 signature = FALSE;
    PARAMETRIZE { species = SPECIES_POOCHYENA; baseMove = MOVE_BITE; category = CONTEST_CATEGORY_TOUGH; signature = FALSE; }
    PARAMETRIZE { species = SPECIES_EEVEE; baseMove = MOVE_LAST_RESORT; category = CONTEST_CATEGORY_CUTE; signature = TRUE; }
    InitContest();
    gSpecialVar_ContestCategory = category;
    Test_ContestLoadNpc(0, FindPreset(species, CONTEST_GIMMICK_Z_APPEAL), category);
    eContest.appealNumber = 2;
    eContestantStatus[0].currMove = baseMove;
    eContestGimmickStatus[0].selected = CONTEST_GIMMICK_Z_APPEAL;
    Test_CalculateAppealMoveImpact(0);
    EXPECT_EQ(eContestantStatus[0].currMove, baseMove);
    EXPECT_EQ(eContestantStatus[0].baseAppeal, gContestEffects[GetMoveContestEffect(baseMove)].appeal + 30);
    EXPECT_GE(eContestantStatus[0].appeal, eContestantStatus[0].baseAppeal + (signature ? 10 : 0));
    EXPECT(eContestGimmickStatus[0].zUsed);
    EXPECT_EQ(eContestExcitement.moveExcitement, GetMoveContestCategory(baseMove) == category ? 2 : 1);
    eContest.appealNumber = 3;
    Test_ContestChooseNpcMove(0);
    EXPECT_EQ(eContestGimmickStatus[0].selected, CONTEST_GIMMICK_NONE);
    EXPECT(!Test_ContestGimmickCanSelect(0, CONTEST_GIMMICK_DYNAMAX, baseMove));
    FreeContest();
}

TEST("Contest NPC: Gigantamax has three appeals, no jam, and cannot switch gimmicks afterward")
{
    InitContest();
    gSpecialVar_ContestCategory = CONTEST_CATEGORY_COOL;
    Test_ContestLoadNpc(0, FindPreset(SPECIES_MACHAMP, CONTEST_GIMMICK_DYNAMAX), CONTEST_CATEGORY_COOL);
    for (u32 turn = 1; turn < 4; turn++)
    {
        eContest.appealNumber = turn;
        Test_ContestChooseNpcMove(0);
        eContestantStatus[0].currMove = MOVE_CROSS_CHOP;
        Test_CalculateAppealMoveImpact(0);
        EXPECT_EQ(eContestGimmickStatus[0].dynamaxTurns, 4 - turn);
        EXPECT_EQ(eContestantStatus[0].baseAppeal, GetMoveContestCategory(MOVE_CROSS_CHOP) == CONTEST_CATEGORY_COOL ? 40 : 30);
        EXPECT_EQ(eContestAppealResults.jam, 0);
        EXPECT_EQ(eContestAppealResults.jam2, 0);
        EXPECT_EQ(eContestGimmickStatus[1].extraAppeal, 10);
        Test_ContestAdvanceRound();
        EXPECT_EQ(eContestGimmickStatus[0].dynamaxTurns, 3 - turn);
    }
    eContest.appealNumber = 4;
    Test_ContestChooseNpcMove(0);
    EXPECT_EQ(eContestGimmickStatus[0].selected, CONTEST_GIMMICK_NONE);
    EXPECT(!Test_ContestGimmickCanSelect(0, CONTEST_GIMMICK_TERA, MOVE_CROSS_CHOP));
    FreeContest();
}

TEST("Contest NPC: linked NPCs keep their original roster and never select a gimmick")
{
    InitContest();
    gLinkContestFlags = LINK_CONTEST_FLAG_IS_LINK;
    gNumLinkContestPlayers = 2;
    gContestPlayerMonIndex = 0;
    SetLinkAIContestants(CONTEST_CATEGORY_COOL, CONTEST_RANK_MASTER, TRUE);
    for (u32 slot = 2; slot < CONTESTANT_COUNT; slot++)
    {
        bool32 found = FALSE;
        for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
            if (gContestMons[slot].species == gContestOpponents[i].species
                && memcmp(gContestMons[slot].moves, gContestOpponents[i].moves, sizeof(gContestMons[slot].moves)) == 0)
                found = TRUE;
        EXPECT(found);
        eContest.appealNumber = 2;
        Test_ContestChooseNpcMove(slot);
        EXPECT_EQ(eContestGimmickStatus[slot].selected, CONTEST_GIMMICK_NONE);
        EXPECT_EQ(gContestMons[slot].heldItem, ITEM_NONE);
    }
    FreeContest();
}

TEST("Contest rules: the third G-Max appeal keeps its animation and form until round end")
{
    InitContest();
    Test_ContestLoadNpc(0, FindPreset(SPECIES_MACHAMP, CONTEST_GIMMICK_DYNAMAX), CONTEST_CATEGORY_COOL);
    for (u32 turn = 0; turn < 3; turn++)
    {
        eContestGimmickStatus[0].selected = turn == 0 ? CONTEST_GIMMICK_DYNAMAX : CONTEST_GIMMICK_NONE;
        eContestantStatus[0].currMove = MOVE_CROSS_CHOP;
        Test_CalculateAppealMoveImpact(0);
        if (turn == 0)
            gContestMons[0].species = SPECIES_MACHAMP_GMAX;
        EXPECT(eContestGimmickStatus[0].showGimmickIntro == (turn == 0));
        EXPECT(ContestGimmick_IsDynamaxed(0));
        EXPECT_EQ(Test_ContestGetAnimMove(0), MOVE_G_MAX_CHI_STRIKE);
        EXPECT_EQ(gContestMons[0].species, SPECIES_MACHAMP_GMAX);
        Test_ContestAdvanceRound();
    }
    EXPECT_EQ(eContestGimmickStatus[0].dynamaxTurns, 0);
    EXPECT_EQ(gContestMons[0].species, SPECIES_MACHAMP);
    EXPECT(!eContestGimmickStatus[0].gigantamaxActive);
    EXPECT(!ContestGimmick_IsDynamaxed(0));
    FreeContest();
}

TEST("Contest rules: Dynamax prevents strong jam and nervousness without consuming a shield")
{
    u8 shield = 0;
    PARAMETRIZE { shield = 0; }
    PARAMETRIZE { shield = 1; }
    InitContest();
    eContestGimmickStatus[0].dynamaxTurns = 1;
    eContestantStatus[0].appeal = 80;
    eContestantStatus[0].jamSafetyCount = shield;
    eContestAppealResults.contestant = 1;
    eContestAppealResults.jam = 60;
    gContestEffects[CONTEST_EFFECT_STARTLE_PREV_MONS].function();
    EXPECT_EQ(eContestantStatus[0].appeal, 80);
    EXPECT_EQ(eContestantStatus[0].jam, 0);
    EXPECT_EQ(eContestantStatus[0].jamSafetyCount, shield);
    eContestAppealResults.contestant = 0;
    eContestGimmickStatus[1].dynamaxTurns = 1;
    gContestEffects[CONTEST_EFFECT_MAKE_FOLLOWING_MON_NERVOUS].function();
    EXPECT(!eContestantStatus[1].nervous);
    FreeContest();
}

TEST("Contest rules: variable appeal effects retain Mega Z and Stellar bonuses")
{
    enum ContestGimmick gimmick = CONTEST_GIMMICK_MEGA;
    s16 bonus = 20;
    PARAMETRIZE { gimmick = CONTEST_GIMMICK_MEGA; bonus = 20; }
    PARAMETRIZE { gimmick = CONTEST_GIMMICK_Z_APPEAL; bonus = 30; }
    PARAMETRIZE { gimmick = CONTEST_GIMMICK_TERA; bonus = 10; }
    InitContest();
    gSpecialVar_ContestCategory = GetMoveContestCategory(MOVE_LOW_KICK);
    eContestAppealResults.turnOrder[0] = 2;
    eContestAppealResults.turnOrder[2] = 0;
    eContestantStatus[0].currMove = MOVE_LOW_KICK;
    if (gimmick == CONTEST_GIMMICK_MEGA)
        eContestGimmickStatus[0].megaActive = TRUE;
    else if (gimmick == CONTEST_GIMMICK_Z_APPEAL)
    {
        eContestGimmickStatus[0].heldItem = ITEM_FIGHTINIUM_Z;
        eContestGimmickStatus[0].selected = gimmick;
    }
    else
    {
        eContestGimmickStatus[0].teraActive = TRUE;
        eContestGimmickStatus[0].teraType = TYPE_STELLAR;
    }
    Test_CalculateAppealMoveImpact(0);
    EXPECT_EQ(GetMoveContestEffect(MOVE_LOW_KICK), CONTEST_EFFECT_BETTER_WHEN_LATER);
    EXPECT_EQ(eContestantStatus[0].appeal, 40 + bonus);
    if (gimmick == CONTEST_GIMMICK_TERA)
    {
        EXPECT(eContestGimmickStatus[0].stellarBoostUsed);
        Test_CalculateAppealMoveImpact(0);
        EXPECT_EQ(eContestantStatus[0].appeal, 40);
    }
    FreeContest();
}

TEST("Contest rules: category-based jam uses the performed Tera categories")
{
    InitContest();
    eContestantStatus[0].currMove = MOVE_THUNDERBOLT;
    eContestantStatus[0].moveCategory = CONTEST_CATEGORY_COOL;
    eContestantStatus[0].appeal = 100;
    eContestantStatus[1].currMove = MOVE_POUND;
    eContestantStatus[1].moveCategory = CONTEST_CATEGORY_COOL;
    eContestAppealResults.contestant = 1;
    gContestEffects[CONTEST_EFFECT_STARTLE_MONS_SAME_TYPE_APPEAL].function();
    EXPECT_NE(GetMoveContestCategory(MOVE_POUND), CONTEST_CATEGORY_COOL);
    EXPECT_EQ(eContestantStatus[0].jam, 40);
    FreeContest();
}

TEST("Contest rules: Z and Max animations do not inherit a base move charging replay")
{
    enum ContestGimmick gimmick = CONTEST_GIMMICK_Z_APPEAL;
    PARAMETRIZE { gimmick = CONTEST_GIMMICK_Z_APPEAL; }
    PARAMETRIZE { gimmick = CONTEST_GIMMICK_DYNAMAX; }
    InitContest();
    gBattlerSpriteIds[B_BATTLER_3] = SPRITE_NONE;
    eContestantStatus[0].currMove = MOVE_SOLAR_BEAM;
    eContestGimmickStatus[0].heldItem = ITEM_GRASSIUM_Z;
    eContestGimmickStatus[0].moveGimmick = gimmick;
    if (gimmick == CONTEST_GIMMICK_DYNAMAX)
        eContestGimmickStatus[0].dynamaxTurns = 1;
    Test_ContestPrepareMoveAnimData(0, MOVE_SOLAR_BEAM);
    EXPECT_EQ(eContest.moveAnimTurnCount, 0);
    EXPECT_NE(Test_ContestGetAnimMove(0), MOVE_SOLAR_BEAM);
    FreeContest();
}

TEST("Contest rules: an ordinary charging move keeps both charge and attack phases")
{
    InitContest();
    gBattlerSpriteIds[B_BATTLER_3] = SPRITE_NONE;
    eContestantStatus[0].currMove = MOVE_SOLAR_BEAM;
    Test_ContestPrepareMoveAnimData(0, MOVE_SOLAR_BEAM);
    EXPECT_EQ(eContest.moveAnimTurnCount, 2);
    EXPECT_EQ(gAnimMoveTurn, 0);
    eContest.moveAnimTurnCount = 1;
    Test_ContestPrepareMoveAnimData(0, MOVE_SOLAR_BEAM);
    EXPECT_EQ(eContest.moveAnimTurnCount, 1);
    EXPECT_EQ(gAnimMoveTurn, 1);
    FreeContest();
}

TEST("Contest rules: a nervous Dynamax entrant gives no free hearts and retains its remaining appeals")
{
    InitContest();
    eContestGimmickStatus[0].dynamaxTurns = 2;
    eContestantStatus[0].nervous = TRUE;
    eContestantStatus[0].currMove = MOVE_NONE;
    Test_CalculateAppealMoveImpact(0);
    for (u32 i = 1; i < CONTESTANT_COUNT; i++)
        EXPECT_EQ(eContestGimmickStatus[i].extraAppeal, 0);
    Test_ContestAdvanceRound();
    EXPECT_EQ(eContestGimmickStatus[0].dynamaxTurns, 2);
    FreeContest();
}

TEST("Contest rules: registration loads player metadata before appeal resources exist")
{
    gContestResources = NULL;
    gLinkContestFlags = 0;
    gContestMonPartyIndex = 0;
    gSpecialVar_ContestCategory = CONTEST_CATEGORY_COOL;
    enum Item item = ITEM_GRASSIUM_Z;
    enum Type type = TYPE_STELLAR;
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_VENUSAUR, 50, 0, OTID_STRUCT_PLAYER_ID);
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM, &item);
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_TERA_TYPE, &type);
    SetContestants(CONTEST_CATEGORY_COOL, CONTEST_RANK_NORMAL);
    EXPECT(gContestResources == NULL);
    EXPECT_EQ(gContestMons[gContestPlayerMonIndex].species, SPECIES_VENUSAUR);
    EXPECT_EQ(gContestMons[gContestPlayerMonIndex].heldItem, item);
    EXPECT_EQ(gContestMons[gContestPlayerMonIndex].teraType, type);
}

static void InitContestIcons(void)
{
    InitContest();
    ResetSpriteData();
    FreeAllSpritePalettes();
    gReservedSpritePaletteCount = 4;
    gBattle_BG0_Y = 0;
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    for (u32 i = 0; i < CONTESTANT_COUNT; i++)
        eContestGimmickStatus[i].tabIconSpriteId = SPRITE_NONE;
}

static void FreeContestIcons(void)
{
    Test_ContestDestroyGimmickIcons();
    gReservedSpritePaletteCount = 0;
    gBattle_BG0_Y = 0;
    FreeContest();
}

TEST("Contest icons: active Mega Tera and Dynamax have their existing battle indicators")
{
    InitContestIcons();
    eContestGimmickStatus[0].megaActive = TRUE;
    eContestGimmickStatus[1].teraActive = TRUE;
    eContestGimmickStatus[1].teraType = TYPE_FIRE;
    eContestGimmickStatus[2].dynamaxTurns = 1;
    eContestGimmickStatus[3].selected = CONTEST_GIMMICK_MEGA;
    Test_ContestRefreshGimmickIcons();
    const u16 tags[] = {TAG_MEGA_INDICATOR_TILE, TAG_FIRE_INDICATOR_TILE, TAG_DYNAMAX_INDICATOR_TILE};
    for (u32 i = 0; i < ARRAY_COUNT(tags); i++)
    {
        u8 id = eContestGimmickStatus[i].tabIconSpriteId;
        EXPECT_LT(id, MAX_SPRITES);
        EXPECT(gSprites[id].inUse);
        EXPECT_EQ(GetSpriteTileTagByTileStart(gSprites[id].oam.tileNum), tags[i]);
        EXPECT_EQ(gSprites[id].x, 148);
        EXPECT_EQ(gSprites[id].y, 24 + 40 * i);
        EXPECT(!gSprites[id].invisible);
        EXPECT_EQ((u32)gSprites[id].oam.priority, 0);
    }
    EXPECT_EQ(eContestGimmickStatus[3].tabIconSpriteId, SPRITE_NONE);
    eContestGimmickStatus[2].dynamaxTurns = 0;
    Test_ContestRefreshGimmickIcons();
    EXPECT_EQ(eContestGimmickStatus[2].tabIconSpriteId, SPRITE_NONE);
    EXPECT_LT(eContestGimmickStatus[0].tabIconSpriteId, MAX_SPRITES);
    FreeContestIcons();
}

TEST("Contest icons: every valid Tera type uses its matching indicator")
{
    enum Type type = TYPE_NORMAL;
    for (u32 i = TYPE_NORMAL; i <= TYPE_STELLAR; i++)
        if (i != TYPE_MYSTERY)
            PARAMETRIZE { type = i; }
    InitContestIcons();
    eContestGimmickStatus[0].teraActive = TRUE;
    eContestGimmickStatus[0].teraType = type;
    Test_ContestRefreshGimmickIcons();
    u8 id = eContestGimmickStatus[0].tabIconSpriteId;
    EXPECT_LT(id, MAX_SPRITES);
    EXPECT_EQ(GetSpriteTileTagByTileStart(gSprites[id].oam.tileNum), TAG_NORMAL_INDICATOR_TILE + type - TYPE_NORMAL);
    EXPECT_EQ(GetSpritePaletteTagByPaletteNum(gSprites[id].oam.paletteNum), TAG_TERA_INDICATOR_PAL);
    FreeContestIcons();
}

TEST("Contest icons: shared graphics are released and can be rebuilt without stale IDs")
{
    InitContestIcons();
    for (u32 i = 0; i < CONTESTANT_COUNT; i++)
    {
        eContestGimmickStatus[i].teraActive = TRUE;
        eContestGimmickStatus[i].teraType = TYPE_WATER;
    }
    for (u32 iteration = 0; iteration < 4; iteration++)
    {
        Test_ContestRefreshGimmickIcons();
        u16 tile = GetSpriteTileStartByTag(TAG_WATER_INDICATOR_TILE);
        EXPECT_NE(tile, 0xFFFF);
        for (u32 i = 0; i < CONTESTANT_COUNT; i++)
        {
            u8 id = eContestGimmickStatus[i].tabIconSpriteId;
            EXPECT_LT(id, MAX_SPRITES);
            EXPECT_EQ((u32)gSprites[id].oam.tileNum, tile);
        }
        Test_ContestDestroyGimmickIcons();
        for (u32 i = 0; i < CONTESTANT_COUNT; i++)
            EXPECT_EQ(eContestGimmickStatus[i].tabIconSpriteId, SPRITE_NONE);
        EXPECT_EQ(GetSpriteTileStartByTag(TAG_WATER_INDICATOR_TILE), 0xFFFF);
        EXPECT_EQ(IndexOfSpritePaletteTag(TAG_TERA_INDICATOR_PAL), 0xFF);
    }
    FreeContestIcons();
}

TEST("Contest icons: row changes move the icon and move selection hides covered panels")
{
    InitContestIcons();
    eContestGimmickStatus[0].megaActive = TRUE;
    Test_ContestRefreshGimmickIcons();
    u8 id = eContestGimmickStatus[0].tabIconSpriteId;
    EXPECT_LT(id, MAX_SPRITES);
    gContestantTurnOrder[0] = 3;
    gBattle_BG0_Y = DISPLAY_HEIGHT;
    gSprites[id].callback(&gSprites[id]);
    EXPECT_EQ(gSprites[id].y, 144);
    EXPECT(gSprites[id].invisible);
    gBattle_BG0_Y = 0;
    SetGpuReg(REG_OFFSET_BG0CNT, 1);
    gSprites[id].callback(&gSprites[id]);
    EXPECT(!gSprites[id].invisible);
    EXPECT_EQ((u32)gSprites[id].oam.priority, 1);
    FreeContestIcons();
}

TEST("Contest icons: exhausted sprite slots do not allocate unused indicator graphics")
{
    InitContestIcons();
    for (u32 i = 0; i < MAX_SPRITES; i++)
        EXPECT_LT(CreateInvisibleSpriteWithCallback(SpriteCallbackDummy), MAX_SPRITES);
    eContestGimmickStatus[0].megaActive = TRUE;
    Test_ContestRefreshGimmickIcons();
    EXPECT_EQ(eContestGimmickStatus[0].tabIconSpriteId, SPRITE_NONE);
    EXPECT_EQ(GetSpriteTileStartByTag(TAG_MEGA_INDICATOR_TILE), 0xFFFF);
    EXPECT_EQ(IndexOfSpritePaletteTag(TAG_MEGA_INDICATOR_PAL), 0xFF);
    for (u32 i = 0; i < MAX_SPRITES; i++)
        DestroySprite(&gSprites[i]);
    FreeContestIcons();
}
