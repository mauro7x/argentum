#include "../../includes/View/UserInfo.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UserInfo::UserInfo(const Renderer* renderer, const Player& player)
    : HUDComponent(renderer), player(player) {}

void UserInfo::init(const json& config) {
    if (initialized) {
        throw Exception("UserInfo already initialized.");
    }

    offset_x = config["offset"]["x"];
    offset_y = config["offset"]["y"];
    w = config["w"];
    h = config["h"];

    initialized = true;
}

void UserInfo::loadMedia() {
    if (!initialized) {
        throw Exception("UserInfo not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_USER_INFO_BASE_FP);
}

void UserInfo::render() const {
    if (!initialized) {
        throw Exception("UserInfo not initialized.");
    }

    // Renderizar
    SDL_Rect render_quad = {offset_x, offset_y, w, h};
    g_renderer->render(base.getTexture(), &render_quad);
}

UserInfo::~UserInfo() {}

//-----------------------------------------------------------------------------
