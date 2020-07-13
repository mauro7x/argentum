#ifndef __DELETE_CREATURE_BROADCAST_H__
#define __DELETE_CREATURE_BROADCAST_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/types.h"
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class DeleteCreatureBroadcast : public Broadcast {
   private:
    InstanceId id;

   public:
    /* Constructor */
    DeleteCreatureBroadcast(const InstanceId& id);

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    void exec(MapView& map, Player& player, CharacterContainer& characters,
              CreatureContainer& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~DeleteCreatureBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __DELETE_CREATURE_BROADCAST_H__
