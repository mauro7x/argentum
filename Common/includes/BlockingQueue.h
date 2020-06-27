#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

//-----------------------------------------------------------------------------
#include <condition_variable>
#include <exception>
#include <mutex>
#include <queue>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* COLA DISEÑADA PARA OBJETOS ALOCADOS EN EL HEAP, SIENDO T SUS PUNTEROS */

template <class T>
class BlockingQueue {
   private:
    std::mutex m;
    std::queue<T> queue;
    std::condition_variable cv;
    bool permanently_closed;

   public:
    /* Constructor */
    BlockingQueue() : permanently_closed(false) {}

    /* Deshabilitamos el constructor por copia. */
    BlockingQueue(const BlockingQueue&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    BlockingQueue& operator=(const BlockingQueue&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    BlockingQueue(BlockingQueue&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    BlockingQueue& operator=(BlockingQueue&& other) = delete;

    /* Inserta un objeto a la cola, y notifica a todos los threads que esperan
     * por la condition-variable. */
    void push(T t) {
        std::unique_lock<std::mutex> l(m);
        queue.push(t);
        cv.notify_all();
    }

    /* Extrae un objeto de la cola. Si la misma está vacía pone a dormir al
     * thread solicitante.
     * Si la misma está vacía y además cerrada, retorna NULL */
    T pop() {
        std::unique_lock<std::mutex> l(m);
        while (queue.empty()) {
            if (permanently_closed) {
                return NULL;
            }

            cv.wait(l);
        }

        T t = queue.front();
        queue.pop();
        return t;
    }

    /* Cierra la cola definitivamente, notificando a todos los threads que
     * esperan por la condition-variable */
    void close() {
        std::unique_lock<std::mutex> l(m);
        permanently_closed = true;
        cv.notify_all();
    }

    /* Destructor */
    ~BlockingQueue() {
        // Sólo tiene sentido si T es un puntero
        while (!queue.empty()) {
            T t = queue.front();
            queue.pop();
            delete t;
        }
    }
};

//-----------------------------------------------------------------------------
#endif  // __BLOCKING_QUEUE_H__
