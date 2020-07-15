#include "../../includes/GameView/MapView.h"

//-----------------------------------------------------------------------------
// Métodos privados

SDL_Rect MapView::_getRenderQuad(const Texture& texture, const int x_tile,
                                 const int y_tile) const {
    /* Arreglamos el sistema de coordenadas de Tiled */
    SDL_Rect render_quad = {0};
    render_quad.x = x_tile * TILE_WIDTH;
    render_quad.y = y_tile * TILE_HEIGHT;
    render_quad.w = texture.getWidth();
    render_quad.h = texture.getHeight();

    /* El sistema de coordenadas de Tiled es LeftBottom y el nuestro es LeftUp,
     * hay que invertir solo la Y */
    render_quad.y += TILE_HEIGHT - render_quad.h;

    return render_quad;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapView::MapView(const Renderer* renderer, const Camera& camera,
                 const ItemSpriteContainer& item_sprites)
    : g_renderer(renderer),
      g_camera(camera),
      tiles(renderer),
      item_sprites(item_sprites) {}

void MapView::loadMedia() {
    tiles.loadMedia();
    maps.loadMaps();
}

void MapView::setRenderArea() {
    // Límites de renderizado

    g_camera.getVisibleRect(x_min, x_max, y_min, y_max);
    x_min = (x_min / TILE_WIDTH) - rendering_x_tiles_padding;
    x_max = (x_max / TILE_WIDTH) + rendering_x_tiles_padding;
    y_min = (y_min / TILE_HEIGHT) - rendering_y_tiles_padding;
    y_max = (y_max / TILE_HEIGHT) + rendering_y_tiles_padding;

    if (x_min < 0)
        x_min = 0;

    if (x_max >= w_tiles)
        x_max = w_tiles;

    if (y_min < 0)
        y_min = 0;

    if (y_max >= h_tiles)
        y_max = h_tiles;
}

bool MapView::selectMap(const Id id) {
    if ((current_map_id == id) && (current_map != NULL)) {
        return false;
    }

    current_map_id = id;
    this->current_map = &maps[current_map_id];
    w_tiles = current_map->getWidthTiles();
    h_tiles = current_map->getHeightTiles();
    return true;
}

void MapView::selectNPC(const int x_tile, const int y_tile) {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    TileId& npc_id = current_map->getNPC(x_tile, y_tile);
    npc_id++;
}

void MapView::clearSelectedNPC(const int x_tile, const int y_tile) {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    TileId& npc_id = current_map->getNPC(x_tile, y_tile);
    npc_id--;
}

void MapView::selectPortal(const int x_tile, const int y_tile) {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    TileId& portal = current_map->getPortal(x_tile, y_tile);
    portal++;
}

void MapView::clearSelectedPortal(const int x_tile, const int y_tile) {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    TileId& portal = current_map->getPortal(x_tile, y_tile);
    portal--;
}

void MapView::occupyTile(InstanceId id, const int x_tile, const int y_tile) {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    current_map->occupyTile(id, x_tile, y_tile);
}

void MapView::addItem(Id id, uint32_t amount, const int x_tile,
                      const int y_tile) {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    current_map->setItem(id, amount, x_tile, y_tile);
}

void MapView::clearTileOccupant(const int x_tile, const int y_tile) {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    current_map->clearTileOccupant(x_tile, y_tile);
}

void MapView::clearTileItem(const int x_tile, const int y_tile) {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    current_map->clearTileItem(x_tile, y_tile);
}

void MapView::clear() {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    current_map->clear();
}

void MapView::renderGround() const {
    SDL_Rect render_quad = {0};

    /* Primera capa */
    for (int y = y_min; y < y_max; y++) {
        for (int x = x_min; x < x_max; x++) {
            const Tile& current_tile = current_map->getTileWithoutChecks(x, y);

            if (current_tile.ground_1_id) {
                const Texture& texture = tiles[current_tile.ground_1_id];
                render_quad = _getRenderQuad(texture, x, y);
                g_camera.renderAddingOffset(g_renderer, texture.getTexture(),
                                            render_quad);
            }
        }
    }

    /* Segunda capa */
    for (int y = y_min; y < y_max; y++) {
        for (int x = x_min; x < x_max; x++) {
            const Tile& current_tile = current_map->getTileWithoutChecks(x, y);

            if (current_tile.ground_2_id) {
                const Texture& texture = tiles[current_tile.ground_2_id];
                render_quad = _getRenderQuad(texture, x, y);
                g_camera.renderAddingOffset(g_renderer, texture.getTexture(),
                                            render_quad);
            }
        }
    }
}

void MapView::renderRow(const int row,
                        std::list<InstanceId>& units_to_render) const {
    SDL_Rect render_quad = {0};

    for (int x = x_min; x < x_max; x++) {
        const Tile& current_tile = current_map->getTileWithoutChecks(x, row);

        // Decoración
        if (current_tile.decoration_id) {
            const Texture& texture = tiles[current_tile.decoration_id];
            render_quad = _getRenderQuad(texture, x, row);
            g_camera.renderAddingOffset(g_renderer, texture.getTexture(),
                                        render_quad);
        }

        // NPCs
        if (current_tile.npc_id) {
            const Texture& texture = tiles[current_tile.npc_id];
            render_quad = _getRenderQuad(texture, x, row);
            g_camera.renderAddingOffset(g_renderer, texture.getTexture(),
                                        render_quad);
        }

        // Items
        if (current_tile.item_id) {
            const Texture& texture = item_sprites[current_tile.item_id].texture;
            render_quad = _getRenderQuad(texture, x, row);
            g_camera.renderAddingOffset(g_renderer, texture.getTexture(),
                                        render_quad);
        }

        // Ocupantes
        if (current_tile.occupant_id) {
            units_to_render.emplace_back(current_tile.occupant_id);
        }
    }
}

void MapView::renderTop(bool indoor) const {
    SDL_Rect render_quad = {0};

    if (!indoor) {
        for (int y = y_min; y < y_max; y++) {
            for (int x = x_min; x < x_max; x++) {
                const Tile& current_tile =
                    current_map->getTileWithoutChecks(x, y);

                if (current_tile.roof_id) {
                    const Texture& texture = tiles[current_tile.roof_id];
                    render_quad = _getRenderQuad(texture, x, y);
                    g_camera.renderAddingOffset(
                        g_renderer, texture.getTexture(), render_quad);
                }
            }
        }
    } else {
        for (int y = y_min; y < y_max; y++) {
            for (int x = x_min; x < x_max; x++) {
                const Tile& current_tile =
                    current_map->getTileWithoutChecks(x, y);

                if (!(current_tile.indoor)) {
                    /* Renderizar una textura negra */
                    render_quad = {(x * TILE_WIDTH), (y * TILE_HEIGHT),
                                   TILE_WIDTH, TILE_HEIGHT};
                    g_camera.fillQuadAddingOffset(g_renderer, render_quad);
                }
            }
        }
    }
}

InstanceId MapView::getOccupant(const int x, const int y) const {
    return current_map->getTileWithoutChecks(x, y).occupant_id;
}

Id MapView::getNPC(const int x, const int y) const {
    return current_map->getTileWithoutChecks(x, y).npc;
}

bool MapView::indoor(const int x, const int y) const {
    return current_map->getTileWithoutChecks(x, y).indoor;
}

bool MapView::portal(const int x, const int y) const {
    return current_map->getTileWithoutChecks(x, y).portal;
}

void MapView::getRowRange(int& row_min, int& row_max) const {
    row_min = y_min;
    row_max = y_max;
}

int MapView::widthInPx() const {
    return w_tiles * TILE_WIDTH;
}

int MapView::heightInPx() const {
    return h_tiles * TILE_HEIGHT;
}

MapView::~MapView() {}

//-----------------------------------------------------------------------------
