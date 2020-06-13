#include "../../includes/View/Stage.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Stage::Stage(const HUDProxy& hud, const MapView& map, const Player& player,
             const UnitContainer<Character, CharacterData>& characters,
             const UnitContainer<Creature, CreatureData>& creatures)
    : g_hud(hud),
      g_map(map),
      g_player(player),
      g_characters(characters),
      g_creatures(creatures) {}

void Stage::render() const {
    SDL_Rect player_pos = g_player.getPos();

    /* Renderizamos el piso */
    g_map.renderGround();

    {  // Algoritmo de renderización por filas
        int map_cols = g_map.widthInTiles();
        int map_rows = g_map.heightInTiles();
        int unit_id;

        for (int row = 0; row < map_rows; row++) {
            g_map.renderDecoration(row);
            g_map.renderNPCs(row);

            for (int col = 0; col < map_cols; col++) {
                unit_id = g_map.getOccupant(col, row);
                if (unit_id) {
                    g_characters.render(unit_id);
                    g_creatures.render(unit_id);
                }

                if (player_pos.x == col && player_pos.y == row) {
                    g_player.render();
                }
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
