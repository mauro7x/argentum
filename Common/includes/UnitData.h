#ifndef __UNIT_DATA_H__
#define __UNIT_DATA_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "Inventory.h"
#include "Orientation.h"
#include "types.h"
//-----------------------------------------------------------------------------

/* Información básica necesaria para cualquier unidad */
struct UnitData {
    InstanceId gid; /* unit id (ingame)*/

    Id map;
    int x_tile, y_tile;      /* coordenadas en tiles */
    Orientation orientation; /* orientación de la unidad */
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct CharacterData {
    UnitData basic_data;
    std::string nickname;
    Id head_id, body_id;
    EquipmentData equipment;
};

struct PlayerData : public CharacterData {
    uint32_t health, max_health;
    uint32_t mana, max_mana;
    uint32_t safe_gold, excess_gold;
    uint32_t level, exp, levelup_exp;

    InventoryData inventory;
};

struct CreatureData {
    UnitData basic_data;
    Id creature_id;
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_DATA_H__