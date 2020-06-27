#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

//-----------------------------------------------------------------------------
#include <chrono>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/BlockingQueue.h"
#include "../../../Common/includes/DataStructs.h"
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/NonBlockingQueue.h"
#include "../../../Common/includes/paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../SDL/Renderer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../GameModel/Broadcasts/Broadcast.h"
#include "../GameModel/Commands/Command.h"
#include "../GameModel/Messages/Message.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../ConstantRateFunc.h"
#include "../contexts.h"
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
#include "Stage.h"
#include "UnitContainer.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class GameView : public ConstantRateFunc {
   private:
    // Comunicación entre hilos
    BlockingQueue<Command*>& commands;
    NonBlockingQueue<Broadcast*>& broadcasts;
    NonBlockingQueue<Message*>& messages;

    // Componentes principales
    Renderer& renderer;
    Camera camera;

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

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Inicializa recursos internos */
    void _init();

    /* Carga la media necesaria */
    void _loadMedia();

    /* Vacía la cola de eventos de SDL */
    void _processSDLEvents();

    /* Vacía la cola de mensajes del servidor */
    void _processMessages();

    /* Vacía la cola de broadcasts del servidor */
    void _processBroadcasts();

    /* Iteración del loop */
    void _func(const int it) override;

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    GameView(BlockingQueue<Command*>& commands,
             NonBlockingQueue<Broadcast*>& broadcasts,
             NonBlockingQueue<Message*>& messages, Renderer& renderer);

    /* Destructor */
    ~GameView();
};

//-----------------------------------------------------------------------------

#endif  // __GAME_VIEW_H__
