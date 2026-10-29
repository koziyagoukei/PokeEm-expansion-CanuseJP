#include "global.h"
#include "event_data.h"
#include "frontier_pokevirus.h"
#include "frontier_story.h"
#include "malloc.h"
#include "pokedex.h"
#include "pokedex_common.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"
#include "constants/characters.h"

extern struct PokedexView *sPokedexView;

static const u16 sRescues[] =
{
    FLAG_STORY_RESCUED_TOWER, FLAG_STORY_RESCUED_DOME, FLAG_STORY_RESCUED_PALACE,
    FLAG_STORY_RESCUED_ARENA, FLAG_STORY_RESCUED_FACTORY, FLAG_STORY_RESCUED_PIKE,
    FLAG_STORY_RESCUED_PYRAMID, FLAG_STORY_RESCUED_CONTEST,
};

static void PrepareEpilogue(void)
{
    VarSet(VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE);
    VarSet(VAR_STORY_ENDING_STATE, ENDING_STATE_DONE);
    FlagSet(FLAG_STORY_POKEVIRUS_FADED);
    for (u32 i = 0; i < ARRAY_COUNT(sRescues); i++)
        FlagSet(sRescues[i]);
    for (u32 flag = FLAG_STORY_NAMED_STATES_START; flag <= FLAG_STORY_NAMED_STATES_END; flag++)
        FlagSet(flag);
    memset(&gSaveBlock3Ptr->pokevirus, 0, sizeof(gSaveBlock3Ptr->pokevirus));
}

TEST("Frontier epilogue: every scenario and every named person must be rescued after the ending")
{
    PrepareEpilogue();
    EXPECT(FrontierPokevirus_CanName());
    for (u32 i = 0; i < ARRAY_COUNT(sRescues); i++)
    {
        FlagClear(sRescues[i]);
        EXPECT(!FrontierPokevirus_CanName());
        FlagSet(sRescues[i]);
    }
    for (u32 i = 0; i < NAMED_STORY_COUNT; i++)
    {
        u32 flag = FLAG_STORY_NAMED_STATES_START + i * 2;
        FlagClear(flag);
        EXPECT(!FrontierPokevirus_CanName());
        FlagSet(flag);
    }
    VarSet(VAR_STORY_ENDING_STATE, ENDING_STATE_HUB);
    EXPECT(!FrontierPokevirus_CanName());
    VarSet(VAR_STORY_ENDING_STATE, ENDING_STATE_DONE);
    FlagClear(FLAG_STORY_POKEVIRUS_FADED);
    EXPECT(!FrontierPokevirus_CanName());
    FlagSet(FLAG_STORY_POKEVIRUS_FADED);
    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    EXPECT(!FrontierPokevirus_CanName());
}

TEST("Frontier epilogue: the player's name survives a record round trip and cannot be overwritten")
{
    static const u8 name[] = _("ユメツナギ");
    PrepareEpilogue();
    EXPECT(FrontierPokevirus_RegisterName(name));
    EXPECT(FrontierPokevirus_IsRegistered());
    EXPECT(!FrontierPokevirus_CanName());
    EXPECT(!FrontierPokevirus_RegisterName(COMPOUND_STRING("アアア")));
    struct FrontierPokevirusSave saved = gSaveBlock3Ptr->pokevirus;
    memset(&gSaveBlock3Ptr->pokevirus, 0xFF, sizeof(saved));
    EXPECT(!FrontierPokevirus_IsRegistered());
    gSaveBlock3Ptr->pokevirus = saved;
    EXPECT(FrontierPokevirus_IsRegistered());
    EXPECT_EQ(StringCompare(gSaveBlock3Ptr->pokevirus.name, name), 0);
    u8 display[POKEMON_NAME_LENGTH + 3];
    FrontierPokevirus_CopyName(display);
    EXPECT_EQ(display[0], EXT_CTRL_CODE_BEGIN);
    EXPECT_EQ(display[1], EXT_CTRL_CODE_JPN);
    EXPECT_EQ(StringCompare(display + 2, name), 0);
    gSaveBlock3Ptr->pokevirus.name[0] ^= 1;
    EXPECT(!FrontierPokevirus_IsRegistered());
}

