#include "../../includes/View/UserStats.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UserStats::UserStats(const Renderer* renderer, const Player& player)
    : HUDComponent(renderer),
      player(player),
      health(renderer),
      mana(renderer) {}

void UserStats::init(const json& config) {
    if (initialized) {
        throw Exception("UserStats already initialized.");
    }

    // Offsets de renderizado
    render_rect.x = config["offset"]["x"];
    render_rect.y = config["offset"]["y"];
    render_rect.w = config["w"];
    render_rect.h = config["h"];

    // Health
    SDL_Point health_offset = {0};
    health_offset.x = config["components"]["health_bar"]["offset"]["x"];
    health_offset.x += render_rect.x;
    health_offset.y = config["components"]["health_bar"]["offset"]["y"];
    health_offset.y += render_rect.y;
    Uint8 r = config["components"]["health_bar"]["color"]["r"];
    Uint8 g = config["components"]["health_bar"]["color"]["g"];
    Uint8 b = config["components"]["health_bar"]["color"]["b"];
    Uint8 a = config["components"]["health_bar"]["color"]["a"];
    health.init(health_offset, 0, 100, SDL_Color({r, g, b, a}),
                ABSOLUTE_PBTEXTYPE);

    // Mana
    SDL_Point mana_offset = {0};
    mana_offset.x = config["components"]["mana_bar"]["offset"]["x"];
    mana_offset.x += render_rect.x;
    mana_offset.y = config["components"]["mana_bar"]["offset"]["y"];
    mana_offset.y += render_rect.y;
    r = config["components"]["mana_bar"]["color"]["r"];
    g = config["components"]["mana_bar"]["color"]["g"];
    b = config["components"]["mana_bar"]["color"]["b"];
    a = config["components"]["mana_bar"]["color"]["a"];
    mana.init(mana_offset, 0, 100, SDL_Color({r, g, b, a}), ABSOLUTE_PBTEXTYPE);

    initialized = true;
}

void UserStats::loadMedia() {
    if (!initialized) {
        throw Exception("UserStats not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_USER_STATS_BASE_FP);
    health.loadMedia();
    mana.loadMedia();
}

void UserStats::update() {
    health.update(player.getHealth(), player.getMaxHealth());
    mana.update(player.getMana(), player.getMaxMana());
}

void UserStats::render() const {
    if (!initialized) {
        throw Exception("UserStats not initialized.");
    }

    // Renderizar la base
    SDL_Rect render_quad = render_rect;
    g_renderer->render(base.getTexture(), &render_quad);

    health.render();
    mana.render();
}

void UserStats::free() {
    health.free();
    mana.free();
}

UserStats::~UserStats() {
    free();
}

//-----------------------------------------------------------------------------
