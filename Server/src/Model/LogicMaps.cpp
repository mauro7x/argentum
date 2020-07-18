#include "../../includes/Model/LogicMaps.h"

#include "../../../Common/includes/RandomNumberGenerator.h"

#define MAX_FREE_ITEM_TILE_SEARCHING_STEP 30
#define SAFE_ZONE_MAX_SEARCHING_STEP 100

//-----------------------------------------------------------------------------
// Métodos privados
const bool LogicMaps::_moveOccupant(Tile& from_tile, Tile& to_tile,
                                    bool is_creature) {
    if (to_tile.collision || to_tile.occupant_id || to_tile.npc_id) {
        // El tile está ocupado / hay colisión.
        return false;
    }
    if (is_creature && to_tile.safe_zone) {
        return false;
    }

    // Se puede mover.
    to_tile.occupant_id = from_tile.occupant_id;
    from_tile.occupant_id = 0;

    return true;
}

//-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública
LogicMaps::LogicMaps() {}

void LogicMaps::loadMedia() {
    maps.loadMaps();
}

void LogicMaps::getMapsId(std::vector<Id>& maps_id) const {
    return maps.getMapsId(maps_id);
}

Map& LogicMaps::getMap(const Id id) {
    return maps[id];
}

const Map& LogicMaps::getMap(const Id id) const {
    return maps[id];
}

void LogicMaps::establishEntitySpawningPosition(const Id& map_id, int& x,
                                                int& y, bool is_creature) {
    Map& map = maps[map_id];
    bool valid_position = false;
    RandomNumberGenerator gen;
    int step = 0;
    while (!valid_position) {
        x = gen(0, map.getWidthTiles() - 1);
        y = gen(0, map.getHeightTiles() - 1);

        const Tile& tile = map.getTile(x, y);

        if (!tile.collision && !tile.npc_id && !tile.occupant_id) {
            if (is_creature && tile.safe_zone &&
                step < SAFE_ZONE_MAX_SEARCHING_STEP) {
                ++step;
                continue;
            }
            if (!is_creature && !tile.safe_zone &&
                step < SAFE_ZONE_MAX_SEARCHING_STEP) {
                ++step;
                continue;
            }

            valid_position = true;
        }

        ++step;
    }
}
void LogicMaps::getNearestFreeTile(const Id& map_id, int& x, int& y,
                                   const bool is_for_item) const {
    // Primero nos fijamos si está libre el mismo tile en (x, y)
    const Map& map = maps[map_id];
    const Tile& current_tile = map.getTile(x, y);
    if (is_for_item) {
        if (!current_tile.item_id && !current_tile.collision &&
            !current_tile.npc_id) {
            return;
        }
    } else {
        if (!current_tile.item_id && !current_tile.collision &&
            !current_tile.npc_id && !current_tile.occupant_id) {
            return;
        }
    }

    bool empty_tile_found = false;

    int step = 1;
    int _x = 0;
    int _y = 0;

    while (!empty_tile_found) {
        for (int x_inc = -1; x_inc <= 1; ++x_inc) {
            _x = (x + x_inc * step);

            for (int y_inc = -1; y_inc <= 1; ++y_inc) {
                _y = (y + y_inc * step);

                if (_x < 0 || _y < 0 || _x > map.getWidthTiles() - 1 ||
                    _y > map.getHeightTiles() - 1)
                    continue;

                const Tile& tile = map.getTile(_x, _y);

                if (is_for_item) {
                    if (!tile.item_id && !tile.collision && !tile.npc_id) {
                        empty_tile_found = true;
                        x = _x;
                        y = _y;
                        return;
                    }
                } else {
                    if (!tile.item_id && !tile.collision && !tile.npc_id &&
                        !tile.occupant_id) {
                        empty_tile_found = true;
                        x = _x;
                        y = _y;
                        return;
                    }
                }
            }
        }
        ++step;
        if (step == MAX_FREE_ITEM_TILE_SEARCHING_STEP)
            throw(CouldNotFindFreeTileException());
    }
}

const std::vector<Id>& LogicMaps::getCreatures(const Id& map_id) const {
    return maps[map_id].getCreatures();
}

const Tile& LogicMaps::getTile(const Id& map_id, const int x,
                               const int y) const {
    return maps[map_id].getTile(x, y);
}

const std::string& LogicMaps::getMapName(const Id& map_id) const {
    return maps[map_id].getMapName();
}

void LogicMaps::addItem(const Id& map_id, const Id item_id,
                        const uint32_t amount, int& x, int& y) {
    this->getNearestFreeTile(map_id, x, y, true);
    Tile& tile = maps[map_id].getTile(x, y);
    tile.item_id = item_id;
    tile.item_amount = amount;
}

void LogicMaps::clearTileOccupant(const Id& map_id, const int x, const int y) {
    return maps[map_id].clearTileOccupant(x, y);
}

void LogicMaps::occupyTile(const Id& map_id, InstanceId id, const int x,
                           const int y) {
    return maps[map_id].occupyTile(id, x, y);
}

void LogicMaps::clearTileItem(const Id& map_id, const int x, const int y) {
    return maps[map_id].clearTileItem(x, y);
}

const bool LogicMaps::moveOccupant(const Id& map_id, const int x, const int y,
                                   const Orientation& orientation,
                                   bool is_creature) {
    Map& map = maps[map_id];
    Tile& from_tile = map.getTile(x, y);

    if (orientation == UP_ORIENTATION) {
        if (y == 0) {
            // Limite superior alcanzado.
            return false;
        }
        Tile& to_tile = map.getTile(x, y - 1);
        return _moveOccupant(from_tile, to_tile, is_creature);
    }

    if (orientation == DOWN_ORIENTATION) {
        if (y + 1 == map.getHeightTiles()) {
            // Limite inferior alcanzado.
            return false;
        }
        Tile& to_tile = map.getTile(x, y + 1);
        return _moveOccupant(from_tile, to_tile, is_creature);
    }

    if (orientation == LEFT_ORIENTATION) {
        if (x == 0) {
            // Limite izquierdo alcanzado.
            return false;
        }
        Tile& to_tile = map.getTile(x - 1, y);
        return _moveOccupant(from_tile, to_tile, is_creature);
    }

    if (x + 1 == map.getWidthTiles()) {
        // Limite derecho alcanzado.
        return false;
    }
    Tile& to_tile = map.getTile(x + 1, y);
    return _moveOccupant(from_tile, to_tile, is_creature);
}

const bool LogicMaps::isSafeZone(const Id& map_id, const int x,
                                 const int y) const {
    return maps[map_id].isSafeZone(x, y);
}

bool LogicMaps::isPositionValidForCreature(const Id& map_id, const int x,
                                           const int y) const {
    const Tile& tile = maps[map_id].getTile(x, y);
    if (tile.collision || tile.occupant_id || tile.npc_id || tile.safe_zone) {
        return false;
    }
    return true;
}

//-------------------------------------------------------------------------