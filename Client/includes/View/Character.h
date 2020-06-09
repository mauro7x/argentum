#ifndef __CHARACTER_H__
#define __CHARACTER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "Renderer.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Data que irá cambiando durante la ejecución */
struct CharacterData {
    int x_tile, y_tile;                            /* coordenadas en tiles */
    Id head_id, body_id;                           /* cuerpo básico */
    Id helmet_id, armour_id, shield_id, weapon_id; /* vestimenta */
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Character {
   protected:
    /* Componentes para renderizar */
    Renderer* g_renderer;
    UnitSpriteContainer* g_sprites;
    int tile_w, tile_h;

    /* Componentes geográficos del personaje */
    int x_tile, y_tile; /* posición en tiles */
    int x, y;           /* posicion en pixeles */
    float scale_factor; /* por si no entramos en el tile a lo ancho */

    /* Componentes gráficos del personaje */
    Id head_id, body_id;                           /* cuerpo */
    Id helmet_id, armour_id, shield_id, weapon_id; /* vestimenta */

    /* Copia la data recibida en sus atributos */
    void _copyData(const CharacterData& data);

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
              const CharacterData& data, const int tile_w, const int tile_h);

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
