#ifndef __THREAD_H__
#define __THREAD_H__

//-----------------------------------------------------------------------------
#include <thread>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Thread {
   private:
    std::thread thread;

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: -
     */
    Thread();

    /* Deshabilitamos el constructor por copia. */
    Thread(const Thread&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Thread& operator=(const Thread&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Thread(Thread&& other);

    /*Habilitamos el operador= para movimiento. */
    Thread& operator=(Thread&& other);

    /**
     * Descripción: lanza la ejecución del thread, que ejecutará el
     * método RUN, a ser overrideado por clases hijas (objetos activos)/
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    void start();

    /**
     * Descripción: joinea el hilo de ejecución.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    void join();

    /**
     * Descripción: método a ser overrideado por clases hijas, hilo
     * de ejecución.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    virtual void run() = 0;

    /**
     * Descripción: destructor.
     */
    virtual ~Thread();
};

//-----------------------------------------------------------------------------
#endif  // __THREAD_H__
