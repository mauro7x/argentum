#ifndef __COMMAND_DISPATCHER_H__
#define __COMMAND_DISPATCHER_H__

//-----------------------------------------------------------------------------
#include <exception>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/BlockingQueue.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/Thread.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../GameView/GameView.h"
#include "Commands/Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class CommandDispatcher : public Thread {
   private:
    const SocketWrapper& socket;       /* SÓLO ESCRITURA (SEND) */
    BlockingQueue<Command*>& commands; /* Comandos a enviar */
    GameView& game_view;

    //-----------------------------------------------------------------------------
    // Métodos privados
    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    CommandDispatcher(const SocketWrapper& socket,
                      BlockingQueue<Command*>& commands, GameView& game_view);

    /* Deshabilitamos el constructor por copia. */
    CommandDispatcher(const CommandDispatcher&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    CommandDispatcher& operator=(const CommandDispatcher&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    CommandDispatcher(CommandDispatcher&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    CommandDispatcher& operator=(CommandDispatcher&& other) = delete;

    //-----------------------------------------------------------------------------

    /* Hilo principal de ejecución */
    void run() override;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~CommandDispatcher();
};

//-----------------------------------------------------------------------------

#endif  // __COMMAND_DISPATCHER_H__
