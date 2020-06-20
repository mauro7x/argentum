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

    offset_x = config["offset"]["x"];
    offset_y = config["offset"]["y"];
    w = config["w"];
    h = config["h"];

    initialized = true;
}

void UserInventory::loadMedia() {
    if (!initialized) {
        throw Exception("UserInventory not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_USER_INVENTORY_BASE_FP);
}

void UserInventory::render() const {
    if (!initialized) {
        throw Exception("UserInventory not initialized.");
    }

    // Renderizar
    SDL_Rect render_quad = {offset_x, offset_y, w, h};
    g_renderer->render(base.getTexture(), &render_quad);
}

UserInventory::~UserInventory() {}

//-----------------------------------------------------------------------------
