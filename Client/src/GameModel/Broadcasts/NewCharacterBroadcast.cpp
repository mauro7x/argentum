#include "../../../includes/GameModel/Broadcasts/NewCharacterBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NewCharacterBroadcast::NewCharacterBroadcast(const CharacterData& data)
    : Broadcast(), data(data) {}

void NewCharacterBroadcast::exec(MapView& map, Player& player,
                                 CharacterContainer& characters,
                                 CreatureContainer& creatures) {
    characters.add(data.basic_data.gid, data);
    map.occupyTile(data.basic_data.gid, data.basic_data.x_tile,
                   data.basic_data.y_tile);
}

NewCharacterBroadcast::~NewCharacterBroadcast() {}

//-----------------------------------------------------------------------------
