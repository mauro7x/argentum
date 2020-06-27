#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

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

class GameView : public ConstantRateFunc {
   private:
    // Comunicación entre hilos
    BlockingQueue<Command*>& commands;
    NonBlockingQueue<Broadcast*>& broadcasts;
    NonBlockingQueue<Message*>& messages;

    // Componentes SDL principales
    // Window window;
    // Camera camera;
    Renderer& renderer;

    //-----------------------------------------------------------------------------
    // Métodos privados

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
