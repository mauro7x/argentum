#ifndef __MONITOR_H__
#define __MONITOR_H__

//-----------------------------------------------------------------------------
#include <mutex>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Monitor {
   private:
    std::mutex m;
    // Object object;

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: -
     */
    Monitor();

    /* Deshabilitamos el constructor por copia. */
    Monitor(const Monitor&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Monitor& operator=(const Monitor&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Monitor(Monitor&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Monitor& operator=(Monitor&& other) = delete;

    /**
     * Descripción: TODOS los métodos de la API pública deben ser
     * critical sections para nuestro programa.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    void criticalSectionMethod();

    /**
     * Descripción: destructor.
     */
    ~Monitor();
};

//-----------------------------------------------------------------------------
#endif  // __MONITOR_H__
