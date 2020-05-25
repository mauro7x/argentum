#ifndef __ACTIVE_OBJECT_H__
#define __ACTIVE_OBJECT_H__

//-----------------------------------------------------------------------------
#include <atomic>

#include "Thread.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ActiveObject : public Thread {
   private:
    // Atributos del objeto activo.
    std::atomic_bool is_running;

   public:
    /**
     * Descripción: constuctor.
     *
     * Parámetros: -
     */
    ActiveObject();

    /**
     * Descripción: código que se ejecutará al correr el hilo.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    virtual void run() override;

    /**
     * Descripción: chequea si el hilo se está ejecutando.
     *
     * Parámetros: -
     *
     * Retorno: TRUE si el hilo se está ejecutando, FALSE si no.
     */
    bool isRunning();

    /**
     * Descripción: destructor.
     */
    virtual ~ActiveObject();
};

//-----------------------------------------------------------------------------
#endif  // __ACTIVE_OBJECT_H__
