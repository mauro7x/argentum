#ifndef __ACTIVE_CLIENTS_H__
#define __ACTIVE_CLIENTS_H__

//-----------------------------------------------------------------------------
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/NonBlockingQueue.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ClientConnection.h"
#include "Notifications/Notification.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ActiveClients {
   private:
    // std::unordered_map<InstanceId, ClientConnection*> content;
    std::unordered_map<InstanceId, ClientConnection> content;
    NonBlockingQueue<Command*>& commands;
    NonBlockingQueue<InstanceId*>& finished_connections;

    //-------------------------------------------------------------------------
    // Métodos privados
    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    ActiveClients(NonBlockingQueue<Command*>& commands,
                  NonBlockingQueue<InstanceId*>& finished_connections);

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
    void add(const InstanceId id, const Id map, SocketWrapper& peer);

    /* Elimina un cliente que ha finalizado su conexión */
    void remove(const InstanceId id);

    /* Notifica a un cliente en particular */
    void notify(const InstanceId id, Notification* notification);

    /* Notifica a todos los clientes */
    void notifyAll(Reply* reply);

    /* Cambia el mapa de un cliente */
    void changeMap(const InstanceId id, const Id new_map);

    /*
     * Envía un broadcast de actualización de updated_client a todos los
     * clientes conectados, incluyendo al jugador que se actualizó según
     * send_to_updated_client.
     */
    void sendDifferentialBroadcastToAll(Notification* broadcast,
                                        const InstanceId updated_client,
                                        const bool send_to_updated_client);

    void sendMessageToAll(Notification* message);

    void sendEventToAll(Notification* event);

    /* Termina todas las conexiones forzosamente */
    void stop();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ActiveClients();
};

//-----------------------------------------------------------------------------

#endif  // __ACTIVE_CLIENTS_H__
