#include "../includes/MapContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapContainer::MapContainer() {}

void MapContainer::loadMaps() {
    std::ifstream file(MAPS_FILEPATH);
    json j;
    file >> j;
}

MapContainer::~MapContainer() {}

//-----------------------------------------------------------------------------
