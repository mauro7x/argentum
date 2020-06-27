#ifndef __NEW_CREATURE_BROADCAST_H__
#define __NEW_CREATURE_BROADCAST_H__

//-----------------------------------------------------------------------------
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class NewCreatureBroadcast : public Broadcast {
   private:
    CreatureData data;

   public:
    /* Constructor */
    NewCreatureBroadcast(const CreatureData& data);

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    void exec(MapView& map, Player& player,
              UnitContainer<Character, CharacterData>& characters,
              UnitContainer<Creature, CreatureData>& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~NewCreatureBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __NEW_CREATURE_BROADCAST_H__
