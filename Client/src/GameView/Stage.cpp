#include "../../includes/GameView/Stage.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Stage::Stage(const MapView& map, const Player& player,
             const CharacterContainer& characters,
             const CreatureContainer& creatures)
    : g_map(map),
      g_player(player),
      g_characters(characters),
      g_creatures(creatures) {}

void Stage::render() const {
    SDL_Point player_pos = g_player.getPos();

    // Renderizamos el piso
    g_map.renderGround();

    // Algoritmo de renderización por filas
    int min_row, max_row;
    g_map.getRowRange(min_row, max_row);
    std::list<InstanceId> units_to_render;

    for (int row = min_row; row < max_row; row++) {
        g_map.renderRow(row, units_to_render);

        for (auto it = units_to_render.begin(); it != units_to_render.end();) {
            g_player.render(*it);
            g_characters.render(*it);
            g_creatures.render(*it);
            it = units_to_render.erase(it);
        }
    }

    if (!g_map.indoor(player_pos.x, player_pos.y)) {
        g_map.renderRoofs();
    } else {
        g_map.renderShadowOutdoor();
    }
}

Stage::~Stage() {}

//-----------------------------------------------------------------------------
