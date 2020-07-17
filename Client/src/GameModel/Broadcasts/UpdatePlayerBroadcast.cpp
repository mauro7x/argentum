#include "../../../includes/GameModel/Broadcasts/UpdatePlayerBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UpdatePlayerBroadcast::UpdatePlayerBroadcast(const PlayerData& data)
    : Broadcast(), data(data) {}

void UpdatePlayerBroadcast::exec(MapView& map, Player& player,
                                 CharacterContainer& characters,
                                 CreatureContainer& creatures) {
    SDL_Point old_pos = player.getPos();
    map.clearTileOccupant(old_pos.x, old_pos.y);
    player.update(data);

    if (map.selectMap(data.basic_data.map)) {
        // El mapa cambió

        characters.clear();
        creatures.clear();

        // Aca se limpia el mapa actual. El mapa
        // viejo queda con items y ocupantes, que se
        // van a limpiar cuando viaje a ese mapa.
        map.clear();
    }

    map.occupyTile(data.basic_data.gid, data.basic_data.x_tile,
                   data.basic_data.y_tile);
}

UpdatePlayerBroadcast::~UpdatePlayerBroadcast() {}

//-----------------------------------------------------------------------------
