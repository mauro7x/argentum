#ifndef __UNIT_H__
#define __UNIT_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/DataStructs.h"
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/defs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../defs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Renderer.h"
#include "Sprites.h"
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

    /* Data del personaje */
    UnitData data = {0};
    UnitState state = NOT_INIT;

    /* Componentes para el renderizado gráfico */
    float x = 0, y = 0;              /* posición en pixeles */
    float x_vel = 0, y_vel = 0;      /* velocidades en cada componente*/
    Uint32 last_moved = 0;           /* ultimo movimiento */
    int current_animation_frame = 0; /* frame actual a renderizar */

    /* Settea la velocidad de movimiento en caso de ser necesario */
    void _setMovementSpeed();

    /* Verifica si el movimiento ya terminó (si nos pasamos) */
    void _movementFinished();

    /* Calcula la posición x del clip a renderizar */
    int _calculateSpriteX(const UnitSprite& sprite) const;

    /* Calcula la posición y del clip a renderizar */
    int _calculateSpriteY(const UnitSprite& sprite) const;

    /* Renderiza un sprite agregando el offset necesario */
    void _render(const UnitSprite& sprite) const;

   public:
    /* Constructor por defecto para poder almacenarlas en maps */
    Unit();

    /* Constructor */
    Unit(Renderer* renderer, UnitSpriteContainer* sprites);

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
    virtual void act(const int it);

    /* Renderizarse si se encuentra dentro de la cámara */
    virtual void render() const = 0;

    //-------------------------------------------------------------------------

    /* Obtiene la posición en tiles */
    SDL_Point getPos() const;

    /* Destructor */
    virtual ~Unit();
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_H__
