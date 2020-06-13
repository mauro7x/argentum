#include "../../includes/View/Stage.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Stage::Stage(const HUDProxy& hud, const MapView& map, const Player& player)
    : g_hud(hud), g_map(map), g_player(player) {}

void Stage::render() const {
    SDL_Rect player_pos = g_player.getPos();

    /* Renderizamos el piso */
    g_map.renderGround();

    {  // Algoritmo de renderización por partes
        int map_rows = g_map.heightInTiles();
        for (int row = 0; row < map_rows; row++) {
            g_map.renderDecoration(row);

            /* renderizar unidades que estén en esta misma fila */

            /* esto se puede hacer mucho mas eficientemente si en vez de tener
             * que preguntarle a las unidades si estan en una determinada fila,
             * lo sabemos de antes. haciendo que el mapa tenga una capa de ids
             * mapeados a posiciones matamos esto también*/

            if (player_pos.y == row) {
                g_player.render();
            }
        }
    }

    /* Renderizamos los techos/sombras si es necesario */
    if (g_map.indoor(player_pos.x, player_pos.y)) {
        g_map.renderShadowOutdoor(); /* estamos adentro de una construcción */
    } else {
        g_map.renderRoofs(); /* estamos afuera de las construcciones */
    }

    g_hud.render();
}

Stage::~Stage() {}

//-----------------------------------------------------------------------------
