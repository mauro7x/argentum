#ifndef __BROADCAST_H__
#define __BROADCAST_H__

//-----------------------------------------------------------------------------
#include "../../GameView/Character.h"
#include "../../GameView/CharacterContainer.h"
#include "../../GameView/Creature.h"
#include "../../GameView/CreatureContainer.h"
#include "../../GameView/MapView.h"
#include "../../GameView/Player.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Broadcast abstracto que se sabe ejecutar. Puede recibir parámetros en el
 * método de ejecución según sea necesario. Tener en cuenta que los atributos
 * particulares que necesite cada comando deben ser agregados como atributos
 * específicos de las clases concretas que hereden de esta. */

class Broadcast {
   public:
    /* Constructor */
    Broadcast() {}

    /* Deshabilitamos el constructor por copia. */
    Broadcast(const Broadcast&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Broadcast& operator=(const Broadcast&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Broadcast(Broadcast&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Broadcast& operator=(Broadcast&& other) = delete;

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    virtual void exec(MapView& map, Player& player,
                      CharacterContainer& characters,
                      CreatureContainer& creatures) = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Broadcast() {}
};

//-----------------------------------------------------------------------------

#endif  // __BROADCAST_H__
