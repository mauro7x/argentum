#include "../../includes/View/UnitContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

void UnitContainer::_free() {
    if (content.size()) {
        std::vector<Unit*> tmp;
        for (auto it = content.begin(); it != content.end(); it++) {
            delete (*it);
        }

        content.swap(tmp);
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UnitContainer::UnitContainer(const Renderer* renderer,
                             const UnitSpriteContainer& unit_sprites)
    : g_renderer(renderer), g_unit_sprites(unit_sprites) {}

void UnitContainer::init(const int tile_w, const int tile_h) {
    this->tile_w = tile_w;
    this->tile_h = tile_h;
}

void UnitContainer::create() {
    /* por ahora, hardcodear la creación de una unidad */
}

void UnitContainer::handleEvent(const SDL_Event& e) const {
    /* Broadcasteamos el evento */
    for (auto it = content.begin(); it != content.end(); it++) {
        (*it)->handleEvent(e);
    }
}

UnitContainer::~UnitContainer() {
    _free();
}

//-----------------------------------------------------------------------------
