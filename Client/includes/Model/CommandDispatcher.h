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
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class CommandDispatcher : public Thread {
   private:
    const SocketWrapper& socket;       /* SÓLO ESCRITURA (SEND) */
    BlockingQueue<Command*>& commands; /* Comandos a enviar */

    //-----------------------------------------------------------------------------
    // Métodos privados
    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    CommandDispatcher(const SocketWrapper& socket,
                      BlockingQueue<Command*>& commands);

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

    /* Detiene la ejecución cerrando la cola */
    void stop();

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~CommandDispatcher();
};

//-----------------------------------------------------------------------------

#endif  // __COMMAND_DISPATCHER_H__
