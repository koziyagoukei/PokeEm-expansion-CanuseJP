#include "global.h"
#include "event_data.h"
#include "frontier_pokevirus.h"
#include "frontier_story.h"
#include "naming_screen.h"
#include "overworld.h"
#include "pokedex.h"
#include "string_util.h"
#include "util.h"
#include "constants/characters.h"

#define POKEVIRUS_NAME_MAGIC 0x504B4E4D

STATIC_ASSERT(sizeof(struct FrontierPokevirusSave) == 20, PokevirusRecordSaveSize);

static const u8 sDefaultName[] = _("ヒトポケルス");
const u8 gPokevirusDexCategory[] = _("{JPN}ひじつざいポケモン");
const u8 gPokevirusDexDescription[] = _("{JPN}皆と 生きようとして\n全てを 奪ってしまったもの。\nその名だけは ここに 残されている。");

STATIC_ASSERT(offsetof(struct SaveBlock3, pokevirus) >= offsetof(struct SaveBlock3, fusions)
              + sizeof(((struct SaveBlock3 *)0)->fusions), PokevirusRecordIsAppendOnly);

static bool32 IsValidName(const u8 *name)
{
    bool32 hasLetter = FALSE;
    for (u32 i = 0; i <= POKEMON_NAME_LENGTH; i++)
    {
        u32 c = name[i];
        if (c == EOS)
            return hasLetter;
        if (c == EXT_CTRL_CODE_BEGIN || c == PLACEHOLDER_BEGIN || c == CHAR_NEWLINE
         || c == CHAR_PROMPT_CLEAR || c == CHAR_PROMPT_SCROLL)
            return FALSE;
        hasLetter |= c != CHAR_SPACE;
    }
    return FALSE;
}

bool32 FrontierPokevirus_IsRegistered(void)
{
    const struct FrontierPokevirusSave *record = &gSaveBlock3Ptr->pokevirus;
    return record->magic == POKEVIRUS_NAME_MAGIC
        && record->checksum == CalcCRC16((const u8 *)record, offsetof(struct FrontierPokevirusSave, checksum))
        && IsValidName(record->name);
}

bool32 FrontierPokevirus_CanName(void)
{
    return !FrontierPokevirus_IsRegistered()
        && FlagGet(FLAG_STORY_POKEVIRUS_FADED)
        && VarGet(VAR_STORY_ENDING_STATE) == ENDING_STATE_DONE
        && FrontierStory_AreAllRescued();
}

bool32 FrontierPokevirus_RegisterName(const u8 *name)
{
    struct FrontierPokevirusSave *record = &gSaveBlock3Ptr->pokevirus;
    if (!FrontierPokevirus_CanName() || !IsValidName(name))
        return FALSE;
    memset(record, 0, sizeof(*record));
    StringCopy(record->name, name);
    record->magic = POKEVIRUS_NAME_MAGIC;
    record->checksum = CalcCRC16((const u8 *)record, offsetof(struct FrontierPokevirusSave, checksum));
    return TRUE;
}

bool32 FrontierPokevirus_IsDexRecord(u32 number)
{
    return number == POKEVIRUS_DEX_RECORD && FrontierPokevirus_IsRegistered();
}

u8 *FrontierPokevirus_CopyName(u8 *dest)
{
    u8 *end = StringCopy(dest, COMPOUND_STRING("{JPN}"));
    return StringCopy(end, FrontierPokevirus_IsRegistered() ? gSaveBlock3Ptr->pokevirus.name : sDefaultName);
}

void FrontierPokevirus_CheckEpilogue(void)
{
    gSpecialVar_Result = FrontierPokevirus_CanName();
}

void FrontierPokevirus_Name(void)
{
    StringCopy(gStringVar2, sDefaultName);
    DoNamingScreen(NAMING_SCREEN_POKEVIRUS, gStringVar2, 0, 0, 0, CB2_ReturnToFieldContinueScript);
}

void FrontierPokevirus_Register(void)
{
    gSpecialVar_Result = FrontierPokevirus_RegisterName(gStringVar2);
    FrontierPokevirus_CopyName(gStringVar1);
}

void FrontierPokevirus_ShowRecord(void)
{
    Pokedex_OpenFromScript();
}
