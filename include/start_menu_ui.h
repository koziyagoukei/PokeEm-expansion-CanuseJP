#ifndef GUARD_START_MENU_UI_H
#define GUARD_START_MENU_UI_H

#define START_MENU_GRID_COLUMNS 5
#define START_MENU_GRID_SIZE 10

enum StartMenuIcon
{
    START_ICON_TIPS,
    START_ICON_POKEDEX,
    START_ICON_POKEMON,
    START_ICON_BAG,
    START_ICON_POKENAV,
    START_ICON_CARD,
    START_ICON_SAVE,
    START_ICON_OPTION,
    START_ICON_EXIT,
    START_ICON_RETIRE,
    START_ICON_REST,
    START_ICON_DEXNAV,
};

struct StartMenuUiEntry
{
    const u8 *label;
    const u8 *description;
    u8 icon;
};

bool32 StartMenuUi_Open(void);
void StartMenuUi_Draw(const struct StartMenuUiEntry *entries, u32 count, u32 selected, const u8 *status);
void StartMenuUi_Close(void);

#endif
