#include "../../../includes/Model/Broadcasts/UpdateCreatureBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UpdateCreatureBroadcast::UpdateCreatureBroadcast(const CreatureData& data)
    : Broadcast(), data(data) {}

void UpdateCreatureBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    SDL_Point old_pos = creatures.getPos(data.basic_data.gid);
    map.clearTileOccupant(old_pos.x, old_pos.y);
    creatures.update(data.basic_data.gid, data);
    map.occupyTile(data.basic_data.gid, data.basic_data.x_tile,
                   data.basic_data.y_tile);
}

UpdateCreatureBroadcast::~UpdateCreatureBroadcast() {}

//-----------------------------------------------------------------------------
