#ifndef __ClientConnection_H__
#define __ClientConnection_H__

#include <queue>
#include <mutex>

class CommandQueue {
private:
    // atributos del Command Queue
    std::queue<Command*> command_queue;
    std::mutex m;

public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: id del player que usa la connexion
     */
    CommandQueue(int player_id, Socket&& sock);

    /* Deshabilitamos el constructor por copia. */
    CommandQueue(const CommandQueue&) = delete;
    /* Deshabilitamos el operador= para copia.*/
    CommandQueue& operator=(const CommandQueue&) = delete;
    /* Deshabilitamos el constructor por movimiento. */
    CommandQueue(CommandQueue&& other) = delete;
    /* Deshabilitamos el operador= para movimiento. */
    CommandQueue& operator=(CommandQueue&& other) = delete;

    void run();

    /**
     * Descripción: destructor.
     */
    ~CommandQueue();
};

#endif  // __ClientConnection_H__
