#ifndef __CLIENT_H__
#define __CLIENT_H__

//-----------------------------------------------------------------------------
#include <stdio.h>

#include <atomic>
#include <exception>
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
#include "Broadcasts/Broadcast.h"
#include "CommandDispatcher.h"
#include "Commands/Command.h"
#include "Receiver.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Client {
   private:
    // Canales de comunicación entre hilos (thread-safe)
    BlockingQueue<Command*> commands;
    NonBlockingQueue<Broadcast*> broadcasts;
    std::atomic_bool exit;
    std::atomic_bool first_package_received;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Interactúa con el jugador para conectarlo a un determinado servidor */
    bool _connect(SocketWrapper& socket) const;  // PROXY

    /* Libera la memoria de comandos/broadcasts que no se hayan procesado */
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
