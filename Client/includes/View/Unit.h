#ifndef __UNIT_H__
#define __UNIT_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "Renderer.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Ver donde poner esto...

struct UnitData {
    int x, y;
    Id head_id, body_id;
    Id helmet_id, armour_id, shield_id, weapon_id;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Unit {
   protected:
    const Renderer* g_renderer;
    const UnitSpriteContainer& g_sprites;
    int tile_w, tile_h; /* dimensiones de cada tile */
    int x_tile, y_tile; /* posición en la grilla en Tiles */
    SDL_Rect box;       /* posición y dimensión en pixeles */
    int x_vel, y_vel;   /* velocidad de movimiento actual */
    int next_x, next_y; /* valores en px a los que debemos llegar */
    Uint32 last_moved;  /* para trackear el tiempo entre movimientos */

    /* Centra la box de la entidad en el tile */
    void _centerBoxOnTile();

    /* Calcula valor de X al que hay que llegar para completar el movimiento */
    int _xValueToReach() const;

    /* Calcula valor de Y al que hay que llegar para completar el movimiento */
    int _yValueToReach() const;

   public:
    /* Constructor */ /* Vamos a recibir un struct o algo para crearnos bien */
    Unit(const Renderer* renderer, const UnitSpriteContainer& sprites);

    /* Deshabilitamos el constructor por copia. */
    Unit(const Unit&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Unit& operator=(const Unit&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Unit(Unit&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Unit& operator=(Unit&& other) = delete;

    //-------------------------------------------------------------------------
    // Lo que debe poder hacer una entidad:

    /* Inicializarse */
    virtual void init(const int tile_w, const int tile_h);

    /* Actualizar información según lo que diga el servidor */
    virtual void update(int corrected_x_tile, int corrected_y_tile);

    /* Actuar en el frame si quiere hacerlo */
    virtual void act();

    /* Renderizarse si se encuentra dentro de la cámara */
    virtual void render() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Unit();
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_H__
