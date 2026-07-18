#ifndef GUARD_CONFIG_LINK_COMPAT_H
#define GUARD_CONFIG_LINK_COMPAT_H

// Fixed identifier for this project family. "FR" in ASCII.
#define LINK_PROJECT_ID          0x4652

// Bump for synchronized battle/protocol changes.
#define LINK_COMPAT_VERSION      2

// Bump for transmitted Pokemon, battle, or contest data layout changes.
#define LINK_DATA_LAYOUT_VERSION 2

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

// LinkPlayer has one legacy unused byte available. Keep both versions in it
// so cable and RFU player exchange paths carry the same compatibility data.
#define LINK_COMPAT_VERSION_BITS        4
#define LINK_COMPAT_VERSION_MASK        ((1 << LINK_COMPAT_VERSION_BITS) - 1)
#define LINK_COMPAT_VERSION_SHIFT       LINK_COMPAT_VERSION_BITS
#define LINK_PACKED_VERSIONS            ((LINK_COMPAT_VERSION << LINK_COMPAT_VERSION_SHIFT) | LINK_DATA_LAYOUT_VERSION)
#define LINK_UNPACK_COMPAT_VERSION(v)   (((v) >> LINK_COMPAT_VERSION_SHIFT) & LINK_COMPAT_VERSION_MASK)
#define LINK_UNPACK_DATA_VERSION(v)     ((v) & LINK_COMPAT_VERSION_MASK)

#if LINK_PROJECT_ID > 0xFFFF
#error LINK_PROJECT_ID must fit in LinkPlayer.lp_field_2
#endif

#if LINK_COMPAT_VERSION > LINK_COMPAT_VERSION_MASK
#error LINK_COMPAT_VERSION exceeds the packed LinkPlayer field
#endif

#if LINK_DATA_LAYOUT_VERSION > LINK_COMPAT_VERSION_MASK
#error LINK_DATA_LAYOUT_VERSION exceeds the packed LinkPlayer field
#endif

#if LINK_PACKED_VERSIONS > 0xFF
#error Packed link compatibility versions must fit in LinkPlayer.neverRead
#endif

#endif // GUARD_CONFIG_LINK_COMPAT_H
