#ifndef __COMMAND_DISPATCHER_H__
#define __COMMAND_DISPATCHER_H__

//-----------------------------------------------------------------------------
#include <atomic>
#include <exception>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/BlockingQueue.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/Thread.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Commands/Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class CommandDispatcher : public Thread {
   private:
    const SocketWrapper& socket;       /* SÓLO ESCRITURA (SEND) */
    BlockingQueue<Command*>& commands; /* Comandos a enviar */
    std::atomic_bool& exit;            /* flag de ejecución compartido */

    //-----------------------------------------------------------------------------
    // Métodos privados
    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    CommandDispatcher(const SocketWrapper& socket,
                      BlockingQueue<Command*>& commands,
                      std::atomic_bool& exit);

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
