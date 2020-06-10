#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <fstream>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Exceptions/SDLException.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/paths.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "Renderer.h"
#include "Stage.h"
#include "Window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "HUD.h"
#include "MapView.h"
#include "Player.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Proxies para simular el server, después se cambia

#include "../../../Common/includes/Queue.h"
#include "../Model/ServerProxy.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class GameView {
   private:
    /* Componentes SDL principales */
    Window window;
    Camera camera;
    Renderer renderer;

    /* Flags internos */
    bool sdl_running;
    bool img_running;
    bool fullscreen;

    /* Componentes de la vista */
    HUDProxy hud;
    MapView map;
    UnitSpriteContainer unit_sprites;
    Player player;

    /* Proxies */
    Queue<int*> requests;
    Queue<PlayerData*> broadcast;
    ServerProxy server; /* proxy, luego se reemplaza con la lógica del cliente*/

    /* La escena que se renderizará en cada frame */
    Stage stage;

    /* Inicializa recursos */
    void _init();

    /* Carga media necesaria */
    void _loadMedia();

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
