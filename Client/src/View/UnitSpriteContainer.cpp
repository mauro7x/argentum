#include "../../includes/View/UnitSpriteContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

void UnitSpriteContainer::_loadSpritesFromJson(const json& sprites,
                                               const std::string& dirpath) {
    int size = sprites.size();
    Id id;
    std::string filepath;
    bool need_color_keying;
    int r = -1, g = -1, b = -1;

    for (int i = 0; i < size; i++) {
        id = sprites[i]["gid"];
        filepath = dirpath + std::to_string(id) + ".png";
        Sprite tmp;
        tmp.id = id;
        tmp.clip_w = sprites[i]["clip_w"];
        tmp.clip_h = sprites[i]["clip_h"];

        tmp.up_row = sprites[i]["up_row_col_clips"][0];
        tmp.up_col = sprites[i]["up_row_col_clips"][1];
        tmp.up_clips = sprites[i]["up_row_col_clips"][2];

        tmp.down_row = sprites[i]["down_row_col_clips"][0];
        tmp.down_col = sprites[i]["down_row_col_clips"][1];
        tmp.down_clips = sprites[i]["down_row_col_clips"][2];

        tmp.left_row = sprites[i]["left_row_col_clips"][0];
        tmp.left_col = sprites[i]["left_row_col_clips"][1];
        tmp.left_clips = sprites[i]["left_row_col_clips"][2];

        tmp.right_row = sprites[i]["right_row_col_clips"][0];
        tmp.right_col = sprites[i]["right_row_col_clips"][1];
        tmp.right_clips = sprites[i]["right_row_col_clips"][2];

        tmp.change_every_n_frames = sprites[i]["change_every_n_frames"];

        Texture texture;

        need_color_keying = sprites[i]["color_key"]["is_needed"];
        if (need_color_keying) {
            r = sprites[i]["color_key"]["color"]["r"];
            g = sprites[i]["color_key"]["color"]["g"];
            b = sprites[i]["color_key"]["color"]["b"];
            texture.loadFromFile(g_renderer, filepath, r, g, b);
        } else {
            texture.loadFromFile(g_renderer, filepath);
        }

        tmp.texture = std::move(texture);
        content[id] = std::move(tmp);
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UnitSpriteContainer::UnitSpriteContainer(const Renderer* renderer)
    : g_renderer(renderer) {}

void UnitSpriteContainer::loadMedia() {
    json sprites = JSON::loadJsonFile(UNIT_SPRITES_FILEPATH);

    std::string dirpath = sprites["dirpath"];
    _loadSpritesFromJson(sprites["pj"]["head"], dirpath);
    _loadSpritesFromJson(sprites["pj"]["body"], dirpath);
    _loadSpritesFromJson(sprites["pj"]["helmet"], dirpath);
    _loadSpritesFromJson(sprites["pj"]["armour"], dirpath);
    _loadSpritesFromJson(sprites["pj"]["shield"], dirpath);
    _loadSpritesFromJson(sprites["pj"]["weapon"], dirpath);
}

const Sprite& UnitSpriteContainer::get(const Id id) const {
    if (content.count(id) == 0) {
        throw Exception("Unknown sprite id.");
    }

    return content.at(id);
}

const Sprite& UnitSpriteContainer::operator[](const Id id) const {
    return this->get(id);
}

UnitSpriteContainer::~UnitSpriteContainer() {}

//-----------------------------------------------------------------------------
