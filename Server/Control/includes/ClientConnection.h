#ifndef __CLIENTCONNECTION_H__
#define __CLIENTCONNECTION_H__

#include "Command.h"
#include "CommandQueue.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/Thread.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include <queue>
#include <string>
#include <mutex>
#include <atomic>
#include <condition_variable>


class ClientConnection {
private:
    // atributos del ClientConnection
    int id;
    SocketWrapper socket;
    CommandQueue* command_queue;
    std::queue<std::string> response_queue;
    std::condition_variable send_cond_var;
    std::mutex send_mutex;
    std::atomic_bool socket_open;

    //threads
    std::thread t_receiver;
    std::thread t_sender;

public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: id del player que usa la connexion
     */
    ClientConnection(int player_id, SocketWrapper&& sock);

    /* Deshabilitamos el constructor por copia. */
    ClientConnection(const ClientConnection&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientConnection& operator=(const ClientConnection&) = delete;

    /* Habilitamo el constructor por movimiento. */
    ClientConnection(ClientConnection&& other);

    /* Deshabilitamos el operador= para movimiento. */
    ClientConnection& operator=(ClientConnection&& other);

    void start();

    void receive();

    void send();

    void writeResponse(bool good, std::string message);
    void writeBroadcast(bool initial, json status);

    /**
     * Descripción: destructor.
     */
    ~ClientConnection();
};

#endif  // __ClientConnection_H__
