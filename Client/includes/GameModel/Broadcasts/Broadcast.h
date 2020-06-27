#ifndef __BROADCAST_H__
#define __BROADCAST_H__

//-----------------------------------------------------------------------------
//#include "../../../../Common/includes/DataStructs.h"
//#include "../../View/Character.h"
//#include "../../View/Creature.h"
//#include "../../View/MapView.h"
//#include "../../View/Player.h"
//#include "../../View/UnitContainer.h"
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
    virtual void exec(/*MapView& map, Player& player,
                      UnitContainer<Character, CharacterData>& characters,
                      UnitContainer<Creature, CreatureData>& creatures*/) = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Broadcast() {}
};

//-----------------------------------------------------------------------------

#endif  // __BROADCAST_H__
