#include "../../includes/GameView/TileContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

void TileContainer::_loadTextures(const json& tileset) {
    TileId first_gid, next_first_gid, id;
    std::string dirpath, filepath;
    bool need_color_keying;
    int r = -1, g = -1, b = -1;

    first_gid = tileset["first_gid"];
    next_first_gid = tileset["next_first_gid"];
    dirpath = tileset["src"]["dirpath"];
    dirpath = paths::asset(dirpath.c_str());
    need_color_keying = tileset["src"]["color_key"]["is_needed"];

    if (need_color_keying) {
        r = tileset["src"]["color_key"]["color"]["r"];
        g = tileset["src"]["color_key"]["color"]["g"];
        b = tileset["src"]["color_key"]["color"]["b"];
    }

    int total_textures = next_first_gid - first_gid;
    for (int i = 0; i < total_textures; i++) {
        filepath = dirpath + std::to_string(i) + ".png";
        id = first_gid + i;

        content.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                        std::forward_as_tuple());

        /* Cargamos la textura desde el archivo */
        if (need_color_keying) {
            content.at(id).loadFromFile(g_renderer, filepath, r, g, b);
        } else {
            content.at(id).loadFromFile(g_renderer, filepath);
        }
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

TileContainer::TileContainer(const Renderer* renderer) : g_renderer(renderer) {}

void TileContainer::loadMedia() {
    json tiles;
    tiles = JSON::loadJsonFile(paths::config(MAPS_FILEPATH))["tilesets"];
    _loadTextures(tiles["ground"]);
    _loadTextures(tiles["decoration"]);
    _loadTextures(tiles["npcs"]);
}

const Texture& TileContainer::operator[](const TileId id) const {
    if (content.count(id) == 0) {
        throw Exception("Unknown tile id.");
    }

    return content.at(id);
}

TileContainer::~TileContainer() {}

//-----------------------------------------------------------------------------
