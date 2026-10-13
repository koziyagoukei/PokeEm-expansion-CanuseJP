#ifndef GUARD_VILLA_SHOP_H
#define GUARD_VILLA_SHOP_H

#include "constants/villa_shop.h"

u16 VillaShop_GetBallPrice(u16 ball);
u16 VillaShop_BuyDecoration(u16 decoration);
u16 VillaShop_ChangeBall(u16 partySlot, u16 ball);
void VillaShop_OpenCatalog(void);
void VillaShop_Purchase(void);

#endif
