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

    render_rect.x = config["offset"]["x"];
    render_rect.y = config["offset"]["y"];
    render_rect.w = config["w"];
    render_rect.h = config["h"];

    initialized = true;
}

void UserStats::loadMedia() {
    if (!initialized) {
        throw Exception("UserStats not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_USER_STATS_BASE_FP);
}

void UserStats::update() {}

void UserStats::render() const {
    if (!initialized) {
        throw Exception("UserStats not initialized.");
    }

    // Renderizar la base
    SDL_Rect render_quad = render_rect;
    g_renderer->render(base.getTexture(), &render_quad);
}

void UserStats::free() {}

UserStats::~UserStats() {}

//-----------------------------------------------------------------------------
