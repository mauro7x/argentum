#ifndef __DELETE_ITEM_BROADCAST_H__
#define __DELETE_ITEM_BROADCAST_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/types.h"
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class DeleteItemBroadcast : public Broadcast {
   private:
    const int x;
    const int y;

   public:
    /* Constructor */
    DeleteItemBroadcast(const int x, const int y);

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    void exec(MapView& map, Player& player,
              UnitContainer<Character, CharacterData>& characters,
              UnitContainer<Creature, CreatureData>& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~DeleteItemBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __DELETE_ITEM_BROADCAST_H__
