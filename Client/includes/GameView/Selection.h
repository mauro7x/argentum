#ifndef __SELECTION_H__
#define __SELECTION_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Inventory.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct Selection {
    // NPC
    bool npc_selected;
    uint32_t npc_x_tile;
    uint32_t npc_y_tile;

    // Portal
    bool portal_selected;
    uint32_t portal_x_tile;
    uint32_t portal_y_tile;

    // Inventory
    int8_t inventory_slot_selected;  // valid if >= 0

    Selection()
        : npc_selected(false),
          npc_x_tile(0),
          npc_y_tile(0),
          portal_selected(false),
          portal_x_tile(0),
          portal_y_tile(0),
          inventory_slot_selected(-1) {}
};

//-----------------------------------------------------------------------------

#endif  // __SELECTION_H__