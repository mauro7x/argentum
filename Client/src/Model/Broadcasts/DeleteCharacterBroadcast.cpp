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
    characters.remove(id);
    // liberar la posicion
}

DeleteCharacterBroadcast::~DeleteCharacterBroadcast() {}

//-----------------------------------------------------------------------------
