#include "../includes/Map.h"

#include "../includes/RandomNumberGenerator.h"
//-----------------------------------------------------------------------------
// Métodos privados

void Map::_checkIfValid(const json& map) const {
    if ((tile_w != map["tilewidth"]) || (tile_h != map["tileheight"])) {
        throw Exception("Invalid map file. Tiles must be %i x %i.", tile_w,
                        tile_h);
    }

    if (map["layers"].size() != EXPECTED_LAYERS) {
        throw Exception(
            "Invalid map file. File must contain these map layers: ground1, "
            "ground2, decoration, roof, safezones, collision, indoor, and "
            "portal layer.");
    }

    for (int i = 0; i < EXPECTED_LAYERS; i++) {
        if ((map["layers"][i]["width"] != w) ||
            (map["layers"][i]["height"] != h)) {
            throw Exception(
                "Invalid map file. All layers must have the same dimensions.");
        }
    }
}

void Map::_fillTiles(const json& map, const json& tilesets) {
    int total_tiles = w * h;
    tiles.reserve(total_tiles);
    Tile tile;
    for (int i = 0; i < total_tiles; i++) {
        /* Ids gráficos */
        tile.ground_1_id = map["layers"][GROUND1_LAYER]["data"][i];
        tile.ground_2_id = map["layers"][GROUND2_LAYER]["data"][i];
        tile.decoration_id = map["layers"][DECORATION_LAYER]["data"][i];
        tile.roof_id = map["layers"][ROOF_LAYER]["data"][i];
        tile.npc_id = map["layers"][NPC_LAYER]["data"][i];

        /* Metadata */
        int safe_zone = map["layers"][SAFEZONE_LAYER]["data"][i];
        tile.safe_zone = (bool)safe_zone;
        int collision = map["layers"][COLLISION_LAYER]["data"][i];
        tile.collision = (bool)collision;
        int indoor = map["layers"][INDOOR_LAYER]["data"][i];
        tile.indoor = (bool)indoor;
        int portal = map["layers"][PORTAL_LAYER]["data"][i];
        tile.portal = (bool)portal;

        /* NPC */
        if (tile.npc_id > 0) {
            /* Debemos calcular el id global del npc */
            int npc_gid = tile.npc_id;
            int tileset_first_gid = tilesets["npcs"]["first_gid"];
            int npc_lid = npc_gid - tileset_first_gid;
            std::string tileset_filepath = tilesets["npcs"]["filepath"];
            json tileset = JSON::loadJsonFile(tileset_filepath);
            tile.npc = tileset["tiles"][npc_lid]["properties"][0]["value"];

        } else {
            tile.npc = 0;
        }

        /* Ocupantes */
        tile.occupant_id = 0;

        /* esto por ahora lo saque porque no se si es necesario -mau */
        // tile.item_id = 0;

        tiles.push_back(tile);
    }
}

int Map::_tileNumber(const int x, const int y) const {
    if (!isValid(x, y)) {
        throw Exception("Invalid map coordinates. x = %i, y = %i\n", x, y);
    }
    return (y * w + x);
}

Tile& Map::_getTile(const int x, const int y) {
    int tile = _tileNumber(x, y);
    return tiles.at(tile);
}

const bool Map::_moveOcuppant(Tile& from_tile, Tile& to_tile) {
    if (to_tile.collision || to_tile.occupant_id || to_tile.npc_id) {
        // El tile está ocupado / hay colisión.
        return false;
    }

    // Se puede mover.
    to_tile.occupant_id = from_tile.occupant_id;
    to_tile.collision = true;

    from_tile.occupant_id = 0;
    from_tile.collision = false;

    return true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Map::Map() : w(0), h(0), tile_w(0), tile_h(0) {}

void Map::init(const json& map, const json& tilesets, const int tile_w,
               const int tile_h) {
    w = map["width"];
    h = map["height"];
    this->tile_w = tile_w;
    this->tile_h = tile_h;

    _checkIfValid(map);
    _fillTiles(map, tilesets);
}

Map::Map(Map&& other) {
    this->w = other.w;
    this->h = other.h;
    this->tile_w = other.tile_w;
    this->tile_h = other.tile_h;
    this->tiles = std::move(other.tiles);
}

Map& Map::operator=(Map&& other) {
    this->w = other.w;
    this->h = other.h;
    this->tile_w = other.tile_w;
    this->tile_h = other.tile_h;
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

const bool Map::moveOcuppant(const int x, const int y,
                             const Orientation& orientation) {
    Tile& from_tile = _getTile(x, y);

    if (orientation == UP_ORIENTATION) {
        if (y == 0) {
            // Limite superior alcanzado.
            return false;
        }
        Tile& to_tile = _getTile(x, y - 1);
        return _moveOcuppant(from_tile, to_tile);
    }

    if (orientation == DOWN_ORIENTATION) {
        if (y + 1 == this->h) {
            // Limite inferior alcanzado.
            return false;
        }
        Tile& to_tile = _getTile(x, y + 1);
        return _moveOcuppant(from_tile, to_tile);
    }

    if (orientation == LEFT_ORIENTATION) {
        if (x == 0) {
            // Limite izquierdo alcanzado.
            return false;
        }
        Tile& to_tile = _getTile(x - 1, y);
        return _moveOcuppant(from_tile, to_tile);
    }

    if (x + 1 == this->w) {
        // Limite derecho alcanzado.
        return false;
    }
    Tile& to_tile = _getTile(x + 1, y);
    return _moveOcuppant(from_tile, to_tile);
}

void Map::establishCharacterSpawningPosition(InstanceId id, int& x, int& y) {
    bool valid_position = false;
    RandomNumberGenerator gen;
    while (!valid_position) {
        x = gen(0, this->w - 1);
        y = gen(0, this->h - 1);

        if (!this->getTile(x, y).collision)
            valid_position = true;
    }
    Tile& tile = this->_getTile(x, y);
    tile.collision = true;
    tile.occupant_id = id;
}

Map::~Map() {}

//-----------------------------------------------------------------------------
