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

    int size = maps.size();
    for (int i = 0; i < size; i++) {
        map = _loadJsonFile(maps[i]["filepath"]);
        content[maps[i]["id"]] = std::move(Map(map));
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
