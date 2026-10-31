#include "global.h"
#include "battle_main.h"
#include "malloc.h"
#include "move.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokemon_storage_sort.h"
#include "string_util.h"
#include "strings.h"
#include "constants/abilities.h"
#include "constants/characters.h"
#include "constants/moves.h"

// Sort groups, from first to last.
enum
{
    SORT_GROUP_MATCH,
    SORT_GROUP_OTHER,
    SORT_GROUP_EGG,
    SORT_GROUP_EMPTY,
};

struct StorageSortEntry
{
    u32 key;
    u16 index;
};

static struct BoxPokemon *GetStorageMonByIndex(u32 index)
{
    return &gPokemonStoragePtr->boxes[index / IN_BOX_COUNT][index % IN_BOX_COUNT];
}

static bool32 MonMatchesSortValue(struct BoxPokemon *boxMon, enum Species species, enum StorageSortKey key, u32 value)
{
    u32 i;

    switch (key)
    {
    case STORAGE_SORT_TYPE:
        return GetSpeciesType(species, 0) == value || GetSpeciesType(species, 1) == value;
    case STORAGE_SORT_MOVE:
        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            if (GetBoxMonData(boxMon, MON_DATA_MOVE1 + i) == value)
                return TRUE;
        }
        return FALSE;
    case STORAGE_SORT_ABILITY:
        return GetAbilityBySpecies(species, GetBoxMonData(boxMon, MON_DATA_ABILITY_NUM)) == value;
    case STORAGE_SORT_DEX:
    default:
        return TRUE;
    }
}

static u32 GetStorageSortKey(struct BoxPokemon *boxMon, enum StorageSortKey key, u32 value)
{
    enum Species species = GetBoxMonData(boxMon, MON_DATA_SPECIES);
    u32 group;

    if (species == SPECIES_NONE)
        return SORT_GROUP_EMPTY << 30;

    if (GetBoxMonData(boxMon, MON_DATA_IS_EGG))
        group = SORT_GROUP_EGG;
    else if (MonMatchesSortValue(boxMon, species, key, value))
        group = SORT_GROUP_MATCH;
    else
        group = SORT_GROUP_OTHER;

    // Forms share a National Dex number, so the species ID orders them.
    return (group << 30) | (SpeciesToNationalPokedexNum(species) << 16) | species;
}

bool32 SortStorageBoxes(u32 firstBox, u32 numBoxes, enum StorageSortKey key, u32 value)
{
    u32 i, j, count, start;
    bool32 changed = FALSE;
    struct StorageSortEntry *entries;
    struct BoxPokemon temp;

    if (numBoxes == 0 || firstBox + numBoxes > TOTAL_BOXES_COUNT)
        return FALSE;

    count = numBoxes * IN_BOX_COUNT;
    start = firstBox * IN_BOX_COUNT;
    entries = Alloc(count * sizeof(*entries));
    if (entries == NULL)
        return FALSE;

    for (i = 0; i < count; i++)
    {
        entries[i].key = GetStorageSortKey(GetStorageMonByIndex(start + i), key, value);
        entries[i].index = i;
    }

    // Insertion sort keeps Pokémon with equal keys in their current order.
    for (i = 1; i < count; i++)
    {
        struct StorageSortEntry entry = entries[i];

        for (j = i; j > 0 && entries[j - 1].key > entry.key; j--)
            entries[j] = entries[j - 1];
        entries[j] = entry;
    }

    // Apply the permutation in place, one cycle at a time:
    // slot j receives the Pokémon that was in slot entries[j].index.
    for (i = 0; i < count; i++)
    {
        if (entries[i].index == i)
            continue;

        changed = TRUE;
        temp = *GetStorageMonByIndex(start + i);
        j = i;
        while (TRUE)
        {
            u32 source = entries[j].index;

            entries[j].index = j;
            if (source == i)
            {
                *GetStorageMonByIndex(start + j) = temp;
                break;
            }
            *GetStorageMonByIndex(start + j) = *GetStorageMonByIndex(start + source);
            j = source;
        }
    }

    Free(entries);
    return changed;
}

u32 GetStorageSortCandidateLimit(enum StorageSortKey key)
{
    switch (key)
    {
    case STORAGE_SORT_TYPE:
        return NUMBER_OF_MON_TYPES;
    case STORAGE_SORT_MOVE:
        return MOVES_COUNT;
    case STORAGE_SORT_ABILITY:
        return ABILITIES_COUNT;
    case STORAGE_SORT_DEX:
    default:
        return 0;
    }
}

