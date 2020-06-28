#ifndef __TILE_H__
#define __TILE_H__

//-----------------------------------------------------------------------------
#include "types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct Tile {
    /* Graficos */
    TileId ground_1_id;
    TileId ground_2_id;
    TileId decoration_id;
    TileId roof_id;
    TileId npc_id;

    /* Metadata */
    bool collision;
    bool safe_zone;
    bool indoor;
    bool portal;
    Id npc;

    /* Ocupantes */
    InstanceId occupant_id;
    Id item_id;
    uint32_t item_amount;
};

//-----------------------------------------------------------------------------
#endif  // __TILE_H__