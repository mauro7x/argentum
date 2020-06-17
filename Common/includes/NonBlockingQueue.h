#ifndef __NON_BLOCKING_QUEUE_H__
#define __NON_BLOCKING_QUEUE_H__

//-----------------------------------------------------------------------------
#include <mutex>
#include <queue>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

template <class T>
class NonBlockingQueue {
   private:
    std::mutex m;
    std::queue<T> queue;

   public:
    /* Constructor */
    NonBlockingQueue() {}

    /* Deshabilitamos el constructor por copia. */
    NonBlockingQueue(const NonBlockingQueue&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    NonBlockingQueue& operator=(const NonBlockingQueue&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    NonBlockingQueue(NonBlockingQueue&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    NonBlockingQueue& operator=(NonBlockingQueue&& other) = delete;

    /* Push tradicional a la cola */
    void push(T t) {
        std::unique_lock<std::mutex> l(m);
        queue.push(t);
    }

    /* Pop. En caso de estar vacía, devuelve NULL. */
    T pop() {
        std::unique_lock<std::mutex> l(m);

        if (queue.empty()) {
            return NULL;
        }

        T t = queue.front();
        queue.pop();
        return t;
    }

    /**
     * Descripción: destructor.
     */
    ~NonBlockingQueue() {}
};

//-----------------------------------------------------------------------------
#endif  // __NON_BLOCKING_QUEUE_H__
