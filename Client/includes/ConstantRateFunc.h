#ifndef __CONSTANT_RATE_FUNCTION_H__
#define __CONSTANT_RATE_FUNCTION_H__

//-----------------------------------------------------------------------------
#include <stdio.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <atomic>
#include <chrono>
#include <cmath>
#include <thread>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ConstantRateFunc {
   protected:
    int rate;
    std::atomic_bool exit;

    /* Función a ejecutar con rate constante */
    virtual void _func(const int it) = 0;

   public:
    /* Constructor */
    ConstantRateFunc(int rate);

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
    void stop();

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~ConstantRateFunc();
};

//-----------------------------------------------------------------------------

#endif  // __CONSTANT_RATE_FUNCTION_H__
