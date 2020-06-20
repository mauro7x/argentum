#ifndef __CLIENT_CONNECTION_H__
#define __CLIENT_CONNECTION_H__

//-----------------------------------------------------------------------------
#include <exception>
#include <mutex>
#include <thread>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/BlockingQueue.h"
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/NonBlockingQueue.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Command.h"
#include "Notification.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClientConnection {
   private:
    InstanceId id;
    SocketWrapper peer;
    NonBlockingQueue<InstanceId*>& finished_connections;

    // Flag de control compartido entre ambos threads
    std::mutex m;
    int finished_threads;

    // Sender
    std::thread sender;
    BlockingQueue<Notification*> notifications;

    // Receiver
    std::thread receiver;
    NonBlockingQueue<Command*>& commands;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Método que tanto el sender como el receiver llamarán cuando terminen.
     * Cuando ambos lo llamen, se agrega la conexión a una lista de conexiones
     * terminadas a ser eliminadas por el servidor. */
    void _finishThread();

    /* Libera la memoria de las notificaciones que quedaron por enviar */
    void _freeNotifications();

    /* Función que corre en el hilo del sender */
    void _sender();

    /* Función que corre en el hilo del receiver */
    void _receiver();

    /* [RECEIVER] Recibe un comando */
    void _receiveCommand(char opcode);

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    ClientConnection(const InstanceId id, SocketWrapper& peer,
                     NonBlockingQueue<InstanceId*>& finished_connections,
                     NonBlockingQueue<Command*>& commands);

    /* Deshabilitamos el constructor por copia. */
    ClientConnection(const ClientConnection&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientConnection& operator=(const ClientConnection&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ClientConnection(ClientConnection&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ClientConnection& operator=(ClientConnection&& other) = delete;

    //-------------------------------------------------------------------------

    /* Inicia ambos hilos: el sender y el receiver */
    void start();

    /* Agrega notificación a enviar a la cola */
    void push(Notification* notification);

    /* Joinea ambos hilos: el sender y el receiver */
    void join();

    /* Cierra la conexión de manera forzosa */
    void stop();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ClientConnection();
};

//-----------------------------------------------------------------------------

#endif  // __CLIENT_CONNECTION_H__
