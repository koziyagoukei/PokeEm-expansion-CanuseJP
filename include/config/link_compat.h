#ifndef GUARD_CONFIG_LINK_COMPAT_H
#define GUARD_CONFIG_LINK_COMPAT_H

// Fixed identifier for this project family. "FR" in ASCII.
#define LINK_PROJECT_ID          0x4652

// Bump for synchronized battle/protocol changes.
#define LINK_COMPAT_VERSION      16

// Bump for transmitted Pokemon, battle, or contest data layout changes.
#define LINK_DATA_LAYOUT_VERSION 8

#define LINK_VARIANT_FRONTIER_HACK 1
#define LINK_BUILD_VARIANT         LINK_VARIANT_FRONTIER_HACK

// These values describe data that is sent directly over the link. The
// corresponding STATIC_ASSERTs make layout changes fail at compile time until
// LINK_DATA_LAYOUT_VERSION and these values are deliberately updated.
#define LINK_SIZEOF_BOX_POKEMON      80
#define LINK_SIZEOF_POKEMON         100
#define LINK_SIZEOF_BATTLE_POKEMON  140
#define LINK_SIZEOF_CONTEST_POKEMON  64
#define LINK_SIZEOF_LINK_PLAYER      28
#define LINK_SIZEOF_LINK_PLAYER_BLOCK 60

// The old four-bit protocol number reached 15. Use bits 8-13 of
// LinkPlayer.version; the low byte stays the game version and bit 14 stays set.
// Keep only the layout in neverRead so old readers reject protocol zero.
#define LINK_COMPAT_VERSION_BITS        6
#define LINK_COMPAT_VERSION_MASK        ((1 << LINK_COMPAT_VERSION_BITS) - 1)
#define LINK_COMPAT_VERSION_SHIFT       8
#define LINK_PACKED_COMPAT_VERSION      (LINK_COMPAT_VERSION << LINK_COMPAT_VERSION_SHIFT)
#define LINK_UNPACK_COMPAT_VERSION(v)   (((v) >> LINK_COMPAT_VERSION_SHIFT) & LINK_COMPAT_VERSION_MASK)

#if LINK_PROJECT_ID > 0xFFFF
#error LINK_PROJECT_ID must fit in LinkPlayer.lp_field_2
#endif

#if LINK_COMPAT_VERSION < 16 || LINK_COMPAT_VERSION > LINK_COMPAT_VERSION_MASK
#error LINK_COMPAT_VERSION must fit the extended LinkPlayer.version encoding
#endif

#if LINK_DATA_LAYOUT_VERSION < 1 || LINK_DATA_LAYOUT_VERSION > 0x0F
#error LINK_DATA_LAYOUT_VERSION must leave the legacy protocol nibble zero
#endif

#endif // GUARD_CONFIG_LINK_COMPAT_H
