#include "../../includes/View/MapView.h"

//-----------------------------------------------------------------------------
// Métodos privados

SDL_Rect MapView::_getRenderQuad(const Texture& texture, const int x_tile,
                                 const int y_tile, const int tile_w,
                                 const int tile_h) const {
    /* Arreglamos el sistema de coordenadas de Tiled */
    SDL_Rect render_quad = {0};
    render_quad.x = x_tile * tile_w;
    render_quad.y = y_tile * tile_h;
    render_quad.w = texture.getWidth();
    render_quad.h = texture.getHeight();

    /* El sistema de coordenadas de Tiled es LeftBottom y el nuestro es LeftUp,
     * hay que invertir solo la Y */
    render_quad.y += tile_h - render_quad.h;

    return render_quad;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapView::MapView(const Renderer* renderer)
    : g_renderer(renderer),
      tiles(renderer),
      current_map_id(-1),
      w_tiles(0),
      h_tiles(0),
      tile_w(0),
      tile_h(0) {}

void MapView::loadMedia() {
    tiles.loadTiles();
    maps.loadMaps();
}

void MapView::select(Id id) {
    if (current_map_id != id) {
        current_map_id = id;
        const Map& current_map = maps[current_map_id];
        w_tiles = current_map.getWidthTiles();
        h_tiles = current_map.getHeightTiles();
        tile_w = current_map.getTileWidth();
        tile_h = current_map.getTileHeight();
    }
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
                render_quad = _getRenderQuad(texture, x, y, tile_w, tile_h);
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
                render_quad = _getRenderQuad(texture, x, y, tile_w, tile_h);
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
            render_quad = _getRenderQuad(texture, x, row, tile_w, tile_h);
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
                render_quad = _getRenderQuad(texture, x, y, tile_w, tile_h);
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
                render_quad = {(x * tile_w), (y * tile_h), tile_w, tile_h};
                float alpha = 0.7;
                g_renderer->fillQuadIfVisible(&render_quad, 0, 0, 0,
                                              alpha * 256);
            }
        }
    }
}

bool MapView::isValid(const int x, const int y) const {
    const Map& current_map = maps[current_map_id];
    return current_map.isValid(x, y);
}

bool MapView::indoor(const int x, const int y) const {
    const Map& current_map = maps[current_map_id];
    const Tile& current_tile = current_map.getTile(x, y);
    return current_tile.indoor;
}

bool MapView::collision(const int x, const int y) const {
    const Map& current_map = maps[current_map_id];
    const Tile& current_tile = current_map.getTile(x, y);
    return current_tile.collision;
}

int MapView::widthInTiles() const {
    return w_tiles;
}

int MapView::heightInTiles() const {
    return h_tiles;
}

int MapView::widthInPx() const {
    return w_tiles * tile_w;
}

int MapView::heightInPx() const {
    return h_tiles * tile_h;
}

MapView::~MapView() {}

//-----------------------------------------------------------------------------
