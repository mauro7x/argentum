#include "../../includes/View/UserInventory.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UserInventory::UserInventory(const Renderer* renderer, const Player& player)
    : HUDComponent(renderer), player(player) {}

void UserInventory::init(const json& config) {
    if (initialized) {
        throw Exception("UserInventory already initialized.");
    }

    // Offsets de renderizado
    render_rect.x = config["offset"]["x"];
    render_rect.y = config["offset"]["y"];
    render_rect.w = config["w"];
    render_rect.h = config["h"];

    initialized = true;
}

void UserInventory::loadMedia() {
    if (!initialized) {
        throw Exception("UserInventory not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_USER_INVENTORY_BASE_FP);
}

void UserInventory::update() {}

void UserInventory::render() const {
    if (!initialized) {
        throw Exception("UserInventory not initialized.");
    }

    // Renderizar la base
    SDL_Rect render_quad = render_rect;
    g_renderer->render(base.getTexture(), &render_quad);
}

void UserInventory::free() {}

UserInventory::~UserInventory() {
    free();
}

//-----------------------------------------------------------------------------
