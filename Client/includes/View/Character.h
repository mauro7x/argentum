#ifndef __CHARACTER_H__
#define __CHARACTER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <cstdint>

// #include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/UnitData.h"
#include "Renderer.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Character {
   protected:
    /* Componentes para renderizar */
    Renderer* g_renderer;
    UnitSpriteContainer* g_sprites;
    int tile_w, tile_h;

    /* Data del personaje */
    CharacterData data;

    /* Componentes para el renderizado gráfico */
    int x, y;           /* posición en pixeles */
    float scale_factor; /* factor para reescalar */

    /* Verifica si el cuerpo entra en el tile, cc calcula el scale_factor */
    void _setScaleFactor();

    /* Renderiza un sprite agregando el offset necesario */
    void _render(const Sprite& sprite) const;

    /* Verifica si tras el update hay que iniciar un movimiento */
    void _startMovementIfNeeded();

    // OLD API ----------------------------------------------------------------

    /*
    int tile_w, tile_h;
    int x_tile, y_tile;
    SDL_Rect box;
    int x_vel, y_vel;
    int next_x, next_y;
    Uint32 last_moved;

    // Centra su posición en pixeles en el tile
    void _centerOnTile();

    // Calcula valor de X al que hay que llegar para completar el movimiento
    int _xValueToReach() const;

    /// Calcula valor de Y al que hay que llegar para completar el movimiento
    int _yValueToReach() const;
    */

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    Character(Renderer* renderer, UnitSpriteContainer* sprites,
              const CharacterData& init_data, const int tile_w,
              const int tile_h);

    /* Deshabilitamos el constructor por copia. */
    Character(const Character&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Character& operator=(const Character&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Character(Character&& other);

    /* Habilitamos el operador= para movimiento. */
    Character& operator=(Character&& other);

    //-------------------------------------------------------------------------

    /* Actualizar información según lo que diga el servidor */
    void update(const CharacterData& updated_data);

    /* Acción que realiza en cada frame */
    void act();

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Character();
};

//-----------------------------------------------------------------------------

#endif  // __CHARACTER_H__
