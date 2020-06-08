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

void MapView::_renderGround() const {
    const Map& current_map = maps[current_map_id];
    int x_tiles = current_map.getWidthTiles();
    int y_tiles = current_map.getHeightTiles();
    int tile_w = current_map.getTileWidth();
    int tile_h = current_map.getTileHeight();

    SDL_Rect render_quad = {0};

    for (int y = 0; y < y_tiles; y++) {
        for (int x = 0; x < x_tiles; x++) {
            const Tile& current_tile = current_map.getTile(x, y);

            if (current_tile.ground_id) {
                const Texture& texture = tiles[current_tile.ground_id];
                render_quad = _getRenderQuad(texture, x, y, tile_w, tile_h);
                g_renderer->renderIfVisible(texture.getTexture(), &render_quad);
            }
        }
    }
}

void MapView::_renderDecoration() const {
    const Map& current_map = maps[current_map_id];
    int x_tiles = current_map.getWidthTiles();
    int y_tiles = current_map.getHeightTiles();
    int tile_w = current_map.getTileWidth();
    int tile_h = current_map.getTileHeight();

    SDL_Rect render_quad = {0};

    for (int y = 0; y < y_tiles; y++) {
        for (int x = 0; x < x_tiles; x++) {
            const Tile& current_tile = current_map.getTile(x, y);

            if (current_tile.decoration_id) {
                const Texture& texture = tiles[current_tile.decoration_id];
                render_quad = _getRenderQuad(texture, x, y, tile_w, tile_h);
                g_renderer->renderIfVisible(texture.getTexture(), &render_quad);
            }
        }
    }
}

void MapView::_renderRoofs(const int x_player, const int y_player) const {
    const Map& current_map = maps[current_map_id];

    if (!(current_map.getTile(x_player, y_player).indoor)) {
        int x_tiles = current_map.getWidthTiles();
        int y_tiles = current_map.getHeightTiles();
        int tile_w = current_map.getTileWidth();
        int tile_h = current_map.getTileHeight();

        SDL_Rect render_quad = {0};

        for (int y = 0; y < y_tiles; y++) {
            for (int x = 0; x < x_tiles; x++) {
                const Tile& current_tile = current_map.getTile(x, y);

                if (current_tile.roof_id) {
                    const Texture& texture = tiles[current_tile.roof_id];
                    render_quad = _getRenderQuad(texture, x, y, tile_w, tile_h);
                    g_renderer->renderIfVisible(texture.getTexture(),
                                                &render_quad);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapView::MapView(const Renderer* renderer)
    : g_renderer(renderer), tiles(renderer), current_map_id(0) {}

void MapView::loadMedia() {
    tiles.loadTiles();
    maps.loadMaps();
}

void MapView::select(Id id) {
    if (current_map_id != id) {
        current_map_id = id;
    }
}

bool MapView::collision(const int x, const int y) const {
    const Map& current_map = maps[current_map_id];
    const Tile& current_tile = current_map.getTile(x, y);
    return current_tile.collision;
}

int MapView::getWidth() const {
    const Map& current_map = maps[current_map_id];
    return (current_map.getTileWidth()) * (current_map.getWidthTiles());
}

int MapView::getHeight() const {
    const Map& current_map = maps[current_map_id];
    return (current_map.getTileHeight()) * (current_map.getHeightTiles());
}

void MapView::render(const int x_player, const int y_player) const {
    /* Primero el piso */
    _renderGround();

    /* Luego las decoraciones */
    _renderDecoration();

    /* Por último, si corresponde, los techos */
    _renderRoofs(x_player, y_player);
}

MapView::~MapView() {}

//-----------------------------------------------------------------------------
