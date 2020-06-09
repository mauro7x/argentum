#include "../includes/Equipment.h"
#include "../includes/Character.h"

Equipment::Equipment() {
    // Inicializo array de wearables con nullptr.
    for (unsigned int i = 0; i < container.size(); ++i) {
        container[i] = nullptr;
    }
}

Equipment::~Equipment() {
    // Elimino los items restantes.
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (container[i]) {
            delete container[i];
        }
    }
}

Wearable* Equipment::add(Wearable* item) {
    WearableType type = item->getType();
    Wearable* prev_item = this->container[type];
    this->container[type] = item;
    return prev_item;
}

const unsigned int Equipment::getAttackPoints(Character& attacker) {
    if (!this->container[WEAPON]) {
        return 0;
    }
    return this->container[WEAPON]->use(attacker);
}

const unsigned int Equipment::getDefensePoints(Character& defender) {
    unsigned int defense_points = 0;
    // Sumo los puntos de defensa de cada wearables de defensa que lleva puesto,
    // que resultan ser todos menos WEAPON.
    for (unsigned int type = 0; type < N_WEARABLE_ITEMS; ++type) {
        if (type == WEAPON) {
            continue;
        }
        if (this->container[type]) {
            defense_points += this->container[type]->use(defender);
        }
    }
    return defense_points;
}
