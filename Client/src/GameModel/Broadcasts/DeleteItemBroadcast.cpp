#include "../../../includes/GameModel/Broadcasts/DeleteItemBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

DeleteItemBroadcast::DeleteItemBroadcast(const int x, const int y)
    : Broadcast(), x(x), y(y) {}

void DeleteItemBroadcast::exec(MapView& map, Player& player,
                               CharacterContainer& characters,
                               CreatureContainer& creatures) {
    map.clearTileItem(x, y);
}

DeleteItemBroadcast::~DeleteItemBroadcast() {}

//-----------------------------------------------------------------------------
