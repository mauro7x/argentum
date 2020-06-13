#ifndef __UNIT_H__
#define __UNIT_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/UnitData.h"
#include "Renderer.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
enum UnitState { NOT_INIT, READY, MOVING };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/** UNIT
 * El objetivo de esta clase es el de encapsular el comportamiento de
 * "movimiento" smooth y del renderizado de los sprites que lo componen.
 *
 * Las clases derivadas deberán inicializar de forma válida la unidad para poder
 * utilizarla, llenando sus distintos campos.
 */

class Unit {
   protected:
    /* Componentes para renderizar */
    Renderer* g_renderer;
    UnitSpriteContainer* g_sprites;
    int tile_w, tile_h;

    /* Data del personaje */
    UnitData data;
    UnitState state;

    /* Componentes para el renderizado gráfico */
    float x, y;               /* posición en pixeles */
    float tile_movement_time; /* tiempo necesario para mover un tile */
    float x_vel, y_vel;       /* velocidades en cada componente*/
    Uint32 last_moved;        /* ultimo movimiento */
    int last_tick;            /* ultimo tick recibido */

    /* Settea la velocidad de movimiento en caso de ser necesario */
    void _setMovementSpeed();

    /* Verifica si el movimiento ya terminó (si nos pasamos) */
    void _movementFinished();

    /* Calcula la posición x del clip a renderizar */
    int _calculateSpriteX(const Sprite& sprite) const;

    /* Calcula la posición y del clip a renderizar */
    int _calculateSpriteY(const Sprite& sprite) const;

    /* Renderiza un sprite agregando el offset necesario */
    void _render(const Sprite& sprite) const;

   public:
    /* Constructor por defecto para poder almacenarlas en maps */
    Unit();

    /* Constructor */
    Unit(Renderer* renderer, UnitSpriteContainer* sprites, const int tile_w = 0,
         const int tile_h = 0, const int tile_movement_time = 0);

    /* Deshabilitamos el constructor por copia. */
    Unit(const Unit&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Unit& operator=(const Unit&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Unit(Unit&& other);

    /* Habilitamos el operador= para movimiento. */
    Unit& operator=(Unit&& other);

    //-------------------------------------------------------------------------

    /* Acción que realiza en cada frame */
    virtual void act(const int tick);

    /* Renderizarse si se encuentra dentro de la cámara */
    virtual void render() const = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Unit();
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_H__
