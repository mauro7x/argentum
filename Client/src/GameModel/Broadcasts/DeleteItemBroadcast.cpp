#include "../../../includes/GameModel/Broadcasts/DeleteItemBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

DeleteItemBroadcast::DeleteItemBroadcast(const int x, const int y)
    : Broadcast(), x(x), y(y) {}

void DeleteItemBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    map.clearTileItem(x, y);
}

DeleteItemBroadcast::~DeleteItemBroadcast() {}

//-----------------------------------------------------------------------------
