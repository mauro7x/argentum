#ifndef __NEW_PLAYER_BROADCAST_H__
#define __NEW_PLAYER_BROADCAST_H__

//-----------------------------------------------------------------------------
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class NewPlayerBroadcast : public Broadcast {
   private:
    PlayerData data;

   public:
    /* Constructor */
    NewPlayerBroadcast(const PlayerData& data);

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    void exec(MapView& map, Player& player,
              UnitContainer<Character, CharacterData>& characters,
              UnitContainer<Creature, CreatureData>& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~NewPlayerBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __NEW_PLAYER_BROADCAST_H__
