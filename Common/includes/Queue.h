#ifndef __QUEUE_H__
#define __QUEUE_H__

//-----------------------------------------------------------------------------
#include <mutex>
#include <queue>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

template <class T>
class Queue {
   private:
    std::mutex m;
    std::queue<T> queue;

   public:
    /* Constructor */
    Queue() {}

    /* Deshabilitamos el constructor por copia. */
    Queue(const Queue&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Queue& operator=(const Queue&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Queue(Queue&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Queue& operator=(Queue&& other) = delete;

    /* Push */
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
    ~Queue() {}
};

//-----------------------------------------------------------------------------
#endif  // __QUEUE_H__
