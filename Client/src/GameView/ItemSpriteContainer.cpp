#include "../../includes/GameView/ItemSpriteContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

void ItemSpriteContainer::_loadSpritesFromJson(const json& sprites,
                                               const std::string& dirpath) {
    int size = sprites.size();
    Id id;
    std::string filepath;
    bool need_color_keying;
    int r = -1, g = -1, b = -1;

    for (int i = 0; i < size; i++) {
        id = sprites[i]["gid"];
        filepath = dirpath + std::to_string(id) + ".png";

        content.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                        std::forward_as_tuple(id));

        need_color_keying = sprites[i]["color_key"]["is_needed"];
        if (need_color_keying) {
            r = sprites[i]["color_key"]["color"]["r"];
            g = sprites[i]["color_key"]["color"]["g"];
            b = sprites[i]["color_key"]["color"]["b"];
            content.at(id).texture.loadFromFile(g_renderer, filepath, r, g, b);
        } else {
            content.at(id).texture.loadFromFile(g_renderer, filepath);
        }
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ItemSpriteContainer::ItemSpriteContainer(const Renderer* renderer)
    : g_renderer(renderer) {}

void ItemSpriteContainer::loadMedia() {
    json sprites = JSON::loadJsonFile(paths::config(ITEM_SPRITES_FILEPATH));

    std::string dirpath = sprites["dirpath"];
    dirpath = paths::asset(dirpath.c_str());
    _loadSpritesFromJson(sprites["helmets"], dirpath);
    _loadSpritesFromJson(sprites["armours"], dirpath);
    _loadSpritesFromJson(sprites["shields"], dirpath);
    _loadSpritesFromJson(sprites["weapons"], dirpath);
    _loadSpritesFromJson(sprites["wands"], dirpath);
    _loadSpritesFromJson(sprites["potions"], dirpath);
    _loadSpritesFromJson(sprites["others"], dirpath);
}

const ItemSprite& ItemSpriteContainer::get(const Id id) const {
    try {
        return content.at(id);
    } catch (...) {
        throw Exception(
            "ItemSpriteContainer::operator[]: unknown item sprite id (%u)\n",
            id);
    }
}

const ItemSprite& ItemSpriteContainer::operator[](const Id id) const {
    try {
        return content.at(id);
    } catch (...) {
        throw Exception(
            "ItemSpriteContainer::operator[]: unknown item sprite id (%u)\n",
            id);
    }
}

ItemSpriteContainer::~ItemSpriteContainer() {}

//-----------------------------------------------------------------------------
