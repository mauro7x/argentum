#include "../../../includes/GameModel/Broadcasts/NewItemBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NewItemBroadcast::NewItemBroadcast(const ItemData& data)
    : Broadcast(), data(data) {}

void NewItemBroadcast::exec(MapView& map, Player& player,
                            UnitContainer<Character, CharacterData>& characters,
                            UnitContainer<Creature, CreatureData>& creatures) {
    map.addItem(data.item_id, data.x_tile, data.y_tile);
}

NewItemBroadcast::~NewItemBroadcast() {}

//-----------------------------------------------------------------------------
