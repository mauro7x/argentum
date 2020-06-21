#include "../../../includes/Model/Broadcasts/DeleteCharacterBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

DeleteCharacterBroadcast::DeleteCharacterBroadcast(const InstanceId& id)
    : Broadcast(), id(id) {}

void DeleteCharacterBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    SDL_Point old_pos = characters.getPos(id);
    characters.remove(id);

    map.clearTile(old_pos.x, old_pos.y);
}

DeleteCharacterBroadcast::~DeleteCharacterBroadcast() {}

//-----------------------------------------------------------------------------
