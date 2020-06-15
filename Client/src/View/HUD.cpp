#include "../../includes/View/HUD.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

HUDProxy::HUDProxy(const Renderer* renderer)
    : initialized(false), g_renderer(renderer) {}

void HUDProxy::init(const json& config) {
    if (initialized) {
        throw Exception("HUD already initialized.");
    }

    offset_x = config["offset"]["x"];
    offset_y = config["offset"]["y"];
    w = config["w"];
    h = config["h"];

    initialized = true;
}

void HUDProxy::loadMedia() {
    if (!initialized) {
        throw Exception("HUD not initialized.");
    }

    texture.loadFromFile(g_renderer, HUD_PATH + std::string("HUD.png"));
}

void HUDProxy::render() const {
    if (!initialized) {
        throw Exception("HUD not initialized.");
    }

    SDL_Rect render_quad = {offset_x, offset_y, w, h};
    g_renderer->render(texture.getTexture(), &render_quad);
}

HUDProxy::~HUDProxy() {}

//-----------------------------------------------------------------------------
