#ifndef __SERVER_H__
#define __SERVER_H__

//-----------------------------------------------------------------------------
#include <stdio.h>

#include <iostream>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Accepter.h"
#include "Database.h"
#include "Engine.h"
#include "NewConnection.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/NonBlockingQueue.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Server {
   private:
    Database database;
    NonBlockingQueue<NewConnection*> new_connections;
    Accepter accepter;
    Engine engine;

    //-------------------------------------------------------------------------
    // Métodos privados
    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    Server(const std::string& port, const int max_clients_queued);

    /* Deshabilitamos el constructor por copia. */
    Server(const Server&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Server& operator=(const Server&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Server(Server&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Server& operator=(Server&& other) = delete;

    //-------------------------------------------------------------------------

    /* Pone a correr al servidor */
    void run();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Server();
};

//-----------------------------------------------------------------------------

#endif  // __SERVER_H__
