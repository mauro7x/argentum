#include "../../includes/GameView/CharacterContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

CharacterContainer::CharacterContainer(const Renderer* renderer,
                                       const Camera& camera,
                                       UnitSpriteContainer* sprites)
    : g_renderer(renderer), g_camera(camera), g_sprites(sprites) {}

void CharacterContainer::loadMedia() {
    // Fuentes a utilizar
    nickname_font =
        TTF_OpenFont(paths::asset(FONT_AUGUSTA_FP).c_str(), INFO_NAME_FONTSIZE);
    level_font = TTF_OpenFont(paths::asset(FONT_CINZELBOLD_FP).c_str(),
                              INFO_LVL_FONTSIZE);
    msg_font = TTF_OpenFont(MSG_FONT.c_str(), MSG_FONTSIZE);

    if (!nickname_font || !level_font || !msg_font) {
        throw Exception(
            "CharacterContainer::loadMedia: error opening TTF_Font/s.");
    }
}

void CharacterContainer::add(const InstanceId id,
                             const CharacterData& init_data) {
    if (content.count(id) > 0) {
        throw Exception(
            "CharacterContainer::add: ID already taken by another character "
            "(id repeated).");
    }

    content.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                    std::forward_as_tuple(g_renderer, g_camera, g_sprites,
                                          nickname_font, level_font, msg_font));
    content.at(id).init(init_data);
}

void CharacterContainer::update(const InstanceId id,
                                const CharacterData& updated_data) {
    if (content.count(id) == 0) {
        throw Exception(
            "CharacterContainer::update: attempt to update unknown character "
            "(id is invalid).");
    }

    content.at(id).update(updated_data);
}

SDL_Point CharacterContainer::getPos(const InstanceId id) const {
    if (content.count(id) == 0) {
        throw Exception(
            "CharacterContainer::getPos: attempt to get pos of unknown "
            "character (id is invalid).");
    }

    return content.at(id).getPos();
}

void CharacterContainer::act(const int it) {
    for (auto i = content.begin(); i != content.end(); i++) {
        i->second.act(it);
    }
}

void CharacterContainer::addMessage(InstanceId sender_id,
                                    const std::string& msg) {
    if (content.count(sender_id) > 0) {
        content.at(sender_id).addMessage(msg);
    }
}

void CharacterContainer::render(const InstanceId id) const {
    if (content.count(id) > 0) {
        content.at(id).render();
    }
}

void CharacterContainer::remove(const InstanceId id) {
    if (content.count(id) == 0) {
        throw Exception(
            "CharacterContainer::remove: attempt to remove unknown character "
            "(id is invalid).");
    }

    content.erase(id);
}

void CharacterContainer::clear() {
    content.clear();
}

CharacterContainer::~CharacterContainer() {
    if (nickname_font) {
        TTF_CloseFont(nickname_font);
        nickname_font = NULL;
    }

    if (level_font) {
        TTF_CloseFont(level_font);
        level_font = NULL;
    }

    if (msg_font) {
        TTF_CloseFont(msg_font);
        msg_font = NULL;
    }
}

//-----------------------------------------------------------------------------
