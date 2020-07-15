#include "../../includes/GameView/UnitSpriteContainer.h"

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

        content.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                        std::forward_as_tuple(id));
        UnitSprite& new_sprite = content.at(id);

        new_sprite.id = id;
        new_sprite.clip_w = (uint16_t)sprites[i]["clip_w"];
        new_sprite.clip_h = (uint16_t)sprites[i]["clip_h"];

        new_sprite.up_row = (uint8_t)sprites[i]["up_row_col_clips"][0];
        new_sprite.up_col = (uint8_t)sprites[i]["up_row_col_clips"][1];
        new_sprite.up_clips = (uint8_t)sprites[i]["up_row_col_clips"][2];

        new_sprite.down_row = (uint8_t)sprites[i]["down_row_col_clips"][0];
        new_sprite.down_col = (uint8_t)sprites[i]["down_row_col_clips"][1];
        new_sprite.down_clips = (uint8_t)sprites[i]["down_row_col_clips"][2];

        new_sprite.left_row = (uint8_t)sprites[i]["left_row_col_clips"][0];
        new_sprite.left_col = (uint8_t)sprites[i]["left_row_col_clips"][1];
        new_sprite.left_clips = (uint8_t)sprites[i]["left_row_col_clips"][2];

        new_sprite.right_row = (uint8_t)sprites[i]["right_row_col_clips"][0];
        new_sprite.right_col = (uint8_t)sprites[i]["right_row_col_clips"][1];
        new_sprite.right_clips = (uint8_t)sprites[i]["right_row_col_clips"][2];

        new_sprite.change_every_n_frames =
            (uint8_t)sprites[i]["change_every_n_frames"];

        need_color_keying = sprites[i]["color_key"]["is_needed"];
        if (need_color_keying) {
            r = sprites[i]["color_key"]["color"]["r"];
            g = sprites[i]["color_key"]["color"]["g"];
            b = sprites[i]["color_key"]["color"]["b"];
            new_sprite.texture.loadFromFile(g_renderer, filepath, r, g, b);
        } else {
            new_sprite.texture.loadFromFile(g_renderer, filepath);
        }
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UnitSpriteContainer::UnitSpriteContainer(const Renderer* renderer)
    : g_renderer(renderer) {}

void UnitSpriteContainer::loadMedia() {
    json sprites = JSON::loadJsonFile(paths::config(UNIT_SPRITES_FILEPATH));

    shorter_characters_offset = sprites["shorter_characters_offset"];
    std::string dirpath = sprites["dirpath"];
    dirpath = paths::asset(dirpath.c_str());
    _loadSpritesFromJson(sprites["pj"]["head"], dirpath);
    _loadSpritesFromJson(sprites["pj"]["body"], dirpath);
    _loadSpritesFromJson(sprites["pj"]["helmet"], dirpath);
    _loadSpritesFromJson(sprites["pj"]["armour"], dirpath);
    _loadSpritesFromJson(sprites["pj"]["shield"], dirpath);
    _loadSpritesFromJson(sprites["pj"]["weapon"], dirpath);
    _loadSpritesFromJson(sprites["creature"], dirpath);
}

void UnitSpriteContainer::loadHeadsMedia() {
    json sprites = JSON::loadJsonFile(paths::config(UNIT_SPRITES_FILEPATH));

    std::string dirpath = sprites["dirpath"];
    dirpath = paths::asset(dirpath.c_str());
    _loadSpritesFromJson(sprites["pj"]["head"], dirpath);
}

void UnitSpriteContainer::loadBodiesMedia() {
    json sprites = JSON::loadJsonFile(paths::config(UNIT_SPRITES_FILEPATH));

    std::string dirpath = sprites["dirpath"];
    dirpath = paths::asset(dirpath.c_str());
    _loadSpritesFromJson(sprites["pj"]["body"], dirpath);
}

const UnitSprite& UnitSpriteContainer::get(Id id, bool is_shorter) const {
    if (is_shorter) {
        id += shorter_characters_offset;
    }

    return content.at(id);
}

const UnitSprite& UnitSpriteContainer::operator[](const Id id) const {
    return this->get(id);
}

UnitSpriteContainer::~UnitSpriteContainer() {}

//-----------------------------------------------------------------------------
