#ifndef __SELECTION_H__
#define __SELECTION_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Inventory.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct Selection {
    Id npc_selected;                 // valid if > 0
    int8_t inventory_slot_selected;  // valid if >= 0
};

//-----------------------------------------------------------------------------

#endif  // __SELECTION_H__