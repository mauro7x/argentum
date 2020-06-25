#include "../../../includes/Model/Broadcasts/DeleteCreatureBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

DeleteCreatureBroadcast::DeleteCreatureBroadcast(const InstanceId& id)
    : Broadcast(), id(id) {}

void DeleteCreatureBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    SDL_Point old_pos = creatures.getPos(id);
    creatures.remove(id);

    map.clearTileOcuppant(old_pos.x, old_pos.y);
}

DeleteCreatureBroadcast::~DeleteCreatureBroadcast() {}

//-----------------------------------------------------------------------------
