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

    //-------------------------------------------------------------------------
    // PROXY PARA HARCODEAR EL INVENTARIO Y PROBAR LA HUD

    data.equipment = {1100, 1302, 1402, 0};
    data.inventory = {
        InventorySlot({1000, 1}),   InventorySlot({1001, 1}),
        InventorySlot({1102, 2}),   InventorySlot({0, 0}),
        InventorySlot({0, 0}),      InventorySlot({0, 0}),
        InventorySlot({0, 0}),      InventorySlot({0, 0}),
        InventorySlot({0, 0}),      InventorySlot({0, 0}),
        InventorySlot({0, 0}),      InventorySlot({1300, 9000}),
        InventorySlot({0, 0}),      InventorySlot({0, 0}),
        InventorySlot({1401, 454}), InventorySlot({1500, 1}),
        InventorySlot({1400, 1}),   InventorySlot({1301, 1}),
    };
    //-------------------------------------------------------------------------

    SDL_Point old_pos = player.getPos();
    map.clearTile(old_pos.x, old_pos.y);
    player.update(data);
    map.occupyTile(data.basic_data.gid, data.basic_data.x_tile,
                   data.basic_data.y_tile);
}

UpdatePlayerBroadcast::~UpdatePlayerBroadcast() {}

//-----------------------------------------------------------------------------
