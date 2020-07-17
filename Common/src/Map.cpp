#include "../includes/Map.h"

#include "../includes/RandomNumberGenerator.h"

#define MAX_FREE_ITEM_TILE_SEARCHING_STEP 30
#define SAFE_ZONE_MAX_SEARCHING_STEP 100
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
    Tile tile;
    size_t i = 0;
    for (size_t row = 0; row < (size_t)h; row++) {
        for (size_t col = 0; col < (size_t)w; col++) {
            // Calculamos el índice actual
            i = (row * w) + col;

            // Ids gráficos (tiles gráficos)
            tile.ground_1_id = map["layers"][GROUND1_LAYER]["data"][i];
            tile.ground_2_id = map["layers"][GROUND2_LAYER]["data"][i];
            tile.decoration_id = map["layers"][DECORATION_LAYER]["data"][i];
            tile.roof_id = map["layers"][ROOF_LAYER]["data"][i];
            tile.npc_id = map["layers"][NPC_LAYER]["data"][i];

            // Metadata (tiles lógicos)
            int safe_zone = map["layers"][SAFEZONE_LAYER]["data"][i];
            tile.safe_zone = (bool)safe_zone;
            int collision = map["layers"][COLLISION_LAYER]["data"][i];
            tile.collision = (bool)collision;
            int indoor = map["layers"][INDOOR_LAYER]["data"][i];
            tile.indoor = (bool)indoor;
            int portal = map["layers"][PORTAL_LAYER]["data"][i];
            tile.portal = (bool)portal;

            if (tile.npc_id > 0) {
                /* Debemos calcular el id global del npc */
                size_t npc_gid = tile.npc_id;
                size_t tileset_first_gid = tilesets["npcs"]["first_gid"];
                size_t npc_lid = npc_gid - tileset_first_gid;
                std::string tileset_filepath = tilesets["npcs"]["filepath"];
                tileset_filepath = paths::asset(tileset_filepath.c_str());
                json tileset = JSON::loadJsonFile(tileset_filepath);
                tile.npc = tileset["tiles"][npc_lid]["properties"][0]["value"];
            } else {
                tile.npc = 0;
            }

            // Ocupantes del tile
            tile.occupant_id = 0;
            tile.item_id = 0;
            tile.item_amount = 0;

            tiles.push_back(tile);
        }
    }
}

Tile& Map::_getTile(const int x, const int y) {
    if (!_isValid(x, y)) {
        throw Exception("Invalid map coordinates. x = %i, y = %i\n", x, y);
    }

    return tiles[TILENUMBER(x, y)];
}

Tile& Map::_getTileWithoutChecks(const int x, const int y) {
    return tiles[TILENUMBER(x, y)];
}


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Map::Map() {}

void Map::init(const json& map, const json& tilesets,
               std::vector<Id>& creatures, std::string& name) {
    w = map["width"];
    h = map["height"];
    this->creatures = creatures;
    this->name = name;

    _checkIfValid(map);
    _fillTiles(map, tilesets);
}

//-----------------------------------------------------------------------------
// Escritura

TileId& Map::getNPC(const int x, const int y) {
    Tile& tile = _getTile(x, y);
    return tile.npc_id;
}

TileId& Map::getPortal(int x, int y) {
    // Hay que llegar a la celda inferior izquierda del portal
    // Primero descendemos
    while (_getTile(x, y).portal) {
        y++;
    }
    y--;

    // Ahora vamos a la izquierda
    while (_getTile(x, y).portal) {
        x--;
    }
    x++;

    return _getTile(x, y).decoration_id;
}

void Map::occupyTile(InstanceId id, const int x, const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.occupant_id = id;
}

void Map::setItem(const Id item_id, const uint32_t amount, const int x,
                  const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.item_id = item_id;
    tile.item_amount = amount;
}


void Map::clearTileOccupant(const int x, const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.occupant_id = 0;
}

void Map::clearTileItem(const int x, const int y) {
    Tile& tile = this->_getTile(x, y);
    tile.item_id = 0;
    tile.item_amount = 0;
}

void Map::clear() {
    for (size_t row = 0; row < (size_t)h; row++) {
        for (size_t col = 0; col < (size_t)w; col++) {
            Tile& tile = _getTileWithoutChecks(col, row);
            tile.item_amount = 0;
            tile.item_id = 0;
            tile.occupant_id = 0;
        }
    }
}

//-----------------------------------------------------------------------------
// Lectura

int Map::getWidthTiles() const {
    return w;
}

int Map::getHeightTiles() const {
    return h;
}

const bool Map::isSafeZone(const int x, const int y) const {
    return this->getTile(x, y).safe_zone;
}

bool Map::_isValid(const int x, const int y) const {
    if ((x >= w) || (x < 0) || (y >= h) || (y < 0)) {
        return false;
    } else {
        return true;
    }
}
Tile& Map::getTile(const int x, const int y){
    if (!_isValid(x, y)) {
        throw Exception("Invalid map coordinates. x = %i, y = %i\n", x, y);
    }

    return tiles[TILENUMBER(x, y)];
}
const Tile& Map::getTile(const int x, const int y) const {
    if (!_isValid(x, y)) {
        throw Exception("Invalid map coordinates. x = %i, y = %i\n", x, y);
    }

    return tiles[TILENUMBER(x, y)];
}

const Tile& Map::getTileWithoutChecks(const int x, const int y) const {
    return tiles[TILENUMBER(x, y)];
}

const std::vector<Id>& Map::getCreatures() const {
    return creatures;
}

const std::string& Map::getMapName() const {
    return name;
}

//-----------------------------------------------------------------------------

Map::~Map() {}

const char* CouldNotFindFreeTileException::what() const noexcept {
    return "No se pudieron dropear algunos items.";
}

//-----------------------------------------------------------------------------
