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
    // liberar la posicion anterior del personaje
    creatures.update(data.basic_data.gid, data);
    // ocupar la posición nueva en el mapa
}

UpdateCreatureBroadcast::~UpdateCreatureBroadcast() {}

//-----------------------------------------------------------------------------
