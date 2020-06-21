#include "../../../includes/Model/Broadcasts/DeleteCreatureBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

DeleteCreatureBroadcast::DeleteCreatureBroadcast(const InstanceId& id)
    : Broadcast(), id(id) {}

void DeleteCreatureBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    creatures.remove(id);
    // liberar la posicion
}

DeleteCreatureBroadcast::~DeleteCreatureBroadcast() {}

//-----------------------------------------------------------------------------
