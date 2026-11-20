// Frontier content: GBA-GC Extension public test build.
//
// The release number pairs this ROM with the GameCube companion.dol of the
// same release (gba-gc-extension docs/releases.md). The connector refuses a
// host of another release and shows both numbers. 1 = v0.1.0-alpha (public),
// 2 = m3-dev (Milestone 3 move effects), 3 = m3.5-dev (Milestone 3.5 volatile
// status and multi-turn actions), 4 = m3.5-audit (audit fixes, request V10),
// 5 = m4a-dev (Milestone 4-A abilities, request V11), 6 = m4a2-dev
// (Milestone 4-A2 multi-hook abilities, request V12), 7 = rule-poc (V13
// modifier records PoC), 8 = m4p-dev (M4-P shared modifiers, request V14 fixed
// point), 9 = m4w-dev (M4-W weather, request V15), 10 = m4t-dev (M4-T
// terrain, request V16); 2 to 10 are development builds only.
#include "global.h"
#include "gc_extension.h"

u16 GcExtension_GetRelease(void)
{
    return 10;
}
