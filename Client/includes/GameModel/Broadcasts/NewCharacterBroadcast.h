#ifndef __NEW_CHARACTER_BROADCAST_H__
#define __NEW_CHARACTER_BROADCAST_H__

//-----------------------------------------------------------------------------
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class NewCharacterBroadcast : public Broadcast {
   private:
    CharacterData data;

   public:
    /* Constructor */
    NewCharacterBroadcast(const CharacterData& data);

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    void exec(MapView& map, Player& player, CharacterContainer& characters,
              CreatureContainer& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~NewCharacterBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __NEW_CHARACTER_BROADCAST_H__
