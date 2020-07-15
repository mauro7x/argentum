#ifndef __CONSTANT_RATE_FUNCTION_H__
#define __CONSTANT_RATE_FUNCTION_H__

//-----------------------------------------------------------------------------
#include <stdio.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <thread>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define MAX_FPS_ALLOWED 120
#define ORIGINAL_RATE 16
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ConstantRateFunc {
   protected:
    const uint16_t original_rate;
    uint16_t rate;
    uint16_t delta_ms;
    std::atomic_bool exit;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Función a ejecutar con rate constante */
    virtual void _func(const int it) = 0;

    /** Actualiza el it a pasarle a _func para mantener el rate original.
     *
     * Motivación: a fin de permitirle al usuario ingresar cualquier FPS con el
     * que desee jugar, pero al mismo tiempo que la animación sea la misma (con
     * menos o más frames), se implementa esta función que convierte las
     * iteraciones en tiempo transcurrido según el rate actual. Luego ese tiempo
     * lo divide segun el rate original, para así obtener las iteraciones que
     * transcurrieron. Lo que sobra, se acumula hasta llegar a completar una
     * iteracion entera.
     */
    void _updateIt(int& it);

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    ConstantRateFunc();

    /* Deshabilitamos el constructor por copia. */
    ConstantRateFunc(const ConstantRateFunc&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ConstantRateFunc& operator=(const ConstantRateFunc&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ConstantRateFunc(ConstantRateFunc&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ConstantRateFunc& operator=(ConstantRateFunc&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública

    /* Ejecuta el loop */
    void run();

    /* Detiene el loop (thread-safe) */
    void quit();

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~ConstantRateFunc();
};

//-----------------------------------------------------------------------------

#endif  // __CONSTANT_RATE_FUNCTION_H__
