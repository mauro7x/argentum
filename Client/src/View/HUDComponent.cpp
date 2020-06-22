#include "../../includes/View/HUDComponent.h"

//-----------------------------------------------------------------------------
// Métodos privados

void HUDComponent::_center(SDL_Point& texture_pos, const Texture& texture,
                           const SDL_Rect& rect) {
    fprintf(stderr, "rect: x = %i, y = %i, w = %i, h = %i\n", rect.x, rect.y,
            rect.w, rect.h);
    texture_pos.x = rect.x + (rect.w - texture.getWidth()) / 2;
    texture_pos.y = rect.y + (rect.h - texture.getHeight()) / 2;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

HUDComponent::HUDComponent(const Renderer* renderer)
    : initialized(false), g_renderer(renderer) {}

void HUDComponent::update() {}

void HUDComponent::free() {}

HUDComponent::~HUDComponent() {}

//-----------------------------------------------------------------------------
