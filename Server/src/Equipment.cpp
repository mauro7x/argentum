#include "Equipment.h"

Equipment::Equipment(): container(N_WEARABLE_ITEMS, nullptr) {}

Equipment::~Equipment() {}

Wearable* Equipment::add(Wearable* item) {
    Wearable* prev_item = this->container[item->getType()];
    this->container[item->getType()] = item;
    return prev_item;
}

const unsigned int Equipment::getDamagePoints() const {
    if (!this->container[WEAPON])
        return 0;
    return this->container[WEAPON]->use();
}

const unsigned int Equipment::getDefensePoints() const {
    unsigned int defense_points = 0;
    for (int i = 0; i < N_WEARABLE_ITEMS; ++i) {
        if (i == WEAPON)
            continue;
        if (this->container[i])
            defense_points += this->container[i]->use();
    }
    return defense_points;
}
