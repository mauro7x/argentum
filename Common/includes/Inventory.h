#ifndef __INVENTORY_TYPES_H__
#define __INVENTORY_TYPES_H__

//-----------------------------------------------------------------------------
#include <array>

#include "types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define N_INVENTORY_SLOTS 18
#define N_BANK_ACCOUNT_SLOTS 30
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
enum WearableType { WEAPON, HELMET, ARMOUR, SHIELD, N_WEARABLE_ITEMS };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct InventorySlot {
    Id item;
    uint32_t amount;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
typedef std::array<InventorySlot, N_INVENTORY_SLOTS> InventoryData;
typedef std::array<Id, N_WEARABLE_ITEMS> EquipmentData;
typedef std::array<InventorySlot, N_BANK_ACCOUNT_SLOTS> BankAccountData;
//-----------------------------------------------------------------------------

#endif  // __INVENTORY_TYPES_H__