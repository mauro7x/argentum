#ifndef __ClientConnection_H__
#define __ClientConnection_H__

#include "Command.h"
#include <queue>
#include <mutex>

class ClientConnection {
private:
    // atributos del ClientConnection
    int id;
    std::queue<Command*> command_queue;
    std::mutex m;

public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: id del player que usa la connexion
     */
    ClientConnection(int player_id);

    /* Deshabilitamos el constructor por copia. */
    ClientConnection(const ClientConnection&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientConnection& operator=(const ClientConnection&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ClientConnection(ClientConnection&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ClientConnection& operator=(ClientConnection&& other) = delete;

    
    void execute_command();

    /**
     * Descripción: destructor.
     */
    ~ClientConnection();
};

#endif  // __ClientConnection_H__
