#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
#include "Character.h"
#include "Console.h"
#include "Creature.h"
#include "EventHandler.h"
#include "HUD.h"
#include "MapView.h"
#include "Player.h"
#include "Renderer.h"
#include "Stage.h"
#include "UnitContainer.h"
#include "UnitSpriteContainer.h"
#include "Window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Proxies para simular el server, después se cambia

#include "../../../Common/includes/NonBlockingQueue.h"
#include "../Model/ServerProxy.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class GameView {
   private:
    /* Componentes SDL principales */
    Window window;
    Camera camera;
    Renderer renderer;
    int rate;

    /* Flag de ejecución */
    bool view_running;

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
    NonBlockingQueue<int*> requests;
    NonBlockingQueue<PlayerData*> broadcast;
    ServerProxy server; /* proxy, luego se reemplaza con la lógica del cliente*/

    /* La escena que se renderizará en cada frame */
    Stage stage;

    /* Objetos activo que handlea eventos */
    EventHandler event_handler;

    /* Inicializa recursos */
    void _init();

    /* Carga media necesaria */
    void _loadMedia();

    /* Vacía la cola de eventos de SDL */
    void _processSDLEvents();

    /* Vacía la cola de updates del servidor */
    void _processServerUpdates();

    /* Ejecuta una iteración del loop */
    void _loopIteration(const int it);

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
