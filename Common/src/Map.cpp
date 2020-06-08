#include "../includes/Map.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Map::_checkIfValid(const json& map_layers) const {
    if (map_layers.size() != EXPECTED_LAYERS) {
        throw Exception(
            "Invalid map file. File must contain these map layers: ground1, "
            "ground2, decoration, roof, safezones, collision, indoor, and "
            "portal layer.");
    }

    for (int i = 0; i < EXPECTED_LAYERS; i++) {
        if ((map_layers[i]["width"] != w) || (map_layers[i]["height"] != h)) {
            throw Exception(
                "Invalid map file. All layers must have the same dimensions.");
        }
    }
}

void Map::_fillTiles(const json& map_layers) {
    _checkIfValid(map_layers);

    int total_tiles = w * h;
    tiles.reserve(total_tiles);
    Tile tile;
    for (int i = 0; i < total_tiles; i++) {
        tile.ground_1_id = map_layers[GROUND1]["data"][i];
        tile.ground_2_id = map_layers[GROUND2]["data"][i];
        tile.decoration_id = map_layers[DECORATION]["data"][i];
        tile.roof_id = map_layers[ROOF]["data"][i];
        int safe_zone = map_layers[SAFEZONE]["data"][i];
        tile.safe_zone = (bool)safe_zone;
        int collision = map_layers[COLLISION]["data"][i];
        tile.collision = (bool)collision;
        int indoor = map_layers[INDOOR]["data"][i];
        tile.indoor = (bool)indoor;
        int portal = map_layers[PORTAL]["data"][i];
        tile.portal = (bool)portal;
        tiles.push_back(tile);
    }
}

int Map::_tileNumber(const int x, const int y) const {
    if ((x >= w) || (x < 0) || (y >= h) || (y < 0)) {
        throw Exception("Invalid map coordinates.");
    }
    return (y * w + x);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Map::Map() : w(0), h(0), tile_w(0), tile_h(0) {}

void Map::init(const json& j_map, const int tile_w, const int tile_h) {
    w = j_map["width"];
    h = j_map["height"];
    this->tile_w = tile_w;
    this->tile_h = tile_h;

    if ((tile_w != j_map["tilewidth"]) || (tile_h != j_map["tileheight"])) {
        throw Exception("Invalid map file. Tiles must be %i x %i.", tile_w,
                        tile_h);
    }

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

int Map::getWidthTiles() const {
    return w;
}

int Map::getHeightTiles() const {
    return h;
}

int Map::getTileWidth() const {
    return tile_w;
}

int Map::getTileHeight() const {
    return tile_h;
}

bool Map::isValid(const int x, const int y) const {
    if ((x >= w) || (x < 0) || (y >= h) || (y < 0)) {
        return false;
    } else {
        return true;
    }
}

const Tile& Map::getTile(const int x, const int y) const {
    int tile = _tileNumber(x, y);
    return tiles.at(tile);
}

Map::~Map() {}

//-----------------------------------------------------------------------------
