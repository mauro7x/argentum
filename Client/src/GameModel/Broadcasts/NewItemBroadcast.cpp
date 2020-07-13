#include "../../../includes/GameModel/Broadcasts/NewItemBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NewItemBroadcast::NewItemBroadcast(const ItemData& data)
    : Broadcast(), data(data) {}

void NewItemBroadcast::exec(MapView& map, Player& player,
                            CharacterContainer& characters,
                            CreatureContainer& creatures) {
    map.addItem(data.item_id, data.amount, data.x_tile, data.y_tile);
}

NewItemBroadcast::~NewItemBroadcast() {}

//-----------------------------------------------------------------------------