TEST("Frontier epilogue: invalid or truncated names never become saved records")
{
    static const u8 controls[] = {EXT_CTRL_CODE_BEGIN, PLACEHOLDER_BEGIN, CHAR_NEWLINE, CHAR_PROMPT_CLEAR, CHAR_PROMPT_SCROLL};
    PrepareEpilogue();
    EXPECT(!FrontierPokevirus_RegisterName(COMPOUND_STRING("")));
    EXPECT(!FrontierPokevirus_RegisterName(COMPOUND_STRING("   ")));
    for (u32 i = 0; i < ARRAY_COUNT(controls); i++)
    {
        u8 name[] = {controls[i], EOS};
        EXPECT(!FrontierPokevirus_RegisterName(name));
    }
    u8 tooLong[POKEMON_NAME_LENGTH + 2];
    memset(tooLong, CHAR_A, sizeof(tooLong));
    tooLong[sizeof(tooLong) - 1] = EOS;
    EXPECT(!FrontierPokevirus_RegisterName(tooLong));
    tooLong[POKEMON_NAME_LENGTH] = EOS;
    EXPECT(FrontierPokevirus_RegisterName(tooLong));
}

TEST("Frontier epilogue: the non-existent Pokemon category and description fit the dex record")
{
    EXPECT_EQ(StringCompare(gPokevirusDexCategory, COMPOUND_STRING("{JPN}ひじつざいポケモン")), 0);
    EXPECT_LE(GetStringWidth(FONT_NORMAL, gPokevirusDexCategory, 0), 108);
    EXPECT_LE(GetStringWidth(FONT_NORMAL, gPokevirusDexDescription, 0), 224);
}

TEST("Frontier epilogue: the unnumbered record is pinned without losing the final national entry or changing completion counts")
{
    PrepareEpilogue();
    EnableNationalPokedex();
    for (u32 number = 1; number <= NATIONAL_DEX_COUNT; number++)
        GetSetPokedexFlag(number, FLAG_SET_SEEN);
    u32 seen = GetNationalPokedexCount(FLAG_GET_SEEN);
    u32 caught = GetNationalPokedexCount(FLAG_GET_CAUGHT);
    sPokedexView = AllocZeroed(sizeof(*sPokedexView));
    ResetPokedexView(sPokedexView);
    CreatePokedexList(DEX_MODE_NATIONAL, ORDER_NUMERICAL);
    u32 count = sPokedexView->pokemonListCount;
    u32 first = sPokedexView->pokedexList[0].dexNum;
    u32 last = sPokedexView->pokedexList[count - 1].dexNum;
    EXPECT(FrontierPokevirus_RegisterName(COMPOUND_STRING("ユメツナギ")));
    CreatePokedexList(DEX_MODE_NATIONAL, ORDER_NUMERICAL);
    EXPECT_EQ(sPokedexView->pokemonListCount, count + 1);
    EXPECT_EQ(sPokedexView->pokedexList[0].dexNum, POKEVIRUS_DEX_RECORD);
    EXPECT_EQ(sPokedexView->pokedexList[1].dexNum, first);
    EXPECT_EQ(sPokedexView->pokedexList[count].dexNum, last);
    EXPECT_EQ(sPokedexView->pokedexList[count + 1].dexNum, 0xFFFF);
    EXPECT_EQ((u32)sPokedexView->pokedexList[0].owned, FALSE);
    EXPECT_EQ(GetSetPokedexFlag(POKEVIRUS_DEX_RECORD, FLAG_GET_SEEN), TRUE);
    GetSetPokedexFlag(POKEVIRUS_DEX_RECORD, FLAG_SET_CAUGHT);
    EXPECT_EQ(GetSetPokedexFlag(POKEVIRUS_DEX_RECORD, FLAG_GET_CAUGHT), FALSE);
    sPokedexView->formSpecies = SPECIES_RAYQUAZA;
    EXPECT_EQ(NationalPokedexNumToSpeciesForm(POKEVIRUS_DEX_RECORD), SPECIES_NONE);
    sPokedexView->formSpecies = SPECIES_NONE;
    EXPECT_EQ(GetNationalPokedexCount(FLAG_GET_SEEN), seen);
    EXPECT_EQ(GetNationalPokedexCount(FLAG_GET_CAUGHT), caught);
    CreatePokedexList(DEX_MODE_NATIONAL, ORDER_ALPHABETICAL);
    EXPECT_EQ(sPokedexView->pokedexList[0].dexNum, POKEVIRUS_DEX_RECORD);
    for (u32 i = 1; i < sPokedexView->pokemonListCount; i++)
        EXPECT_NE(sPokedexView->pokedexList[i].dexNum, POKEVIRUS_DEX_RECORD);
    FREE_AND_SET_NULL(sPokedexView);
}
