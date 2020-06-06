#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <fstream>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "Renderer.h"
#include "Window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "HUD.h"
#include "Map.h"
#include "Player.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Exceptions/SDLException.h"
#include "../../../Common/includes/json.hpp"
#include "../../../Common/includes/paths.h"
using json = nlohmann::json;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class GameView {
   private:
    Window window;
    Camera camera;
    Renderer renderer;
    bool sdl_running;
    bool img_running;
    json config;

    //-------------------------------------------------------------------------
    // OBJETOS QUE COMPONEN LA VISTA

    // Contenedor de mapas inicializados

    // Puntero a mapa actual (un mapa solo tiene los tiles estáticos, no se
    // incluyen acá los NPC, jugadores, o objetos sueltos en el mapa, eso será
    // extra)

    // Contenedor de entidades NPC

    // Contenedor de entidades jugadores

    // Contenedor de entidades objetos sueltos

    // Un SDL_Rect que sea la camara?

    // Una unidad que represente al jugador (Player?)

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void _init();

    /* Maneja un evento */
    void _handleEvent(const SDL_Event& e);

    /* Libera la memoria */
    void _free();

   public:
    /* Constructor */
    GameView();

    /* Deshabilitamos el constructor por copia. */
    GameView(const GameView&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    GameView& operator=(const GameView&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    GameView(GameView&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    GameView& operator=(GameView&& other) = delete;

    /* Pone a correr la vista */
    void operator()();

    /* Destructor */
    ~GameView();
};

//-----------------------------------------------------------------------------

#endif  // __GAME_VIEW_H__
