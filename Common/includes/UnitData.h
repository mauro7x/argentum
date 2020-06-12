#ifndef __UNIT_DATA_H__
#define __UNIT_DATA_H__

//-----------------------------------------------------------------------------
#include <cstdint>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef __ID__
#define __ID__
typedef int Id;
#endif  // __ID__
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
typedef uint8_t Orientation;
enum Orientations { UP, DOWN, LEFT, RIGHT };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ESTRUCTURA BASE

/* Información básica necesaria para cualquier unidad */
struct UnitData {
    int gid;                 /* unit id (ingame)*/
    int x_tile, y_tile;      /* coordenadas en tiles */
    Orientation orientation; /* orientación de la unidad */
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ESTRUCTURAS PARTICULARES

/* Estructura particular para el player principal */
struct PlayerData {
    // Data común a todas las unidades
    UnitData basic_data;

    // Stats y atributos
    uint32_t health, mana, gold; /* stats */

    // ver tema de como sabemos si es fantasma (el server cambia los ids de la
    // ropa?)

    // Cuerpo y vestimenta
    Id head_id, body_id;                           /* cuerpo básico */
    Id helmet_id, armour_id, shield_id, weapon_id; /* vestimenta */
};

/* Estructura particular para los players controlados por terceros */
struct CharacterData {
    /* Data común a las unidades */
    UnitData basic_data;

    // ver tema de como sabemos si es fantasma (el server cambia los ids de la
    // ropa?)

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