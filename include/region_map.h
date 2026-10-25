#ifndef GUARD_REGION_MAP_H
#define GUARD_REGION_MAP_H

// Only the map section names remain. The region map screens (Town Map, PokeNav
// map, Fly map and Pokedex area page) were removed with the Hoenn mainland.

#define MAP_NAME_LENGTH 16

struct RegionMapLocation
{
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    const u8 *name;
};

u8 *GetMapName(u8 *dest, mapsec_u16_t regionMapId, u16 padLength);
u8 *GetMapNameGeneric(u8 *dest, mapsec_u16_t mapSecId);
u8 *GetMapNameHandleAquaHideout(u8 *dest, mapsec_u16_t mapSecId);

extern const struct RegionMapLocation gRegionMapEntries[];

#endif // GUARD_REGION_MAP_H
