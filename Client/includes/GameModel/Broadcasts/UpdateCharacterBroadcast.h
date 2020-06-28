#ifndef __UPDATE_CHARACTER_BROADCAST_H__
#define __UPDATE_CHARACTER_BROADCAST_H__

//-----------------------------------------------------------------------------
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UpdateCharacterBroadcast : public Broadcast {
   private:
    CharacterData data;

   public:
    /* Constructor */
    UpdateCharacterBroadcast(const CharacterData& data);

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    void exec(MapView& map, Player& player,
              UnitContainer<Character, CharacterData>& characters,
              UnitContainer<Creature, CreatureData>& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UpdateCharacterBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __UPDATE_CHARACTER_BROADCAST_H__
