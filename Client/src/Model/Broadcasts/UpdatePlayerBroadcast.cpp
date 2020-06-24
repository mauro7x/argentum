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
    // FALTA IDENTIFICAR EL CAMBIO DE MAPA. EN ESTE ULTIMO CASO, DEBEMOS LIMPIAR
    // EL MAPA ACTUAL, LIMPIAR CREATURES, CHARACTERS, CAMBIAR EL MAPA Y HACER EL
    // UPDATE EN EL NUEVO MAPA.

    // DEBUG
    fprintf(stderr, "UPDATE RECIBIDO: \n");
    fprintf(stderr,
            "> Exp: %i, Lvlup_exp: %i, Vida: %i, Vida_max: %i, Mana: %i, "
            "Mana_max: %i\n\n",
            data.exp, data.levelup_exp, data.health, data.max_health, data.mana,
            data.max_mana);

    SDL_Point old_pos = player.getPos();
    map.clearTile(old_pos.x, old_pos.y);
    player.update(data);
    map.occupyTile(data.basic_data.gid, data.basic_data.x_tile,
                   data.basic_data.y_tile);
}

UpdatePlayerBroadcast::~UpdatePlayerBroadcast() {}

//-----------------------------------------------------------------------------
