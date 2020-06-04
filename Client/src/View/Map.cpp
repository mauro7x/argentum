#include "../../includes/View/Map.h"

//-----------------------------------------------------------------------------
// Métodos privados

void MapProxy::_createTextures(SDL_Renderer* renderer) {
    mTileTextures.reserve(TILE_TEXTURES);
    for (int i = 0; i < TILE_TEXTURES; i++) {
        mTileTextures.push_back(Texture(renderer));
    }
    mTileTextures.shrink_to_fit();
}

void MapProxy::_loadTextures() {
    std::string filepath;
    for (int i = 0; i < TILE_TEXTURES; i++) {
        filepath = "../Assets/Tiles/" + std::to_string(i) + ".png";
        mTileTextures[i].loadFromFile(filepath);
    }
}

void MapProxy::_createTiles() {
    mTiles.reserve(MAP_TOTAL_TILES);

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

        mTiles.push_back(Tile(x, y, &mTileTextures[tile_type], gRenderer));

        x += TILE_WIDTH;

        if (x >= MAP_WIDTH) {
            x = 0;
            y += TILE_HEIGHT;
        }
    }

    mTiles.shrink_to_fit();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapProxy::MapProxy(SDL_Renderer* renderer) : gRenderer(renderer) {
    _createTextures(renderer);
    _loadTextures();
    _createTiles();
}

void MapProxy::render(const SDL_Rect& camera) const {
    for (int i = 0; i < MAP_TOTAL_TILES; i++) {
        mTiles[i].render(camera);
    }
}

MapProxy::~MapProxy() {}

//-----------------------------------------------------------------------------
