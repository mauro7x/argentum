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

    maps = JSON::loadJsonFile(MAPS_FILEPATH);

    int size = maps["data"].size();

    for (int i = 0; i < size; i++) {
        filepath = maps["data"][i]["filepath"];
        map = JSON::loadJsonFile(filepath);
        Map tmp;
        tmp.init(map, maps["tilesets"]);
        content.emplace(maps["data"][i]["id"], std::move(tmp));
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

std::vector<Id> MapContainer::getMapsId() const {
    std::vector<Id> maps_id;

    std::unordered_map<Id, Map>::const_iterator it = this->content.begin();
    while (it != this->content.end()) {
        maps_id.push_back(it->first);
        ++it;
    }

    return maps_id;
}

const Id MapContainer::getCharacterSpawningMap() const {
    std::vector<Id> possible_maps;
    std::unordered_map<Id, Map>::const_iterator it = this->content.begin();
    while (it != this->content.end()) {
        possible_maps.push_back(it->first);
        ++it;
    }
    RandomNumberGenerator gen;
    return possible_maps.at(gen(0, possible_maps.size() - 1));
}

MapContainer::~MapContainer() {}

//-----------------------------------------------------------------------------
