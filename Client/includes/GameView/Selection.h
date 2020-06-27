#ifndef __SELECTION_H__
#define __SELECTION_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Inventory.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct Selection {
    bool npc_selected;
    uint32_t npc_x_tile;
    uint32_t npc_y_tile;
    int8_t inventory_slot_selected;  // valid if >= 0
};

//-----------------------------------------------------------------------------

#endif  // __SELECTION_H__