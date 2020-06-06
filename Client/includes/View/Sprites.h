#ifndef __SPRITES_H__
#define __SPRITES_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/json.hpp"
#include "../../../Common/includes/paths.h"
using json = nlohmann::json;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Sprites {
   private:
    // atributos
    // metodos privados

   public:
    /* Constructor */
    Sprites();

    /* Deshabilitamos el constructor por copia. */
    Sprites(const Sprites&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Sprites& operator=(const Sprites&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Sprites(Sprites&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Sprites& operator=(Sprites&& other) = delete;

    //-------------------------------------------------------------------------

    /* Metodo */
    void metodo() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Sprites();
};

//-----------------------------------------------------------------------------

#endif  // __SPRITES_H__
