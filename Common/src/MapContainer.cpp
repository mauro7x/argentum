#include "../includes/MapContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapContainer::MapContainer() {}

void MapContainer::loadMaps() {
    json maps, map;
    maps = JSON::loadJsonFile(MAPS_FILEPATH);

    int tile_w, tile_h;
    tile_w = maps["tilewidth"];
    tile_h = maps["tileheight"];

    int size = maps["data"].size();
    for (int i = 0; i < size; i++) {
        map = JSON::loadJsonFile(maps["data"][i]["filepath"]);
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
