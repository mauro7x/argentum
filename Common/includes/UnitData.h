#ifndef __UNIT_DATA_H__
#define __UNIT_DATA_H__
//-----------------------------------------------------------------------------
#include <array>
//-----------------------------------------------------------------------------
#include "types.h"
#include "game_config.h"
#include "Orientation.h"
//-----------------------------------------------------------------------------

#ifndef N_WEARABLE_ITEMS
#define N_WEARABLE_ITEMS 4
#endif // N_WEARABLE_ITEMS

//-----------------------------------------------------------------------------
// ESTRUCTURAS AUXILIARES
//-----------------------------------------------------------------------------

struct InventorySlot {
    Id item;
    uint32_t amount;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ESTRUCTURA BASE
//-----------------------------------------------------------------------------

/* Información básica necesaria para cualquier unidad */
struct UnitData {
    InstanceId gid;          /* unit id (ingame)*/

    int x_tile, y_tile;      /* coordenadas en tiles */
    Orientation orientation; /* orientación de la unidad */
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ESTRUCTURAS PARTICULARES
//-----------------------------------------------------------------------------
/* Estructura particular para el player principal */
struct PlayerData {
    // Data común a todas las unidades
    UnitData basic_data;

    // Stats y atributos
    uint32_t health, mana, gold; /* stats */

    std::array<InventorySlot, N_INVENTORY_SLOTS> inventory;
    std::array<Id, N_WEARABLE_ITEMS> equipment;

    // Cuerpo y vestimenta
    Id head_id, body_id;                           /* cuerpo básico */
    Id helmet_id, armour_id, shield_id, weapon_id; /* vestimenta */
};

/* Estructura particular para los players controlados por terceros */
struct CharacterData {
    /* Data común a las unidades */
    UnitData basic_data;

    // Cuerpo y vestimenta
    Id head_id, body_id;                           /* cuerpo básico */
    Id helmet_id, armour_id, shield_id, weapon_id; /* vestimenta */
};

/* Estructura particular para los monstruos */
struct CreatureData {
    /* Data común a las unidades */
    UnitData basic_data;

    // Cuerpo
    Id creature_id; /* creature body */
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_DATA_H__