// Frontier content: GBA-GC Extension public test build.
//
// The release number pairs this ROM with the GameCube companion.dol of the
// same release (gba-gc-extension docs/releases.md). The connector refuses a
// host of another release and shows both numbers. 1 = v0.1.0-alpha (public),
// 2 = m3-dev (Milestone 3 move effects), 3 = m3.5-dev (Milestone 3.5 volatile
// status and multi-turn actions), 4 = m3.5-audit (audit fixes, request V10),
// 5 = m4a-dev (Milestone 4-A abilities, request V11); 2 to 5 are development
// builds only.
#include "global.h"
#include "gc_extension.h"

u16 GcExtension_GetRelease(void)
{
    return 5;
}
