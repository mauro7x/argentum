#include "../../includes/View/TileContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

void TileContainer::_loadTextures(const json& tileset) {
    TileId first_gid, next_first_gid, id;
    std::string dirpath, filepath;
    bool need_color_keying;
    int r = -1, g = -1, b = -1;

    first_gid = tileset["first_gid"];
    next_first_gid = tileset["next_first_gid"];
    dirpath = tileset["dirpath"];
    need_color_keying = tileset["color_key"]["is_needed"];

    if (need_color_keying) {
        r = tileset["color_key"]["color"]["r"];
        g = tileset["color_key"]["color"]["g"];
        b = tileset["color_key"]["color"]["b"];
    }

    int total_textures = next_first_gid - first_gid;
    for (int i = 0; i < total_textures; i++) {
        filepath = dirpath + std::to_string(i) + ".png";
        id = first_gid + i;
        content.emplace(id, std::move(Texture()));

        /* Cargamos la textura desde el archivo */
        if (need_color_keying) {
            content[id].loadFromFile(g_renderer, filepath, r, g, b);
        } else {
            content[id].loadFromFile(g_renderer, filepath);
        }
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

TileContainer::TileContainer(const Renderer* renderer) : g_renderer(renderer) {}

void TileContainer::loadMedia() {
    json tiles;
    tiles = JSON::loadJsonFile(TILES_FILEPATH);
    _loadTextures(tiles["ground"]);
    _loadTextures(tiles["decoration"]);
}

const Texture& TileContainer::operator[](const TileId id) const {
    if (content.count(id) == 0) {
        throw Exception("Unknown tile id.");
    }

    return content.at(id);
}

TileContainer::~TileContainer() {}

//-----------------------------------------------------------------------------