const u8 *GetStorageSortCandidateName(enum StorageSortKey key, u32 value)
{
    switch (key)
    {
    case STORAGE_SORT_TYPE:
        return gTypesInfo[value].name;
    case STORAGE_SORT_MOVE:
        return GetMoveName(value);
    case STORAGE_SORT_ABILITY:
        return gAbilitiesInfo[value].name;
    case STORAGE_SORT_DEX:
    default:
        return gText_EmptyString2;
    }
}

static void MarkCandidate(u8 *seen, u32 limit, u32 value)
{
    if (value != 0 && value < limit)
        seen[value / 8] |= 1 << (value % 8);
}

// Each candidate is sorted as one number: the collation of its name's first two characters
// above its ID. Most names already differ there, so few comparisons read the whole names.
#define SORT_ID_BITS        14
#define SORT_ID_MASK        ((1 << SORT_ID_BITS) - 1)
#define SORT_PREFIX_CHARS   2
#define SORT_PREFIX_BITS    9 // Collation bases are below 0x130.

STATIC_ASSERT(MOVES_COUNT <= SORT_ID_MASK && ABILITIES_COUNT <= SORT_ID_MASK, StorageSortIdBits)
STATIC_ASSERT(SORT_ID_BITS + SORT_PREFIX_CHARS * SORT_PREFIX_BITS <= 32, StorageSortEntryBits)

static u32 GetKanaCollationBase(u8 c, u32 *variant);
static const u8 *SkipExtCtrlCodes(const u8 *str);

static u32 MakeCandidateSortEntry(enum StorageSortKey key, u32 value)
{
    const u8 *str = GetStorageSortCandidateName(key, value);
    u32 prefix = 0;

    for (u32 i = 0; i < SORT_PREFIX_CHARS; i++)
    {
        u32 variant;

        // Like CompareKanaStrings: an ended name (0) sorts before any character.
        str = SkipExtCtrlCodes(str);
        prefix <<= SORT_PREFIX_BITS;
        if (*str != EOS)
            prefix |= GetKanaCollationBase(*str++, &variant) + 1;
    }
    return (prefix << SORT_ID_BITS) | value;
}

static bool32 SortsBefore(enum StorageSortKey key, u32 entry1, u32 entry2)
{
    s32 order;

    if ((entry1 >> SORT_ID_BITS) != (entry2 >> SORT_ID_BITS))
        return entry1 < entry2;
    order = CompareKanaStrings(GetStorageSortCandidateName(key, entry1 & SORT_ID_MASK),
                               GetStorageSortCandidateName(key, entry2 & SORT_ID_MASK));
    if (order != 0)
        return order < 0;
    return entry1 < entry2; // The same name: ascending ID.
}

// A merge sort: full boxes can hold hundreds of different moves, and sorting their names by
// insertion stalled the menu for seconds. entries holds 2 * count numbers, the second half scratch.
static void SortCandidatesByName(enum StorageSortKey key, u16 *values, u32 *entries, u32 count)
{
    u32 *src = entries, *dst = entries + count, *swap;

    for (u32 i = 0; i < count; i++)
        src[i] = MakeCandidateSortEntry(key, values[i]);

    for (u32 width = 1; width < count; width *= 2)
    {
        for (u32 left = 0; left < count; left += 2 * width)
        {
            u32 mid = min(left + width, count);
            u32 right = min(left + 2 * width, count);
            u32 a = left, b = mid, out = left;

            while (a < mid && b < right)
            {
                if (SortsBefore(key, src[b], src[a]))
                    dst[out++] = src[b++];
                else
                    dst[out++] = src[a++];
            }
            while (a < mid)
                dst[out++] = src[a++];
            while (b < right)
                dst[out++] = src[b++];
        }
        swap = src;
        src = dst;
        dst = swap;
    }

    for (u32 i = 0; i < count; i++)
        values[i] = src[i] & SORT_ID_MASK;
}

