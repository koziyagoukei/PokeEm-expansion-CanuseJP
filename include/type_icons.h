#ifndef GUARD_TYPE_ICONS_H
#define GUARD_TYPE_ICONS_H

void LoadTypeIcons(enum BattlerId battler);

#define TYPE_ICON_TAG 0x2720
#define TYPE_ICON_TAG_2 0x2721
#define TYPE_ICON_TAG_3 0x2723
#define tTargetBattler    data[0]
#define tBattlerId        data[1]
#define tTypeCount        data[3]
#define tTypeNum          data[4]

#define TYPE_ICON_1_FRAME(monType) ((monType - 1) * 2)
#define TYPE_ICON_2_FRAME(monType) ((monType - 11) * 2)

#endif // GUARD_TYPE_ICONS_H
