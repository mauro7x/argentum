#include "../includes/MapContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

json MapContainer::_loadMapsFile() const {
    std::ifstream file(MAPS_FILEPATH);
    if (file.fail()) {
        throw Exception("Error opening file: %s", MAPS_FILEPATH);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", MAPS_FILEPATH);
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", MAPS_FILEPATH);
    }

    return j;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapContainer::MapContainer() {}

void MapContainer::loadMaps() {
    json j = _loadMapsFile();
}

MapContainer::~MapContainer() {}

//-----------------------------------------------------------------------------
