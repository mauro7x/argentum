#include "../../includes/GameView/CreatureContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

CreatureContainer::CreatureContainer(const Renderer* renderer,
                                     const Camera& camera,
                                     UnitSpriteContainer* sprites)
    : g_renderer(renderer), g_camera(camera), g_sprites(sprites) {}

void CreatureContainer::loadMedia() {
    // Fuentes a utilizar
    nickname_font =
        TTF_OpenFont(paths::asset(FONT_AUGUSTA_FP).c_str(), INFO_NAME_FONTSIZE);

    if (!nickname_font) {
        throw Exception(
            "CreatureContainer::loadMedia: error opening TTF_Font/s.");
    }
}

void CreatureContainer::add(const InstanceId id,
                            const CreatureData& init_data) {
    if (content.count(id) > 0) {
        throw Exception(
            "CreatureContainer::add: ID already taken by another creature (id "
            "repeated).");
    }

    content.emplace(
        std::piecewise_construct, std::forward_as_tuple(id),
        std::forward_as_tuple(g_renderer, g_camera, g_sprites, nickname_font));
    content.at(id).init(init_data);
}

void CreatureContainer::update(const InstanceId id,
                               const CreatureData& updated_data) {
    content.at(id).update(updated_data);
}

SDL_Point CreatureContainer::getPos(const InstanceId id) const {
    return content.at(id).getPos();
}

void CreatureContainer::act(const int it) {
    for (auto i = content.begin(); i != content.end(); i++) {
        i->second.act(it);
    }
}

void CreatureContainer::render(const InstanceId id) const {
    if (content.count(id) > 0) {
        content.at(id).render();
    }
}

void CreatureContainer::remove(const InstanceId id) {
    content.erase(id);
}

void CreatureContainer::clear() {
    content.clear();
}

CreatureContainer::~CreatureContainer() {
    if (nickname_font) {
        TTF_CloseFont(nickname_font);
        nickname_font = NULL;
    }
}

//-----------------------------------------------------------------------------
