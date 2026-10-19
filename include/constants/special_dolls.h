#ifndef GUARD_CONSTANTS_SPECIAL_DOLLS_H
#define GUARD_CONSTANTS_SPECIAL_DOLLS_H

enum
{
    SPECIAL_DOLL_NONE,
#define SPECIAL_DOLL(id, symbol, name, path, size) SPECIAL_DOLL_##id,
#include "constants/special_doll_list.h"
#undef SPECIAL_DOLL
    SPECIAL_DOLL_COUNT,
};

#define SPECIAL_DOLL_CAPACITY 64
#define SPECIAL_DOLL_PLACEMENT_COUNT 3
#define SPECIAL_DOLL_SAVE_MAGIC 0x53444C31
#define SPECIAL_DOLL_SAVE_VERSION 1

// Fixed ranges keep existing object graphics IDs and dynamic graphics vars stable.
#define SPECIAL_DOLL_GFX_FIRST 0x0800
#define SPECIAL_DOLL_PAL_FIRST 0x1200
#define SPECIAL_DOLL_FIRST_PALETTE_SLOT 8

#endif
