#ifndef __UNIT_DATA_H__
#define __UNIT_DATA_H__

//-----------------------------------------------------------------------------
#include "Inventory.h"
#include "Orientation.h"
#include "types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Información básica necesaria para cualquier unidad */
struct UnitData {
    InstanceId gid; /* unit id (ingame)*/

    int x_tile, y_tile;      /* coordenadas en tiles */
    Orientation orientation; /* orientación de la unidad */
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct CharacterData {
    UnitData basic_data;
    Id head_id, body_id;
    Id helmet_id, armour_id, shield_id, weapon_id;
};

struct PlayerData : public CharacterData {
    uint32_t health, max_health;
    uint32_t mana, max_mana;
    uint32_t safe_gold, excess_gold;
    uint32_t level, exp, levelup_exp;

    Inventory inventory;
    Equipment equipment;
};

struct CreatureData {
    UnitData basic_data;
    Id creature_id;
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_DATA_H__