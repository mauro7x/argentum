#include "../../../includes/Model/Broadcasts/NewPlayerBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NewPlayerBroadcast::NewPlayerBroadcast(const PlayerData& data)
    : Broadcast(), data(data) {}

void NewPlayerBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    map.select(data.basic_data.map);
    player.init(data);
    map.occupyTile(data.basic_data.gid, data.basic_data.x_tile,
                   data.basic_data.y_tile);
}

NewPlayerBroadcast::~NewPlayerBroadcast() {}

//-----------------------------------------------------------------------------
