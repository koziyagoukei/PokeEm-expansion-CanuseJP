#ifndef GUARD_GC_COMPANION_LINK_H
#define GUARD_GC_COMPANION_LINK_H

// JoyBus transport and reliable-frame responder for GC Extension Mode.
// The GameCube is the JoyBus master; the GBA only answers. Nothing here is
// specific to Pokemon: requests are handed to a GcFrameHandler.

#include "gc_ext/wire.h"

typedef bool (*GcFrameHandler)(const Frame *request, Frame *reply);

// Event bits returned by GcLink_Poll.
#define GC_LINK_EVENT_CONNECTED (1 << 0) // raw JoyBus handshake completed
#define GC_LINK_EVENT_RESET     (1 << 1) // GameCube sent a JoyBus RESET (session restarts)
#define GC_LINK_EVENT_TIMEOUT   (1 << 2) // connected, but the GameCube went silent
#define GC_LINK_EVENT_FRAME     (1 << 3) // at least one new request was accepted

struct GcLinkStats
{
    u32 resets;
    u32 overflows;
    u32 crcErrors;
    u32 duplicates;
    u32 nacks;
    u32 requests;
};

bool8 GcLink_Open(void);
void GcLink_Close(void);
u32 GcLink_Poll(GcFrameHandler handler);
bool8 GcLink_IsConnected(void);
bool8 GcLink_HasPendingOutput(void); // replies not yet read by the GameCube
const struct GcLinkStats *GcLink_GetStats(void);

#endif // GUARD_GC_COMPANION_LINK_H
