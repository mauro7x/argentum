#include "EntitiesContainer.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

EntitiesContainer::EntitiesContainer() {}

void EntitiesContainer::add(Entity* new_entity) {
    mEntities.push_back(new_entity);
}

void EntitiesContainer::loadMedia() const {
    for (auto it = mEntities.begin(); it != mEntities.end(); it++) {
        (*it)->loadMedia();
    }
}

void EntitiesContainer::handleEvent(const SDL_Event& e) const {
    for (auto it = mEntities.begin(); it != mEntities.end(); it++) {
        (*it)->handleEvent(e);
    }
}

void EntitiesContainer::act() const {
    for (auto it = mEntities.begin(); it != mEntities.end(); it++) {
        (*it)->act();
    }
}

void EntitiesContainer::render() const {
    for (auto it = mEntities.begin(); it != mEntities.end(); it++) {
        (*it)->render();
    }
}

void EntitiesContainer::free() {
    if (mEntities.size()) {
        std::vector<Entity*> tmp;
        for (auto it = mEntities.begin(); it != mEntities.end(); it++) {
            (*it)->free();
            delete (*it);
        }

        mEntities.swap(tmp);
    }
}

EntitiesContainer::~EntitiesContainer() {
    free();
}

//-----------------------------------------------------------------------------
