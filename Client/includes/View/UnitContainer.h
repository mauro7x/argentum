#ifndef __UNIT_CONTAINER_H__
#define __UNIT_CONTAINER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <vector>

#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UnitContainer {
   private:
    std::vector<Unit*> content;

    /* Libera recursos de las unidades */
    void _free();

   public:
    /* Constructor */
    UnitContainer();

    /* Deshabilitamos el constructor por copia. */
    UnitContainer(const UnitContainer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    UnitContainer& operator=(const UnitContainer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    UnitContainer(UnitContainer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    UnitContainer& operator=(UnitContainer&& other) = delete;

    /* Agrega una nueva unidad, se responsabiliza por su memoria */
    void add(Unit* new_unit);

    /* Handler de eventos */
    void handleEvent(const SDL_Event& e) const;

    /* Destructor */
    ~UnitContainer();
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_CONTAINER_H__
