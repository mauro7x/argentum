#include "../../../includes/Model/Broadcasts/UpdateCharacterBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UpdateCharacterBroadcast::UpdateCharacterBroadcast(const CharacterData& data)
    : Broadcast(), data(data) {}

void UpdateCharacterBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    SDL_Point old_pos = characters.getPos(data.basic_data.gid);
    map.clearTileOcuppant(old_pos.x, old_pos.y);
    characters.update(data.basic_data.gid, data);
    map.occupyTile(data.basic_data.gid, data.basic_data.x_tile,
                   data.basic_data.y_tile);
}

UpdateCharacterBroadcast::~UpdateCharacterBroadcast() {}

//-----------------------------------------------------------------------------
