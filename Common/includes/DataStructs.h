#ifndef __DATA_STRUCTS_H__
#define __DATA_STRUCTS_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Inventory.h"
#include "Orientation.h"
#include "types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Información básica necesaria para cualquier unidad movible */
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
    uint32_t level;
};

struct PlayerData : public CharacterData {
    uint32_t health, max_health;
    uint32_t mana, max_mana;
    uint32_t safe_gold, excess_gold;
    uint32_t exp, levelup_exp;

    InventoryData inventory;
};

struct CreatureData {
    UnitData basic_data;
    std::string name;
    Id creature_id;
    uint32_t health, max_health /*, level*/;
};

struct ItemData {
    Id item_id;
    int x_tile, y_tile;
};

//-----------------------------------------------------------------------------

#endif  // __DATA_STRUCTS_H__
