#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

//-----------------------------------------------------------------------------
#include <condition_variable>
#include <exception>
#include <mutex>
#include <queue>

#include "ClosedQueueException.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

template <class T>
class BlockingQueue {
   private:
    std::mutex m;
    std::queue<T> queue;
    std::condition_variable cv;
    bool permanently_closed;

   public:
    /**
     * Descripción: constructor. Inicia el closed en false.
     *
     * Parámetros: -
     */
    BlockingQueue() : permanently_closed(false) {}

    /* Deshabilitamos el constructor por copia. */
    BlockingQueue(const BlockingQueue&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    BlockingQueue& operator=(const BlockingQueue&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    BlockingQueue(BlockingQueue&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    BlockingQueue& operator=(BlockingQueue&& other) = delete;

    /**
     * Descripción: inserta un objeto a la cola, y notifica a todos
     * los threads que esperan por la condition-variable.
     *
     * Parámetros: objeto de clase T.
     *
     * Retorno: -
     */
    void push(T t) {
        std::unique_lock<std::mutex> l(m);
        queue.push(t);
        cv.notify_all();
    }

    /**
     * Descripción: extrae un objeto de la cola. Si la misma está vacía,
     * pone a dormir al thread solicitante. Si la misma está vacía y
     * además cerrada, lanza excepción.
     *
     * Parámetros: -
     *
     * Retorno: objeto de clase T.
     */
    T pop() {
        std::unique_lock<std::mutex> l(m);
        while (queue.empty()) {
            if (permanently_closed) {
                throw ClosedQueueException("Queue is closed.");
            }

            cv.wait(l);
        }

        T t = queue.front();
        queue.pop();
        return t;
    }

    /**
     * Descripción: cierra la cola definitivamente, notificando a todos
     * los threads que esperan por la condition-variable.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    void close() {
        std::unique_lock<std::mutex> l(m);
        permanently_closed = true;
        cv.notify_all();
    }

    /**
     * Descripción: destructor.
     */
    ~BlockingQueue() {}
};

//-----------------------------------------------------------------------------
#endif  // __BLOCKING_QUEUE_H__
