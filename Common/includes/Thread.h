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
    /* Constructor */
    Thread();

    /* Deshabilitamos el constructor por copia. */
    Thread(const Thread&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Thread& operator=(const Thread&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Thread(Thread&& other);

    /*Habilitamos el operador= para movimiento. */
    Thread& operator=(Thread&& other);

    /* Lanza la ejecución del thread, que ejecutará el método RUN, a ser
     * overrideado por clases hijas (objetos activos) */
    void start();

    /* Joinea el hilo */
    void join();

    /* Método a ser overrideado por clases hijas, hilo de ejecución */
    virtual void run() = 0;

    /* Destructor */
    virtual ~Thread();
};

//-----------------------------------------------------------------------------
#endif  // __THREAD_H__
