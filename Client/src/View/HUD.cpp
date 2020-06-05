#include "../../includes/View/HUD.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

HUDProxy::HUDProxy(const Renderer* renderer) : g_renderer(renderer) {}

void HUDProxy::loadMedia() {
    texture.loadFromFile(g_renderer, HUD_PATH + std::string("hud.png"));
}

void HUDProxy::render() const {  // renderizar la interfaz
    SDL_Rect render_quad = {0, 0, texture.getWidth(), texture.getHeight()};
    g_renderer->render(texture.getTexture(), &render_quad);
}

HUDProxy::~HUDProxy() {}

//-----------------------------------------------------------------------------
