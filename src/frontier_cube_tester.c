// Frontier content: GBA-GC Extension public test build.
//
// The release number pairs this ROM with the GameCube companion.dol of the
// same release (gba-gc-extension docs/releases.md). The connector refuses a
// host of another release and shows both numbers. 1 = v0.1.0-alpha.
#include "global.h"
#include "gc_extension.h"

u16 GcExtension_GetRelease(void)
{
    return 1;
}
