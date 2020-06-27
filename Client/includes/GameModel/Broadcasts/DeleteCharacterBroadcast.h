#ifndef __DELETE_CHARACTER_BROADCAST_H__
#define __DELETE_CHARACTER_BROADCAST_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/types.h"
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class DeleteCharacterBroadcast : public Broadcast {
   private:
    InstanceId id;

   public:
    /* Constructor */
    DeleteCharacterBroadcast(const InstanceId& id);

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    void exec(MapView& map, Player& player,
              UnitContainer<Character, CharacterData>& characters,
              UnitContainer<Creature, CreatureData>& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~DeleteCharacterBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __DELETE_CHARACTER_BROADCAST_H__
