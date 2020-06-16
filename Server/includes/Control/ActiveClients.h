#ifndef __ACTIVE_CLIENTS_H__
#define __ACTIVE_CLIENTS_H__

//-----------------------------------------------------------------------------
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Queue.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ClientConnection.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ActiveClients {
   private:
    std::unordered_map<InstanceId, ClientConnection*> content;
    Queue<Command*>& commands;
    Queue<InstanceId*>& finished_connections;

    //-------------------------------------------------------------------------
    // Métodos privados
    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    ActiveClients(Queue<Command*>& commands,
                  Queue<InstanceId*>& finished_connections);

    /* Deshabilitamos el constructor por copia. */
    ActiveClients(const ActiveClients&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ActiveClients& operator=(const ActiveClients&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ActiveClients(ActiveClients&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ActiveClients& operator=(ActiveClients&& other) = delete;

    //-------------------------------------------------------------------------

    /* Agrega un nuevo cliente activo */
    void add(const InstanceId id, SocketWrapper& peer);

    /* Elimina un cliente que ha finalizado su conexión */
    void remove(const InstanceId id);

    /* Termina todas las conexiones forzosamente */
    void stop();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ActiveClients();
};

//-----------------------------------------------------------------------------

#endif  // __ACTIVE_CLIENTS_H__
