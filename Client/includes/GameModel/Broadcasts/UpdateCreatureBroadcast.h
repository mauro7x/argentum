#ifndef __UPDATE_CREATURE_BROADCAST_H__
#define __UPDATE_CREATURE_BROADCAST_H__

//-----------------------------------------------------------------------------
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UpdateCreatureBroadcast : public Broadcast {
   private:
    CreatureData data;

   public:
    /* Constructor */
    UpdateCreatureBroadcast(const CreatureData& data);

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    void exec(MapView& map, Player& player,
              UnitContainer<Character, CharacterData>& characters,
              UnitContainer<Creature, CreatureData>& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UpdateCreatureBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __UPDATE_CREATURE_BROADCAST_H__
