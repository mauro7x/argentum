#include "../../../includes/Model/Broadcasts/UpdatePlayerBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UpdatePlayerBroadcast::UpdatePlayerBroadcast(const PlayerData& data)
    : Broadcast(), data(data) {}

void UpdatePlayerBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    // liberar la posicion anterior del personaje
    player.update(data);
    // ocupar la posición nueva en el mapa
}

UpdatePlayerBroadcast::~UpdatePlayerBroadcast() {}

//-----------------------------------------------------------------------------
