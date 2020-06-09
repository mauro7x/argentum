#ifndef __PLAYER_H__
#define __PLAYER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <cstdint>
#include <fstream>

#include "../../../Common/includes/paths.h"
#include "Renderer.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Librería de json a utilizar (https://github.com/nlohmann/json).
#include "../../../Common/includes/json.hpp"
using json = nlohmann::json;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Data que irá cambiando durante la ejecución */
struct PlayerData {
    int x_tile, y_tile;                            /* coordenadas en tiles */
    uint16_t health, mana;                         /* stats */
    Id head_id, body_id;                           /* cuerpo básico */
    Id helmet_id, armour_id, shield_id, weapon_id; /* vestimenta */
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Player {
   protected:
    /* Componentes para renderizar */
    Renderer* g_renderer;
    UnitSpriteContainer* g_sprites;
    int tile_w, tile_h;

    /* Data del personaje */
    PlayerData data;

    /* Componentes para el renderizado gráfico */
    int x, y;           /* posición en pixeles */
    float scale_factor; /* factor para reescalar */

    /* Carga un json */
    json _loadJsonFile(std::string filepath) const;

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
    Player(Renderer* renderer, UnitSpriteContainer* sprites);

    /* Deshabilitamos el constructor por copia. */
    Player(const Player&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Player& operator=(const Player&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Player(Player&& other);

    /* Habilitamos el operador= para movimiento. */
    Player& operator=(Player&& other);

    //-------------------------------------------------------------------------

    /* Se inicializa con el primer paquete del server */
    void init(const PlayerData& init_data);

    /* Actualizar información según lo que diga el servidor */
    void update(const PlayerData& updated_data);

    /* Acción que realiza en cada frame */
    void act();

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Player();
};

//-----------------------------------------------------------------------------

#endif  // __PLAYER_H__
