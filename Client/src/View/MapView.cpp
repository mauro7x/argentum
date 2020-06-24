#include "../../includes/View/MapView.h"

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

MapView::MapView(const Renderer* renderer)
    : g_renderer(renderer), tiles(renderer) {}

void MapView::loadMedia() {
    tiles.loadMedia();
    maps.loadMaps();
}

void MapView::changeMap(const Id id) {
    current_map_id = id;
    const Map& current_map = maps[current_map_id];
    w_tiles = current_map.getWidthTiles();
    h_tiles = current_map.getHeightTiles();
}

void MapView::occupyTile(InstanceId id, const int x_tile, const int y_tile) {
    Map& current_map = maps[current_map_id];
    current_map.occupyTile(id, x_tile, y_tile);
}

void MapView::clearTile(const int x_tile, const int y_tile) {
    Map& current_map = maps[current_map_id];
    current_map.clearTile(x_tile, y_tile);
}

void MapView::renderGround() const {
    const Map& current_map = maps[current_map_id];
    SDL_Rect render_quad = {0};

    /* Primera capa */
    for (int y = 0; y < h_tiles; y++) {
        for (int x = 0; x < w_tiles; x++) {
            const Tile& current_tile = current_map.getTile(x, y);

            if (current_tile.ground_1_id) {
                const Texture& texture = tiles[current_tile.ground_1_id];
                render_quad = _getRenderQuad(texture, x, y);
                g_renderer->renderIfVisible(texture.getTexture(), &render_quad);
            }
        }
    }

    /* Segunda capa */
    for (int y = 0; y < h_tiles; y++) {
        for (int x = 0; x < w_tiles; x++) {
            const Tile& current_tile = current_map.getTile(x, y);

            if (current_tile.ground_2_id) {
                const Texture& texture = tiles[current_tile.ground_2_id];
                render_quad = _getRenderQuad(texture, x, y);
                g_renderer->renderIfVisible(texture.getTexture(), &render_quad);
            }
        }
    }
}

void MapView::renderDecoration(const int row) const {
    const Map& current_map = maps[current_map_id];
    SDL_Rect render_quad = {0};

    for (int x = 0; x < w_tiles; x++) {
        const Tile& current_tile = current_map.getTile(x, row);

        if (current_tile.decoration_id) {
            const Texture& texture = tiles[current_tile.decoration_id];
            render_quad = _getRenderQuad(texture, x, row);
            g_renderer->renderIfVisible(texture.getTexture(), &render_quad);
        }
    }
}

void MapView::renderNPCs(const int row) const {
    const Map& current_map = maps[current_map_id];
    SDL_Rect render_quad = {0};

    for (int x = 0; x < w_tiles; x++) {
        const Tile& current_tile = current_map.getTile(x, row);

        if (current_tile.npc_id) {
            const Texture& texture = tiles[current_tile.npc_id];
            render_quad = _getRenderQuad(texture, x, row);
            g_renderer->renderIfVisible(texture.getTexture(), &render_quad);
        }
    }
}

void MapView::renderRoofs() const {
    const Map& current_map = maps[current_map_id];
    SDL_Rect render_quad = {0};

    for (int y = 0; y < h_tiles; y++) {
        for (int x = 0; x < w_tiles; x++) {
            const Tile& current_tile = current_map.getTile(x, y);

            if (current_tile.roof_id) {
                const Texture& texture = tiles[current_tile.roof_id];
                render_quad = _getRenderQuad(texture, x, y);
                g_renderer->renderIfVisible(texture.getTexture(), &render_quad);
            }
        }
    }
}

void MapView::renderShadowOutdoor() const {
    const Map& current_map = maps[current_map_id];
    SDL_Rect render_quad = {0};

    for (int y = 0; y < h_tiles; y++) {
        for (int x = 0; x < w_tiles; x++) {
            const Tile& current_tile = current_map.getTile(x, y);

            if (!(current_tile.indoor)) {
                /* Renderizar una textura negra */
                render_quad = {(x * TILE_WIDTH), (y * TILE_HEIGHT), TILE_WIDTH,
                               TILE_HEIGHT};
                float alpha = 1.0;
                g_renderer->fillQuadIfVisible(&render_quad, 0, 0, 0,
                                              alpha * 255);
            }
        }
    }
}

InstanceId MapView::getOccupant(const int x, const int y) const {
    const Map& current_map = maps[current_map_id];
    const Tile& current_tile = current_map.getTile(x, y);
    return current_tile.occupant_id;
}

bool MapView::indoor(const int x, const int y) const {
    const Map& current_map = maps[current_map_id];
    const Tile& current_tile = current_map.getTile(x, y);
    return current_tile.indoor;
}

int MapView::widthInTiles() const {
    return w_tiles;
}

int MapView::heightInTiles() const {
    return h_tiles;
}

int MapView::widthInPx() const {
    return w_tiles * TILE_WIDTH;
}

int MapView::heightInPx() const {
    return h_tiles * TILE_HEIGHT;
}

MapView::~MapView() {}

//-----------------------------------------------------------------------------
