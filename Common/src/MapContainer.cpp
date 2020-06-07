#include "../includes/MapContainer.h"

#include <iostream>

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
    json maps_info, map;
    maps_info = _loadJsonFile(MAPS_FILEPATH);

    int size = maps_info.size();
    for (int i = 0; i < size; i++) {
        // cargar mapas
        map = _loadJsonFile(maps_info[i]["filepath"]);
        std::cout << "Se cargo un mapa de " << map["width"] << "x"
                  << map["height"] << std::endl;
    }
}

MapContainer::~MapContainer() {}

//-----------------------------------------------------------------------------
