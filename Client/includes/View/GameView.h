#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../Model/Broadcasts/Broadcast.h"
#include "../Model/Commands/Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../defs.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "Character.h"
#include "Creature.h"
#include "EventHandler.h"
#include "HUD.h"
#include "ItemSpriteContainer.h"
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
    // Comunicación entre hilos
    BlockingQueue<Command*>& commands;  // es necesario tenerla aca?
    NonBlockingQueue<Broadcast*>& broadcasts;
    std::atomic_bool& exit;

    // Componentes SDL principales
    Window window;
    Camera camera;
    Renderer renderer;

    // Contenedores de sprites
    UnitSpriteContainer unit_sprites;
    ItemSpriteContainer item_sprites;

    // Unidades
    Player player;
    UnitContainer<Character, CharacterData> characters;
    UnitContainer<Creature, CreatureData> creatures;

    // Componentes de la vista
    HUD hud;
    MapView map;

    // Otros
    Stage stage;
    EventHandler event_handler;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Inicializa recursos */
    void _init();

    /* Carga media necesaria */
    void _loadMedia();

    /* Vacía la cola de eventos de SDL */
    void _processSDLEvents();

    /* Vacía la cola de broadcasts del servidor */
    void _processBroadcasts();

    /* Ejecuta una iteración del loop */
    void _loopIteration(const int it);

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    GameView(BlockingQueue<Command*>& commands,
             NonBlockingQueue<Broadcast*>& broadcasts, std::atomic_bool& exit);

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
