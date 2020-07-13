#ifndef __UPDATE_PLAYER_BROADCAST_H__
#define __UPDATE_PLAYER_BROADCAST_H__

//-----------------------------------------------------------------------------
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UpdatePlayerBroadcast : public Broadcast {
   private:
    PlayerData data;

   public:
    /* Constructor */
    UpdatePlayerBroadcast(const PlayerData& data);

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    void exec(MapView& map, Player& player, CharacterContainer& characters,
              CreatureContainer& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UpdatePlayerBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __UPDATE_PLAYER_BROADCAST_H__
