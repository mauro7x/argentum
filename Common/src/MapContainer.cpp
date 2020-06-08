#include "../includes/MapContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

json MapContainer::_loadJsonFile(std::string filepath) const {
    std::ifstream file(filepath);
    if (file.fail()) {
        throw Exception("Error opening file: %s", filepath);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", filepath);
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", filepath);
    }

    return j;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapContainer::MapContainer() {}

void MapContainer::loadMaps() {
    json maps, map;
    maps = _loadJsonFile(MAPS_FILEPATH);

    int tile_w, tile_h;
    tile_w = maps["tilewidth"];
    tile_h = maps["tileheight"];

    int size = maps["data"].size();
    for (int i = 0; i < size; i++) {
        map = _loadJsonFile(maps["data"][i]["filepath"]);
        Map tmp;
        tmp.init(map, tile_w, tile_h);
        content[maps["data"][i]["id"]] = std::move(tmp);
    }
}

const Map& MapContainer::operator[](Id id) const {
    if (content.count(id) == 0) {
        throw Exception("Unknown map id.");
    }

    return content.at(id);
}

MapContainer::~MapContainer() {}

//-----------------------------------------------------------------------------
