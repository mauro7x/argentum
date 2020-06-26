#include "../includes/Map.h"

#include "../includes/RandomNumberGenerator.h"
//-----------------------------------------------------------------------------
// Métodos privados

void Map::_checkIfValid(const json& map) const {
    if ((TILE_WIDTH != map["tilewidth"]) ||
        (TILE_HEIGHT != map["tileheight"])) {
        throw Exception("Invalid map file. Tiles must be %i x %i.", TILE_WIDTH,
                        TILE_HEIGHT);
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

        tile.item_id = 0;

        tiles.push_back(tile);
    }
}

int Map::_tileNumber(const int x, const int y) const {
    if (!_isValid(x, y)) {
        throw Exception("Invalid map coordinates. x = %i, y = %i\n", x, y);
    }
    return (y * w + x);
}

Tile& Map::_getTile(const int x, const int y) {
    int tile = _tileNumber(x, y);
    return tiles.at(tile);
}

const bool Map::_moveOccupant(Tile& from_tile, Tile& to_tile) {
    if (to_tile.collision || to_tile.occupant_id || to_tile.npc_id) {
        // El tile está ocupado / hay colisión.
        return false;
    }

    // Se puede mover.
    to_tile.occupant_id = from_tile.occupant_id;
    // to_tile.collision = true;

    from_tile.occupant_id = 0;
    // from_tile.collision = false;

    return true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Map::Map() {}

void Map::init(const json& map, const json& tilesets) {
    w = map["width"];
    h = map["height"];

    _checkIfValid(map);
    _fillTiles(map, tilesets);
}

Map::Map(Map&& other) {
    this->w = other.w;
    this->h = other.h;
    this->tiles = std::move(other.tiles);
}

Map& Map::operator=(Map&& other) {
    this->w = other.w;
    this->h = other.h;
    this->tiles = std::move(other.tiles);
    return *this;
}

int Map::getWidthTiles() const {
    return w;
}

int Map::getHeightTiles() const {
    return h;
}

bool Map::_isValid(const int x, const int y) const {
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

TileId& Map::getNPC(const int x, const int y) {
    Tile& tile = _getTile(x, y);
    return tile.npc_id;
}

const bool Map::moveOccupant(const int x, const int y,
                             const Orientation& orientation) {
    Tile& from_tile = _getTile(x, y);

    if (orientation == UP_ORIENTATION) {
        if (y == 0) {
            // Limite superior alcanzado.
            return false;
        }
        Tile& to_tile = _getTile(x, y - 1);
        return _moveOccupant(from_tile, to_tile);
    }

    if (orientation == DOWN_ORIENTATION) {
        if (y + 1 == this->h) {
            // Limite inferior alcanzado.
            return false;
        }
        Tile& to_tile = _getTile(x, y + 1);
        return _moveOccupant(from_tile, to_tile);
    }

    if (orientation == LEFT_ORIENTATION) {
        if (x == 0) {
            // Limite izquierdo alcanzado.
            return false;
        }
        Tile& to_tile = _getTile(x - 1, y);
        return _moveOccupant(from_tile, to_tile);
    }

    if (x + 1 == this->w) {
        // Limite derecho alcanzado.
        return false;
    }
    Tile& to_tile = _getTile(x + 1, y);
    return _moveOccupant(from_tile, to_tile);
}

void Map::establishEntitySpawningPosition(InstanceId id, int& x, int& y,
                                          bool is_creature) {
    bool valid_position = false;
    RandomNumberGenerator gen;
    while (!valid_position) {
        x = gen(0, this->w - 1);
        y = gen(0, this->h - 1);

        const Tile& tile = this->getTile(x, y);

        if (!tile.collision) {
            if (is_creature && tile.safe_zone) {
                continue;
            }
            valid_position = true;
        }
    }
    Tile& tile = this->_getTile(x, y);
    tile.occupant_id = id;
}

void Map::occupyTile(InstanceId id, const int x, const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.occupant_id = id;
}

void Map::addItem(const Id item_id, const int x, const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.item_id = item_id;
}

void Map::addItem(const Id item_id, int& x, int& y,
                  const Orientation& orientation) {
    bool empty_tile_found = false;

    Tile& tile = this->_getTile(x, y);
    fprintf(stderr, "Map::addItem DEBUG: intento agregar en x = %i, y = %i\n",
            x, y);
    if (!tile.item_id) {
        tile.item_id = item_id;
        empty_tile_found = true;
    }
    // HABRIA QUE PONERLE ALGUN LIMITE A ESTE LOOP DE MAXIMA PROFUNDIDAD.
    int step = 0;
    int i = orientation;
    int _x = x;
    int _y = y;
    while (!empty_tile_found) {
        for (int j = 0; j < N_ORIENTATIONS; ++j) {
            i = (i + j) % N_ORIENTATIONS;

            switch (i) {
                case UP_ORIENTATION: {
                    if (y == 0)
                        continue;

                    _y = y - 1 * step;
                    break;
                }

                case DOWN_ORIENTATION: {
                    if (y == this->h)
                        continue;

                    _y = y + 1 * step;
                    break;
                }

                case LEFT_ORIENTATION: {
                    if (x == 0)
                        continue;

                    _x = x - 1 * step;
                    break;
                }

                case RIGHT_ORIENTATION: {
                    if (x == this->w)
                        continue;

                    _x = x + 1 * step;
                    break;
                }

                default:
                    throw(Exception("Map::addItem: Unknown orientation."));
            }

            Tile& tile = this->_getTile(_x, _y);

            if (!tile.item_id) {
                tile.item_id = item_id;
                empty_tile_found = true;
                x = _x;
                y = _y;
                break;
            }
        }
        ++step;
    }
}

void Map::clearTileOccupant(const int x, const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.occupant_id = 0;
}

void Map::clearTileItem(const int x, const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.item_id = 0;
}

Map::~Map() {}

const char* ItemCouldNotBeAddedException::what() const noexcept {
    return "No se pudo agregar el item dropeado al mapa.";
}

//-----------------------------------------------------------------------------
