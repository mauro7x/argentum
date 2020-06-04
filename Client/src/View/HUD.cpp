#include "../../includes/View/HUD.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

HUDProxy::HUDProxy(SDL_Renderer* renderer)
    : g_renderer(renderer), texture(renderer) {}

void HUDProxy::loadMedia() {
    texture.loadFromFile(HUD_PATH + std::string("hud.png"));
}

void HUDProxy::render() const {  // renderizar la interfaz
    texture.render(0, 0);
}

HUDProxy::~HUDProxy() {}

//-----------------------------------------------------------------------------
