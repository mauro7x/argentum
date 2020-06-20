#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <atomic>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <thread>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/BlockingQueue.h"
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Exceptions/SDLException.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/NonBlockingQueue.h"
#include "../../../Common/includes/UnitData.h"
#include "../../../Common/includes/paths.h"
#include "../Model/Commands/Command.h"
#include "../Model/Update.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "Character.h"
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

class GameView {
   private:
    /* Comunicación entre hilos */
    BlockingQueue<Command*>& commands;  // es necesario tenerla aca?
    NonBlockingQueue<Update*>& updates;
    std::atomic_bool& exit;

    /* Componentes SDL principales */
    Window window;
    Camera camera;
    Renderer renderer;
    int rate;

    /* Unidades */
    UnitSpriteContainer unit_sprites;
    Player player;
    UnitContainer<Character, CharacterData> characters;
    UnitContainer<Creature, CreatureData> creatures;

    /* Componentes de la vista */
    HUD hud;
    MapView map;

    /* La escena que se renderizará en cada frame */
    Stage stage;

    /* Objeto activo que handlea eventos */
    EventHandler event_handler;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Inicializa recursos */
    void _init();

    /* Carga media necesaria */
    void _loadMedia();

    /* Vacía la cola de eventos de SDL */
    void _processSDLEvents();

    /* Vacía la cola de updates del servidor */
    void _processUpdates();

    /* Ejecuta una iteración del loop */
    void _loopIteration(const int it);

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    GameView(BlockingQueue<Command*>& commands,
             NonBlockingQueue<Update*>& updates, std::atomic_bool& exit);

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
