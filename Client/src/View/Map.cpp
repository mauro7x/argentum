#include "../../includes/View/Map.h"

//-----------------------------------------------------------------------------
// Métodos privados

void MapProxy::_createTextures() {
    tile_textures.reserve(TILE_TEXTURES);
    for (int i = 0; i < TILE_TEXTURES; i++) {
        tile_textures.push_back(Texture());
    }
    tile_textures.shrink_to_fit();
}

void MapProxy::_loadTextures() {
    std::string filepath;
    for (int i = 0; i < TILE_TEXTURES; i++) {
        filepath = "../Assets/Tiles/" + std::to_string(i) + ".png";
        tile_textures[i].loadFromFile(g_renderer, filepath);
    }
}

void MapProxy::_loadTiles() {
    std::ifstream map("../Assets/Maps/test.map");
    if (map.fail()) {
        throw SDLException("Error in opening map file.\nSDL_Error: %s",
                           SDL_GetError());
    }

    // Posicion en el mapa
    int x = 0;
    int y = 0;

    for (int i = 0; i < MAP_TOTAL_TILES; i++) {
        // Determines what kind of tile will be made
        int tile_type = -1;

        // Read tile from map file
        map >> tile_type;
        if (map.fail()) {
            map.close();
            throw SDLException("Error in reading map file.\nSDL_Error: %s",
                               SDL_GetError());
        }

        // If the number is a valid tile number
        if ((tile_type < 0) || (tile_type >= TILE_TEXTURES)) {
            map.close();
            throw SDLException(
                "Error in parsing map: unknown tile type.\nSDL_Error: %s",
                SDL_GetError());
        }

        tiles.push_back(
            Tile({{x, y, TILE_WIDTH, TILE_HEIGHT}, &tile_textures[tile_type]}));

        x += TILE_WIDTH;

        if (x >= MAP_WIDTH) {
            x = 0;
            y += TILE_HEIGHT;
        }
    }

    tiles.shrink_to_fit();
}

bool MapProxy::_checkCollision(const SDL_Rect& a, const SDL_Rect& b) const {
    // The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    // If any of the sides from A are outside of B
    if (bottomA <= topB) {
        return false;
    }

    if (topA >= bottomB) {
        return false;
    }

    if (rightA <= leftB) {
        return false;
    }

    if (leftA >= rightB) {
        return false;
    }

    // If none of the sides from A are outside B
    return true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapProxy::MapProxy(const Renderer* renderer) : g_renderer(renderer) {
    _createTextures();
    tiles.reserve(MAP_TOTAL_TILES);
}

void MapProxy::loadMedia() {
    _loadTextures();
    _loadTiles();
}

void MapProxy::render(const SDL_Rect& camera) const {
    for (int i = 0; i < MAP_TOTAL_TILES; i++) {
        SDL_Rect render_quad = tiles[i].dim;

        if (_checkCollision(render_quad, camera)) {
            render_quad.x += SCREEN_X_OFFSET - camera.x;
            render_quad.y += SCREEN_Y_OFFSET - camera.y;
            SDL_Texture* texture = (tiles[i].texture)->getTexture();
            g_renderer->render(texture, &render_quad);
        }
    }
}

MapProxy::~MapProxy() {}

//-----------------------------------------------------------------------------
