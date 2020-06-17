#ifndef __CLIENT_CONNECTION_H__
#define __CLIENT_CONNECTION_H__

//-----------------------------------------------------------------------------
#include <atomic>
#include <mutex>
#include <thread>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
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
    NonBlockingQueue<Notification*> notifications;

    // Receiver
    std::thread receiver;
    NonBlockingQueue<Command*>& commands;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Método que los hilos llaman cuando terminan para verificar si la conexión
     * ya terminó y notificarlo mediante la cola finished_connections. */
    void _finishThread();

    /* Función que corre en el hilo del sender */
    void _sender();

    /* Función que corre en el hilo del receiver */
    void _receiver();

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

    /* Termina la conexión de manera forzosa */
    void stop();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ClientConnection();
};

//-----------------------------------------------------------------------------

#endif  // __CLIENT_CONNECTION_H__
