#include "../../../includes/GameModel/Broadcasts/DeleteCreatureBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

DeleteCreatureBroadcast::DeleteCreatureBroadcast(const InstanceId& id)
    : Broadcast(), id(id) {}

void DeleteCreatureBroadcast::exec(MapView& map, Player& player,
                                   CharacterContainer& characters,
                                   CreatureContainer& creatures) {
    SDL_Point old_pos = creatures.getPos(id);
    creatures.remove(id);

    map.clearTileOccupant(old_pos.x, old_pos.y);
}

DeleteCreatureBroadcast::~DeleteCreatureBroadcast() {}

//-----------------------------------------------------------------------------
