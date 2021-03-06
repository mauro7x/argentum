#include "../../includes/Model/Equipment.h"
//-----------------------------------------------------------------------------
#include "../../includes/Model/Character.h"
//-----------------------------------------------------------------------------

Equipment::Equipment(const EquipmentData& init_data,
                     ItemsContainer& items_container)
    : fist((Wearable*)items_container[FIST_ID]) {
    // Inicializo array de wearables con los datos o nullptr.
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (!init_data[i])
            this->container[i] = nullptr;
        else
            this->container[i] = (Wearable*)items_container[init_data[i]];
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
        return nullptr;

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

Response Equipment::useAttackItem(Character& attacker, int& points) {
    if (!this->container[WEAPON])
        return fist->use(attacker, points);

    return this->container[WEAPON]->use(attacker, points);
}

const unsigned int Equipment::getAttackRange() const {
    if (!this->container[WEAPON])
        return fist->getRange();

    return this->container[WEAPON]->getRange();
}

const unsigned int Equipment::getDefensePoints(Character& defender) {
    unsigned int defense_points = 0;
    int points = 0;

    /* Sumo los puntos de defensa de cada wearables de defensa
    que lleva puesto, que resultan ser todos menos WEAPON. */
    for (unsigned int type = 0; type < N_WEARABLE_ITEMS; ++type) {
        if (type == WEAPON || !this->container[type])
            continue;

        this->container[type]->use(defender, points);
        defense_points += points;
    }

    return defense_points;
}

const WeaponType Equipment::getWeaponType() const {
    if (!this->container[WEAPON])
        return fist->getWeaponType();

    return this->container[WEAPON]->getWeaponType();
}

void Equipment::fillBroadcastData(PlayerData& data) const {
    for (int i = 0; i < N_WEARABLE_ITEMS; ++i) {
        if (!this->container[i])
            data.equipment[i] = 0;
        else
            data.equipment[i] = this->container[i]->getId();
    }
}

void Equipment::fillPersistenceData(CharacterCfg& data) const {
    for (int i = 0; i < N_WEARABLE_ITEMS; ++i) {
        if (!this->container[i])
            data.equipment[i] = 0;
        else
            data.equipment[i] = this->container[i]->getId();
    }
}

//-----------------------------------------------------------------------------
