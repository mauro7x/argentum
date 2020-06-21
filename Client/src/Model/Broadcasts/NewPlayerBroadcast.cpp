#include "../../../includes/Model/Broadcasts/NewPlayerBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NewPlayerBroadcast::NewPlayerBroadcast(const PlayerData& data)
    : Broadcast(), data(data) {}

void NewPlayerBroadcast::exec(
    MapView& map, Player& player,
    UnitContainer<Character, CharacterData>& characters,
    UnitContainer<Creature, CreatureData>& creatures) {
    // liberar la posicion anterior del personaje
    map.select(data.basic_data.map);
    player.init(data);
    // ocupar la posición nueva en el mapa
}

NewPlayerBroadcast::~NewPlayerBroadcast() {}

//-----------------------------------------------------------------------------
