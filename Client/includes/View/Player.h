#ifndef __PLAYER_H__
#define __PLAYER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <cstdint>
#include <fstream>

#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/UnitData.h"
#include "../../../Common/includes/paths.h"
#include "Renderer.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
enum PlayerState { NOT_INIT, READY, MOVING };

#define TIME_TO_MOVE 200

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Player {
   protected:
    /* Componentes para renderizar */
    const Renderer* g_renderer;
    const UnitSpriteContainer& g_sprites;
    int tile_w, tile_h;

    /* Data del personaje */
    PlayerData data;
    PlayerState state;

    /* Componentes para el renderizado gráfico */
    float x, y;         /* posición en pixeles */
    float scale_factor; /* factor para reescalar */
    float x_vel, y_vel; /* velocidades en cada componente*/
    Uint32 last_moved;  /* ultimo movimiento */

    /* Settea la velocidad de movimiento en caso de ser necesario */
    void _setMovementSpeed();

    /* Verifica si el movimiento ya terminó (si nos pasamos) */
    void _checkIfMovementHasFinished();

    /* Verifica si el cuerpo entra en el tile, cc calcula el scale_factor */
    void _setScaleFactor();

    /* Renderiza un sprite agregando el offset necesario */
    void _render(const Sprite& sprite) const;

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
    Player(const Renderer* renderer, const UnitSpriteContainer& sprites);

    /* Deshabilitamos el constructor por copia. */
    Player(const Player&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Player& operator=(const Player&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Player(Player&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Player& operator=(Player&& other) = delete;

    //-------------------------------------------------------------------------

    /* Se inicializa con el primer paquete del server */
    void init(const PlayerData& init_data);

    /* Actualizar información según lo que diga el servidor */
    void update(const PlayerData& updated_data);

    /* Acción que realiza en cada frame */
    void act();

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const;

    /* Obtiene la posición en tiles (SOLO X e Y SON VÁLIDOS) */
    SDL_Rect getPos() const;

    /* Obtiene la posición y dimensiones en pixeles */
    SDL_Rect getBox() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Player();
};

//-----------------------------------------------------------------------------

#endif  // __PLAYER_H__
