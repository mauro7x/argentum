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

UnitContainer::UnitContainer() {}

void UnitContainer::add(Unit* new_unit) {
    content.push_back(new_unit);
    content.shrink_to_fit();
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
