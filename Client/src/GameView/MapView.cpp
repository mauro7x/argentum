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

void MapView::occupyTile(InstanceId id, const int x_tile, const int y_tile) {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    current_map->occupyTile(id, x_tile, y_tile);
}

void MapView::addItem(Id id, const int x_tile, const int y_tile) {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    current_map->addItem(id, x_tile, y_tile);
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
    int w_tiles = current_map->getWidthTiles();
    int h_tiles = current_map->getHeightTiles();

    for (int y = 0; y < h_tiles; y++) {
        for (int x = 0; x < w_tiles; x++) {
            current_map->clearTileItem(x, y);
            current_map->clearTileOccupant(x, y);
        }
    }
}

void MapView::renderGround() const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    SDL_Rect render_quad = {0};

    /* Primera capa */
    for (int y = 0; y < h_tiles; y++) {
        for (int x = 0; x < w_tiles; x++) {
            const Tile& current_tile = current_map->getTile(x, y);

            if (current_tile.ground_1_id) {
                const Texture& texture = tiles[current_tile.ground_1_id];
                render_quad = _getRenderQuad(texture, x, y);
                g_camera.renderIfVisible(g_renderer, texture.getTexture(),
                                         render_quad);
            }
        }
    }

    /* Segunda capa */
    for (int y = 0; y < h_tiles; y++) {
        for (int x = 0; x < w_tiles; x++) {
            const Tile& current_tile = current_map->getTile(x, y);

            if (current_tile.ground_2_id) {
                const Texture& texture = tiles[current_tile.ground_2_id];
                render_quad = _getRenderQuad(texture, x, y);
                g_camera.renderIfVisible(g_renderer, texture.getTexture(),
                                         render_quad);
            }
        }
    }
}

void MapView::renderRow(const int row) const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    SDL_Rect render_quad = {0};

    for (int x = 0; x < w_tiles; x++) {
        const Tile& current_tile = current_map->getTile(x, row);

        // Decoración
        if (current_tile.decoration_id) {
            const Texture& texture = tiles[current_tile.decoration_id];
            render_quad = _getRenderQuad(texture, x, row);
            g_camera.renderIfVisible(g_renderer, texture.getTexture(),
                                     render_quad);
        }

        // NPCs
        if (current_tile.npc_id) {
            const Texture& texture = tiles[current_tile.npc_id];
            render_quad = _getRenderQuad(texture, x, row);
            g_camera.renderIfVisible(g_renderer, texture.getTexture(),
                                     render_quad);
        }

        // Items
        if (current_tile.item_id) {
            const Texture& texture = item_sprites[current_tile.item_id].texture;
            render_quad = _getRenderQuad(texture, x, row);
            g_camera.renderIfVisible(g_renderer, texture.getTexture(),
                                     render_quad);
        }
    }
}

void MapView::renderRoofs() const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    SDL_Rect render_quad = {0};

    for (int y = 0; y < h_tiles; y++) {
        for (int x = 0; x < w_tiles; x++) {
            const Tile& current_tile = current_map->getTile(x, y);

            if (current_tile.roof_id) {
                const Texture& texture = tiles[current_tile.roof_id];
                render_quad = _getRenderQuad(texture, x, y);
                g_camera.renderIfVisible(g_renderer, texture.getTexture(),
                                         render_quad);
            }
        }
    }
}

void MapView::renderShadowOutdoor() const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    SDL_Rect render_quad = {0};

    for (int y = 0; y < h_tiles; y++) {
        for (int x = 0; x < w_tiles; x++) {
            const Tile& current_tile = current_map->getTile(x, y);

            if (!(current_tile.indoor)) {
                /* Renderizar una textura negra */
                render_quad = {(x * TILE_WIDTH), (y * TILE_HEIGHT), TILE_WIDTH,
                               TILE_HEIGHT};
                g_camera.fillQuadIfVisible(g_renderer, render_quad);
            }
        }
    }
}

InstanceId MapView::getOccupant(const int x, const int y) const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    const Tile& current_tile = current_map->getTile(x, y);
    return current_tile.occupant_id;
}

Id MapView::getNPC(const int x, const int y) const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    const Tile& current_tile = current_map->getTile(x, y);
    return current_tile.npc;
}

bool MapView::indoor(const int x, const int y) const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    const Tile& current_tile = current_map->getTile(x, y);
    return current_tile.indoor;
}

int MapView::widthInTiles() const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    return w_tiles;
}

int MapView::heightInTiles() const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    return h_tiles;
}

int MapView::widthInPx() const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    return w_tiles * TILE_WIDTH;
}

int MapView::heightInPx() const {
    if (!current_map) {
        throw Exception("MapView:: current_map is NULL!");
    }

    return h_tiles * TILE_HEIGHT;
}

MapView::~MapView() {}

//-----------------------------------------------------------------------------
