#include "../../includes/View/Stage.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Stage::Stage(const HUDProxy& hud, const MapView& map, const Player& player,
             const UnitContainer& units)
    : g_hud(hud), g_map(map), g_player(player), g_units(units) {}

void Stage::render() const {
    SDL_Rect player_pos = g_player.getPos();

    /* Renderizamos el piso */
    g_map.renderGround();

    /* Implementar algoritmo de renderizado de decoración entrelazada con el
     * jugador */

    {  // Algoritmo de renderización por partes
        int map_rows = g_map.heightInTiles();
        for (int row = 0; row < map_rows; row++) {
            g_map.renderDecoration(row);

            /* renderizar unidades que estén en esta misma fila */

            if (player_pos.y == row) {
                g_player.render();
            }
        }
    }

    /* Renderizamos los techos si es necesario */
    if (!(g_map.indoor(player_pos.x, player_pos.y))) {
        g_map.renderRoofs();
    }

    g_hud.render();
}

Stage::~Stage() {}

//-----------------------------------------------------------------------------
