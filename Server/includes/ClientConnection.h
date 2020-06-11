#ifndef __CLIENTCONNECTION_H__
#define __CLIENTCONNECTION_H__

#include "Command.h"
#include "CommandQueue.h"
#include "../../Abstractions/Threads/Thread/Thread.h"
#include <queue>
#include <mutex>

class ClientConnection : Thread {
private:
    // atributos del ClientConnection
    int id;
    CommandQueue command_queue;
    std::mutex m;

public:
    /**
     * Descripci�n: constructor.
     *
     * Par�metros: id del player que usa la connexion
     */
    ClientConnection(int player_id, Socket&& sock);

    /* Deshabilitamos el constructor por copia. */
    ClientConnection(const ClientConnection&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientConnection& operator=(const ClientConnection&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ClientConnection(ClientConnection&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ClientConnection& operator=(ClientConnection&& other) = delete;

    void run();

    /**
     * Descripci�n: destructor.
     */
    ~ClientConnection();
};

#endif  // __ClientConnection_H__
