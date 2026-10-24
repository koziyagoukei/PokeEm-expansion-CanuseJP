#ifndef GUARD_ULTRA_HELP_H
#define GUARD_ULTRA_HELP_H

#include "main.h"
#include "constants/info_viewer.h"

struct InfoViewerStyle
{
    u8 windowFill;
    u8 framePaletteNum;
    const u16 *framePalette;
    u8 textColors[3];
};

struct InfoViewerChoice
{
    const u8 *text;
    u16 nextInfoId;
};

struct InfoViewerEntry
{
    const u8 *title;
    const u8 *const *pages;
    u8 pageCount;
    const struct InfoViewerChoice *choices;
    u8 choiceCount;
    const struct InfoViewerStyle *style;
};

const struct InfoViewerEntry *GetInfoViewerEntry(u16 infoId);
u32 GetInfoViewerPageCount(u16 infoId);

void Special_OpenInfoViewer(void);
void ultra_help(void);
void StartInfoViewer(u16 infoId, MainCallback exitCallback);
void StartUltraHelp(u16 topic, MainCallback exitCallback);

#endif // GUARD_ULTRA_HELP_H
