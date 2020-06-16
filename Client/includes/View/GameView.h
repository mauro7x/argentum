#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <fstream>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Exceptions/SDLException.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/UnitData.h"
#include "../../../Common/includes/paths.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "EventHandler.h"
#include "Renderer.h"
#include "Stage.h"
#include "Window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Character.h"
#include "Creature.h"
#include "HUD.h"
#include "Console.h"
#include "MapView.h"
#include "Player.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Metiendo los contenedores de unidades
#include "UnitContainer.h"
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
    uint32_t rate;

    /* Flag de ejecución */
    std::atomic_bool view_running;

    /* Componentes de la vista */
    HUDProxy hud;
    ConsoleProxy console;
    MapView map;

    /* Unidades */
    UnitSpriteContainer unit_sprites;
    Player player;
    UnitContainer<Character, CharacterData> characters;
    UnitContainer<Creature, CreatureData> creatures;

    /* Proxies */
    Queue<int*> requests;
    Queue<PlayerData*> broadcast;
    ServerProxy server; /* proxy, luego se reemplaza con la lógica del cliente*/

    /* La escena que se renderizará en cada frame */
    Stage stage;

    /* Objetos activo que handlea eventos */
    EventHandler event_handler;

    /* Inicializa recursos */
    void _init();

    /* Carga media necesaria */
    void _loadMedia();

    /* Ejecuta una iteración del game */
    void _gameIteration(uint32_t it);

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
