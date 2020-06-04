#include "../../includes/View/Map.h"

//-----------------------------------------------------------------------------
// Métodos privados

void MapProxy::_createTextures(SDL_Renderer* renderer) {
    tile_textures.reserve(TILE_TEXTURES);
    for (int i = 0; i < TILE_TEXTURES; i++) {
        tile_textures.push_back(Texture(renderer));
    }
    tile_textures.shrink_to_fit();
}

void MapProxy::_loadTextures() {
    std::string filepath;
    for (int i = 0; i < TILE_TEXTURES; i++) {
        filepath = "../Assets/Tiles/" + std::to_string(i) + ".png";
        tile_textures[i].loadFromFile(filepath);
    }
}

void MapProxy::_createTiles() {
    tiles.reserve(MAP_TOTAL_TILES);

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

        tiles.push_back(Tile(x, y, &tile_textures[tile_type], g_renderer));

        x += TILE_WIDTH;

        if (x >= MAP_WIDTH) {
            x = 0;
            y += TILE_HEIGHT;
        }
    }

    tiles.shrink_to_fit();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapProxy::MapProxy(SDL_Renderer* renderer) : g_renderer(renderer) {
    _createTextures(renderer);
    _loadTextures();
    _createTiles();
}

void MapProxy::render(const SDL_Rect& camera) const {
    for (int i = 0; i < MAP_TOTAL_TILES; i++) {
        tiles[i].render(camera);
    }
}

MapProxy::~MapProxy() {}

//-----------------------------------------------------------------------------
