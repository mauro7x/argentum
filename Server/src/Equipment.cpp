#include "../includes/Equipment.h"

Equipment::Equipment() {
    // Inicializo array de wearables con nullptr.
    for (unsigned int i = 0; i < container.size(); ++i) {
        container[i] = nullptr;
    }
}

Equipment::~Equipment() {}

Wearable* Equipment::add(Wearable* item) {
    WearableType type = item->getType();
    Wearable* prev_item = this->container[type];
    this->container[type] = item;
    return prev_item;
}

const unsigned int Equipment::getDamagePoints() const {
    if (!this->container[WEAPON])
        return 0;
    return this->container[WEAPON]->use();
}

const unsigned int Equipment::getDefensePoints() const {
    unsigned int defense_points = 0;
    for (unsigned int type = 0; type < N_WEARABLE_ITEMS; ++type) {
        if (type == WEAPON)
            continue;
        if (this->container[type])
            defense_points += this->container[type]->use();
    }
    return defense_points;
}
