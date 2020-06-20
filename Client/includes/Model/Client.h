#ifndef __CLIENT_H__
#define __CLIENT_H__

//-----------------------------------------------------------------------------
#include <stdio.h>

#include <atomic>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/BlockingQueue.h"
#include "../../../Common/includes/NonBlockingQueue.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../View/GameView.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "CommandDispatcher.h"
#include "Commands/Command.h"
#include "Update.h"
#include "Updater.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Client {
   private:
    // Canales de comunicación entre hilos (thread-safe)
    BlockingQueue<Command*> commands;
    NonBlockingQueue<Update*> updates;
    std::atomic_bool exit;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Interactúa con el jugador para conectarlo a un determinado servidor */
    bool _connect(SocketWrapper& socket) const;  // PROXY

    /* Libera la memoria de los comandos o updates que no se hayan procesado */
    void _freeQueues();

    //-------------------------------------------------------------------------
   public:
    /* Constructor */
    Client();

    /* Deshabilitamos el constructor por copia. */
    Client(const Client&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Client& operator=(const Client&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Client(Client&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Client& operator=(Client&& other) = delete;

    /* Pone a correr al cliente */
    void run();

    /* Destructor */
    ~Client();
};

//-----------------------------------------------------------------------------

#endif  // __CLIENT_H__
