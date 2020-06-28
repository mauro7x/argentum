#include "../../includes/Model/Equipment.h"

#include <iostream>  // Para testear

#include "../../includes/Model/Character.h"

Equipment::Equipment(const EquipmentData& init_data,
                     ItemsContainer& items_container) {
    // Inicializo array de wearables con los datos o nullptr.
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (!init_data[i]) {
            this->container[i] = nullptr;
        } else {
            this->container[i] = (Wearable*)items_container[init_data[i]];
        }
    }
}

Equipment::~Equipment() {}

Wearable* Equipment::add(Wearable* item) {
    WearableType type = item->getType();
    Wearable* prev_item = this->container[type];
    this->container[type] = item;
    return prev_item;
}

Wearable* Equipment::remove(unsigned int n_slot) {
    if (n_slot > this->container.size())
        throw InvalidEquipmentSlotNumberException();
    
    Wearable* item = this->container[n_slot];
    this->container[n_slot] = nullptr;
    return item;
}

void Equipment::dropAll(std::vector<DroppingSlot>& dropped_items) {
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (!container[i])
            continue;

        Id item_id = container[i]->getId();
        dropped_items.emplace_back(item_id, 1);
        container[i] = nullptr;
    }
}

const unsigned int Equipment::useAttackItem(Character& attacker) {
    if (!this->container[WEAPON]) {
        return 0;
    }
    return this->container[WEAPON]->use(attacker);
}

const unsigned int Equipment::getAttackRange() const {
    if (!this->container[WEAPON]) {
        return 0;
    }
    return this->container[WEAPON]->getRange();
}

const unsigned int Equipment::getDefensePoints(Character& defender) {
    unsigned int defense_points = 0;
    /* Sumo los puntos de defensa de cada wearables de defensa
    que lleva puesto,que resultan ser todos menos WEAPON.*/
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

const bool Equipment::isWeaponHealing() const {
    if (!this->container[WEAPON])
        return false;
        
    return this->container[WEAPON]->isHealing();
}

void Equipment::fillBroadcastData(PlayerData& data) const {
    for (int i = 0; i < N_WEARABLE_ITEMS; ++i) {
        if (!this->container[i]) {
            data.equipment[i] = 0;
        } else {
            data.equipment[i] = this->container[i]->getId();
        }
    }
}

const char* InvalidEquipmentSlotNumberException::what() const noexcept {
    return "El numero de slot del equipamiento especificado es inválido.";
}

