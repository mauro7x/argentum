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
        content.at(id).id = id;
        content.at(id).clip_w = (uint16_t)sprites[i]["clip_w"];
        content.at(id).clip_h = (uint16_t)sprites[i]["clip_h"];

        content.at(id).up_row = (uint8_t)sprites[i]["up_row_col_clips"][0];
        content.at(id).up_col = (uint8_t)sprites[i]["up_row_col_clips"][1];
        content.at(id).up_clips = (uint8_t)sprites[i]["up_row_col_clips"][2];

        content.at(id).down_row = (uint8_t)sprites[i]["down_row_col_clips"][0];
        content.at(id).down_col = (uint8_t)sprites[i]["down_row_col_clips"][1];
        content.at(id).down_clips =
            (uint8_t)sprites[i]["down_row_col_clips"][2];

        content.at(id).left_row = (uint8_t)sprites[i]["left_row_col_clips"][0];
        content.at(id).left_col = (uint8_t)sprites[i]["left_row_col_clips"][1];
        content.at(id).left_clips =
            (uint8_t)sprites[i]["left_row_col_clips"][2];

        content.at(id).right_row =
            (uint8_t)sprites[i]["right_row_col_clips"][0];
        content.at(id).right_col =
            (uint8_t)sprites[i]["right_row_col_clips"][1];
        content.at(id).right_clips =
            (uint8_t)sprites[i]["right_row_col_clips"][2];

        content.at(id).change_every_n_frames =
            (uint8_t)sprites[i]["change_every_n_frames"];

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
    _loadSpritesFromJson(sprites["creature"], dirpath);
}

void UnitSpriteContainer::loadHeadsMedia() {
    json sprites = JSON::loadJsonFile(UNIT_SPRITES_FILEPATH);

    std::string dirpath = sprites["dirpath"];
    _loadSpritesFromJson(sprites["pj"]["head"], dirpath);
}

void UnitSpriteContainer::loadBodiesMedia() {
    json sprites = JSON::loadJsonFile(UNIT_SPRITES_FILEPATH);

    std::string dirpath = sprites["dirpath"];
    _loadSpritesFromJson(sprites["pj"]["body"], dirpath);
}

const UnitSprite& UnitSpriteContainer::get(const Id id) const {
    if (content.count(id) == 0) {
        throw Exception("Unknown sprite id: %u", id);
    }

    return content.at(id);
}

const UnitSprite& UnitSpriteContainer::operator[](const Id id) const {
    return this->get(id);
}

UnitSpriteContainer::~UnitSpriteContainer() {}

//-----------------------------------------------------------------------------
