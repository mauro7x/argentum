#include "../includes/Map.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Map::_checkIfValid(const json map_layers) const {
    if (map_layers.size() != EXPECTED_LAYERS) {
        throw Exception(
            "Invalid map file. File must contain six map layers: ground, "
            "decoration, roof, zones, safezones, and collision layer.");
    }

    for (int i = 0; i < EXPECTED_LAYERS; i++) {
        if ((map_layers[i]["width"] != w) || (map_layers[i]["height"] != h)) {
            throw Exception(
                "Invalid map file. File must contain six map layers: ground, "
                "decoration, roof, zones, safezones, and collision layer.");
        }
    }
}

void Map::_fillTiles(const json map_layers) {
    _checkIfValid(map_layers);

    int total_tiles = w * h;
    tiles.reserve(total_tiles);
    Tile tile;
    for (int i = 0; i < total_tiles; i++) {
        tile.ground_id = map_layers[GROUND]["data"][i];
        tile.decoration_id = map_layers[DECORATION]["data"][i];
        tile.roof_id = map_layers[ROOF]["data"][i];
        int safe_zone = map_layers[SAFEZONES]["data"][i];
        tile.safe_zone = (bool)safe_zone;
        int collision = map_layers[COLLISION]["data"][i];
        tile.collision = (bool)collision;
        tiles[i] = tile;
    }
}

int Map::_tileNumber(const int x, const int y) const {
    return (y * w + x);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Map::Map() : w(0), h(0), tile_w(0), tile_h(0) {}

Map::Map(const json j_map) {
    w = j_map["width"];
    h = j_map["height"];
    tile_w = j_map["tilewidth"];
    tile_h = j_map["tileheight"];
    _fillTiles(j_map["layers"]);
}

Map::Map(Map&& other) {
    this->w = std::move(other.w);
    this->h = std::move(other.h);
    this->tile_w = std::move(other.tile_w);
    this->tile_h = std::move(other.tile_h);
    this->tiles = std::move(other.tiles);
}

Map& Map::operator=(Map&& other) {
    this->w = std::move(other.w);
    this->h = std::move(other.h);
    this->tile_w = std::move(other.tile_w);
    this->tile_h = std::move(other.tile_h);
    this->tiles = std::move(other.tiles);
    return *this;
}

int Map::groundId(const int x, const int y) const {
    int tile = _tileNumber(x, y);
    if (tile > (w * x)) {
        throw Exception("Invalid coordinates to map access.");
    }
    return tiles[tile].ground_id;
}

int Map::decorationId(const int x, const int y) const {
    int tile = _tileNumber(x, y);
    if (tile > (w * x)) {
        throw Exception("Invalid coordinates to map access.");
    }
    return tiles[tile].decoration_id;
}

int Map::roofId(const int x, const int y) const {
    int tile = _tileNumber(x, y);
    if (tile > (w * x)) {
        throw Exception("Invalid coordinates to map access.");
    }
    return tiles[tile].roof_id;
}

bool Map::collision(const int x, const int y) const {
    int tile = _tileNumber(x, y);
    if (tile > (w * x)) {
        throw Exception("Invalid coordinates to map access.");
    }
    return tiles[tile].collision;
}

bool Map::isSafeZone(const int x, const int y) const {
    int tile = _tileNumber(x, y);
    if (tile > (w * x)) {
        throw Exception("Invalid coordinates to map access.");
    }
    return tiles[tile].safe_zone;
}

Map::~Map() {}

//-----------------------------------------------------------------------------
