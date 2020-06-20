#include "../../includes/View/UserStats.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UserStats::UserStats(const Renderer* renderer, const Player& player)
    : HUDComponent(renderer), player(player) {}

void UserStats::init(const json& config) {
    if (initialized) {
        throw Exception("UserStats already initialized.");
    }

    offset_x = config["offset"]["x"];
    offset_y = config["offset"]["y"];
    w = config["w"];
    h = config["h"];

    initialized = true;
}

void UserStats::loadMedia() {
    if (!initialized) {
        throw Exception("UserStats not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_USER_STATS_BASE_FP);
}

void UserStats::render() const {
    if (!initialized) {
        throw Exception("UserStats not initialized.");
    }

    // Renderizar
    SDL_Rect render_quad = {offset_x, offset_y, w, h};
    g_renderer->render(base.getTexture(), &render_quad);
}

UserStats::~UserStats() {}

//-----------------------------------------------------------------------------
