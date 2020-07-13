#include "../../../includes/GameModel/Broadcasts/NewPlayerBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NewPlayerBroadcast::NewPlayerBroadcast(const PlayerData& data)
    : Broadcast(), data(data) {}

void NewPlayerBroadcast::exec(MapView& map, Player& player,
                              CharacterContainer& characters,
                              CreatureContainer& creatures) {
    map.selectMap(data.basic_data.map);
    player.init(data);
    map.occupyTile(data.basic_data.gid, data.basic_data.x_tile,
                   data.basic_data.y_tile);
}

NewPlayerBroadcast::~NewPlayerBroadcast() {}

//-----------------------------------------------------------------------------
