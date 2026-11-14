#ifndef GUARD_GC_COMPANION_H
#define GUARD_GC_COMPANION_H

// GC Extension Mode: connects to a GameCube host over JoyBus (Dolphin +
// Integrated GBA), lets the host read the party and its sprites, and uses
// the GBA as controller and second screen. Entered only from a test script.

enum GcCompanionState
{
    GC_STATE_IDLE,
    GC_STATE_CONNECTING,
    GC_STATE_HANDSHAKE,
    GC_STATE_SEND_PARTY,
    GC_STATE_SEND_ASSETS,
    GC_STATE_ACTIVE,
    GC_STATE_DISCONNECTING,
    GC_STATE_RETURN_FIELD,
    GC_STATE_ERROR,
};

// Script special (waitstate). Returns to the field and continues the script.
void GcCompanion_Start(void); // party viewer; see gc_extension.h for GcStartMode
void GcLinkCheck_Start(void); // connection and version check

#endif // GUARD_GC_COMPANION_H
