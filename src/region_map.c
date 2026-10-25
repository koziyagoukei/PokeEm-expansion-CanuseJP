#include "global.h"
#include "region_map.h"
#include "secret_base.h"
#include "string_util.h"
#include "strings.h"
#include "constants/characters.h"
#include "constants/region_map_sections.h"

/*
 *  This file only provides map section names. The region map screens (Town Map,
 *  PokeNav map, Fly map and Pokedex area page) were removed with the Hoenn mainland.
 */

#include "data/region_map/region_map_entries.h"

u8 *GetMapName(u8 *dest, mapsec_u16_t regionMapId, u16 padLength)
{
    u8 *str;
    u16 i;

    if (regionMapId == MAPSEC_SECRET_BASE)
    {
        str = GetSecretBaseMapName(dest);
    }
    else if (regionMapId < MAPSEC_NONE)
    {
        str = StringCopy(dest, gRegionMapEntries[regionMapId].name);
    }
    else
    {
        if (padLength == 0)
        {
            padLength = 18;
        }
        return StringFill(dest, CHAR_SPACE, padLength);
    }
    if (padLength != 0)
    {
        for (i = str - dest; i < padLength; i++)
        {
            *str++ = CHAR_SPACE;
        }
        *str = EOS;
    }
    return str;
}

u8 *GetMapNameGeneric(u8 *dest, mapsec_u16_t mapSecId)
{
    switch (mapSecId)
    {
    case MAPSEC_DYNAMIC:
        return StringCopy(dest, gText_Ferry);
    case MAPSEC_SECRET_BASE:
        return StringCopy(dest, gText_SecretBase);
    default:
        return GetMapName(dest, mapSecId, 0);
    }
}

u8 *GetMapNameHandleAquaHideout(u8 *dest, mapsec_u16_t mapSecId)
{
    if (mapSecId == MAPSEC_AQUA_HIDEOUT_OLD)
        return StringCopy(dest, gText_Hideout);
    else
        return GetMapNameGeneric(dest, mapSecId);
}
