#ifndef GUARD_ULTRA_HELP_H
#define GUARD_ULTRA_HELP_H

#include "main.h"
#include "constants/info_viewer.h"

void Special_OpenInfoViewer(void);
void ultra_help(void);
void StartInfoViewer(u16 infoId, MainCallback exitCallback);
void StartUltraHelp(u16 topic, MainCallback exitCallback);

#endif // GUARD_ULTRA_HELP_H
