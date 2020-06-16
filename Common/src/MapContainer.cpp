#include "../includes/MapContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapContainer::MapContainer() {}

void MapContainer::loadMaps() {
    json maps, map;
    std::string filepath;
    int tile_w, tile_h;

    maps = JSON::loadJsonFile(MAPS_FILEPATH);
    tile_w = maps["tilewidth"];
    tile_h = maps["tileheight"];

    int size = maps["data"].size();
    for (int i = 0; i < size; i++) {
        filepath = maps["data"][i]["filepath"];
        map = JSON::loadJsonFile(filepath);
        Map tmp;
        tmp.init(map, maps["tilesets"], tile_w, tile_h);
        content.emplace(maps["data"][i]["id"], std::move(tmp));
    }
}

Map& MapContainer::operator[](Id id) {
    if (content.count(id) == 0) {
        throw Exception("Unknown map id.");
    }

    return content.at(id);
}

MapContainer::~MapContainer() {}

//-----------------------------------------------------------------------------
