#ifndef GUARD_ULTRA_HELP_H
#define GUARD_ULTRA_HELP_H

#include "main.h"

enum UltraHelpTopicId
{
    ULTRA_HELP_TOPIC_FRONTIER_RULES,
    ULTRA_HELP_TOPIC_COUNT
};

void ultra_help(void);
void StartUltraHelp(u16 topic, MainCallback exitCallback);

#endif // GUARD_ULTRA_HELP_H
