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
enum PlayerOrientation { UP, DOWN, LEFT, RIGHT };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct PlayerData {
    // Posición y orientación
    int x_tile, y_tile;      /* coordenadas en tiles */
    Orientation orientation; /* orientación del personaje */

    // Stats y atributos
    uint16_t health, mana; /* stats */

    // Cuerpo y vestimenta
    Id head_id, body_id;                           /* cuerpo básico */
    Id helmet_id, armour_id, shield_id, weapon_id; /* vestimenta */
};

struct CharacterData {
    // Posición y orientación
    int x_tile, y_tile;      /* coordenadas en tiles */
    Orientation orientation; /* orientación del personaje */

    // Cuerpo y vestimenta
    Id head_id, body_id;                           /* cuerpo básico */
    Id helmet_id, armour_id, shield_id, weapon_id; /* vestimenta */
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_DATA_H__