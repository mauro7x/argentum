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
    void exec(MapView& map, Player& player, CharacterContainer& characters,
              CreatureContainer& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~NewPlayerBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __NEW_PLAYER_BROADCAST_H__
