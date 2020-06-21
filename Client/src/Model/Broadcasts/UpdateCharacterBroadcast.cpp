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
    // liberar la posicion anterior del personaje
    characters.update(data.basic_data.gid, data);
    // ocupar la posición nueva en el mapa
}

UpdateCharacterBroadcast::~UpdateCharacterBroadcast() {}

//-----------------------------------------------------------------------------
