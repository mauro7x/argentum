#include "../includes/MapContainer.h"

// Provisoriamente para map spawning selection
#include "../includes/RandomNumberGenerator.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MapContainer::MapContainer() {}

void MapContainer::loadMaps() {
    json maps, map;
    std::string filepath;
    std::vector<Id> creatures;
    maps = JSON::loadJsonFile(paths::config(MAPS_FILEPATH));

    int size = maps["data"].size();
    Id map_id;
    for (int i = 0; i < size; i++) {
        filepath = maps["data"][i]["filepath"];
        filepath = paths::asset(filepath.c_str());
        map = JSON::loadJsonFile(filepath);
        map_id = (Id)maps["data"][i]["id"];
        maps["data"][i]["creatures"].get_to(creatures);
        content.emplace(std::piecewise_construct, std::forward_as_tuple(map_id),
                        std::forward_as_tuple());
        content.at(map_id).init(map, maps["tilesets"], creatures);
    }
}

Map& MapContainer::operator[](const Id id) {
    if (content.count(id) == 0) {
        throw Exception("Unknown map id.");
    }

    return content.at(id);
}

const Map& MapContainer::operator[](const Id id) const {
    if (content.count(id) == 0) {
        throw Exception("Unknown map id.");
    }

    return content.at(id);
}

void MapContainer::getMapsId(std::vector<Id>& maps_id) const {
    std::unordered_map<Id, Map>::const_iterator it = this->content.begin();
    while (it != this->content.end()) {
        maps_id.push_back(it->first);
        ++it;
    }
}

MapContainer::~MapContainer() {}

//-----------------------------------------------------------------------------
