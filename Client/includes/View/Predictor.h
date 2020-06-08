#ifndef __PREDICTOR_H__
#define __PREDICTOR_H__

//-----------------------------------------------------------------------------
#include "MapView.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Predictor {
   private:
    const MapView& map;

   public:
    /* Constructor */
    Predictor(const MapView& map);

    /* Deshabilitamos el constructor por copia. */
    Predictor(const Predictor&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Predictor& operator=(const Predictor&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Predictor(Predictor&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Predictor& operator=(Predictor&& other) = delete;

    //-------------------------------------------------------------------------

    /* Predice si el jugador se puede mover a una posición */
    bool canMoveTo(const int x, const int y) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Predictor();
};

//-----------------------------------------------------------------------------

#endif  // __PREDICTOR_H__
