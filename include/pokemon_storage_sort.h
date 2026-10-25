#ifndef GUARD_POKEMON_STORAGE_SORT_H
#define GUARD_POKEMON_STORAGE_SORT_H

enum StorageSortKey
{
    STORAGE_SORT_DEX,
    STORAGE_SORT_TYPE,
    STORAGE_SORT_MOVE,
    STORAGE_SORT_ABILITY,
};

// Sorts the Pokémon in boxes [firstBox, firstBox + numBoxes) in place.
// Pokémon matching value (a type, move or ability, depending on key) come first,
// then the other Pokémon, then Eggs, each group in National Dex order.
// Empty slots move to the end. Returns FALSE if nothing was changed.
bool32 SortStorageBoxes(u32 firstBox, u32 numBoxes, enum StorageSortKey key, u32 value);

// Writes the distinct types, moves or abilities of the non-Egg Pokémon in the
// boxes to dst, in the order they are offered to the player. Returns the count.
u32 GetStorageSortCandidates(u32 firstBox, u32 numBoxes, enum StorageSortKey key, u16 *dst, u32 maxCount);
u32 GetStorageSortCandidateLimit(enum StorageSortKey key);
const u8 *GetStorageSortCandidateName(enum StorageSortKey key, u32 value);

// Orders Japanese names by gojūon: katakana sorts with hiragana, and voiced or
// small kana sort with their base kana, only breaking ties.
s32 CompareKanaStrings(const u8 *str1, const u8 *str2);

#endif // GUARD_POKEMON_STORAGE_SORT_H