u32 GetStorageSortCandidates(u32 firstBox, u32 numBoxes, enum StorageSortKey key, u16 *dst, u32 maxCount)
{
    u32 i, j, count = 0;
    u32 limit = GetStorageSortCandidateLimit(key);
    u8 *seen;

    if (limit == 0 || numBoxes == 0 || firstBox + numBoxes > TOTAL_BOXES_COUNT)
        return 0;

    seen = AllocZeroed((limit + 7) / 8);
    if (seen == NULL)
        return 0;

    for (i = firstBox * IN_BOX_COUNT; i < (firstBox + numBoxes) * IN_BOX_COUNT; i++)
    {
        struct BoxPokemon *boxMon = GetStorageMonByIndex(i);
        enum Species species = GetBoxMonData(boxMon, MON_DATA_SPECIES);

        if (species == SPECIES_NONE || GetBoxMonData(boxMon, MON_DATA_IS_EGG))
            continue;

        switch (key)
        {
        case STORAGE_SORT_TYPE:
            MarkCandidate(seen, limit, GetSpeciesType(species, 0));
            MarkCandidate(seen, limit, GetSpeciesType(species, 1));
            break;
        case STORAGE_SORT_MOVE:
            for (j = 0; j < MAX_MON_MOVES; j++)
                MarkCandidate(seen, limit, GetBoxMonData(boxMon, MON_DATA_MOVE1 + j));
            break;
        case STORAGE_SORT_ABILITY:
            MarkCandidate(seen, limit, GetAbilityBySpecies(species, GetBoxMonData(boxMon, MON_DATA_ABILITY_NUM)));
            break;
        default:
            break;
        }
    }

    for (i = 1; i < limit && count < maxCount; i++)
    {
        if (seen[i / 8] & (1 << (i % 8)))
            dst[count++] = i;
    }
    Free(seen);

    // Types keep their usual order; move and ability names are listed in gojūon order.
    if (key != STORAGE_SORT_TYPE && count > 1)
    {
        u32 *entries = Alloc(2 * count * sizeof(*entries));
        if (entries == NULL)
            return 0;
        SortCandidatesByName(key, dst, entries, count);
        Free(entries);
    }

    return count;
}

// Japanese charmap layout: hiragana occupy 0x01-0x50 and katakana are the same
// characters offset by 0x50.
#define KANA_A          0x01
#define KANA_N          0x2E
#define KANA_SMALL_A    0x2F
#define KANA_SMALL_YA   0x34
#define KANA_GA         0x37
#define KANA_BA         0x46
#define KANA_PA         0x4B
#define KANA_SMALL_TSU  0x50
#define KANA_KA         0x06
#define KANA_TSU        0x12
#define KANA_HA         0x1A
#define KANA_YA         0x24
#define KANA_U          0x03
#define KATAKANA_OFFSET 0x50
#define KATAKANA_VU     0xF1

enum
{
    KANA_PLAIN,
    KANA_SMALL,
    KANA_VOICED,
    KANA_SEMI_VOICED,
};

// Kana sort after every other character; *variant breaks ties between kana that share a base.
static u32 GetKanaCollationBase(u8 c, u32 *variant)
{
    *variant = KANA_PLAIN;
    if (c == KATAKANA_VU)
    {
        *variant = KANA_VOICED;
        return 0x100 + KANA_U;
    }
    if (c > KANA_SMALL_TSU && c <= KANA_SMALL_TSU + KATAKANA_OFFSET)
        c -= KATAKANA_OFFSET;

    if (c >= KANA_A && c <= KANA_N)
        return 0x100 + c;
    if (c >= KANA_SMALL_A && c < KANA_SMALL_YA)
    {
        *variant = KANA_SMALL;
        return 0x100 + KANA_A + (c - KANA_SMALL_A);
    }
    if (c >= KANA_SMALL_YA && c < KANA_GA)
    {
        *variant = KANA_SMALL;
        return 0x100 + KANA_YA + (c - KANA_SMALL_YA);
    }
    if (c >= KANA_GA && c < KANA_BA)
    {
        *variant = KANA_VOICED;
        return 0x100 + KANA_KA + (c - KANA_GA);
    }
    if (c >= KANA_BA && c < KANA_PA)
    {
        *variant = KANA_VOICED;
        return 0x100 + KANA_HA + (c - KANA_BA);
    }
    if (c >= KANA_PA && c < KANA_SMALL_TSU)
    {
        *variant = KANA_SEMI_VOICED;
        return 0x100 + KANA_HA + (c - KANA_PA);
    }
    if (c == KANA_SMALL_TSU)
    {
        *variant = KANA_SMALL;
        return 0x100 + KANA_TSU;
    }
    return c;
}

static const u8 *SkipExtCtrlCodes(const u8 *str)
{
    while (*str == EXT_CTRL_CODE_BEGIN)
        str += 1 + GetExtCtrlCodeLength(str[1]);
    return str;
}

s32 CompareKanaStrings(const u8 *str1, const u8 *str2)
{
    s32 tieBreak = 0;

    while (TRUE)
    {
        u32 base1, base2, variant1, variant2;

        str1 = SkipExtCtrlCodes(str1);
        str2 = SkipExtCtrlCodes(str2);
        if (*str1 == EOS || *str2 == EOS)
            break;

        base1 = GetKanaCollationBase(*str1, &variant1);
        base2 = GetKanaCollationBase(*str2, &variant2);
        if (base1 != base2)
            return base1 < base2 ? -1 : 1;
        if (tieBreak == 0 && variant1 != variant2)
            tieBreak = variant1 < variant2 ? -1 : 1;
        str1++;
        str2++;
    }

    if (*str1 != *str2)
        return *str1 == EOS ? -1 : 1;
    return tieBreak;
}
