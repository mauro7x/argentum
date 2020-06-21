#include "../../../includes/Model/Broadcasts/NewCharacterBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NewCharacterBroadcast::NewCharacterBroadcast(const CharacterData& data)
    : Broadcast(), data(data) {}

void NewCharacterBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    characters.add(data.basic_data.gid, data);
    map.occupyTile(data.basic_data.gid, data.basic_data.x_tile,
                   data.basic_data.y_tile);
}

NewCharacterBroadcast::~NewCharacterBroadcast() {}

//-----------------------------------------------------------------------------
