#include "../../../includes/Model/Broadcasts/NewCreatureBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NewCreatureBroadcast::NewCreatureBroadcast(const CreatureData& data)
    : Broadcast(), data(data) {}

void NewCreatureBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    creatures.add(data.basic_data.gid, data);
    // agregarlo en el mapa
}

NewCreatureBroadcast::~NewCreatureBroadcast() {}

//-----------------------------------------------------------------------------
