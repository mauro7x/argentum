#ifndef __NEW_ITEM_BROADCAST_H__
#define __NEW_ITEM_BROADCAST_H__

//-----------------------------------------------------------------------------
#include "Broadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class NewItemBroadcast : public Broadcast {
   private:
    ItemData data;

   public:
    /* Constructor */
    NewItemBroadcast(const ItemData& data);

    //-------------------------------------------------------------------------

    /* Ejecución polimórfica del broadcast */
    void exec(MapView& map, Player& player, CharacterContainer& characters,
              CreatureContainer& creatures);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~NewItemBroadcast();
};

//-----------------------------------------------------------------------------

#endif  // __NEW_ITEM_BROADCAST_H__
