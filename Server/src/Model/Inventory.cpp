#include "../../includes/Model/Inventory.h"

#include <iostream>  // Para testear

#include "../../includes/Model/Formulas.h"

Inventory::Inventory(const InventoryData& init_data, uint32_t init_safe_gold,
                     uint32_t init_excess_gold, Level& character_level,
                     ItemsContainer& items_container)
    : occupied_slots(0),
      safe_gold(init_safe_gold),
      excess_gold(init_excess_gold),
      character_level(character_level) {
    for (unsigned int i = 0; i < slots.size(); ++i) {
        if (!init_data[i].item || !init_data[i].amount) {
            continue;
        }

        Id item_id = init_data[i].item;
        uint32_t amount = init_data[i].amount;

        slots[i] = std::move(Slot(items_container[item_id], amount));

        this->id_slot_map[item_id] = i;
        ++this->occupied_slots;
    }

    // Establezco maximos de oro en exceso y seguro.
    this->updateMaxAmountsOfGold();
}

Inventory::~Inventory() {}

void Inventory::updateMaxAmountsOfGold() {
    this->max_safe_gold =
        Formulas::calculateMaxSafeGold(this->character_level.getLevel());
    this->max_excess_gold =
        Formulas::calculateMaxExcessGold(this->max_safe_gold);
}

const uint8_t Inventory::_getNextFreeSlot() const {
    uint8_t n_slot = 0;

    for (; n_slot < this->slots.size(); ++n_slot) {
        if (this->slots[n_slot].isEmpty())
            break;
    }

    return n_slot;
}

const uint8_t Inventory::addItem(Item* item, const unsigned int amount) {
    uint8_t n_slot;
    const Id item_id = item->getId();

    // Me fijo si ya hay algún slot de este item.
    if (this->id_slot_map.count(item_id)) {
        n_slot = this->id_slot_map[item_id];
        this->slots[n_slot].addItem(item, amount);
        return n_slot;
    }

    if (this->occupied_slots >= this->slots.size())
        throw FullInventoryException();

    // Busco un slot libre y lo agrego.
    n_slot = _getNextFreeSlot();

    this->slots[n_slot].addItem(item, amount);

    this->id_slot_map[item_id] = n_slot;
    ++this->occupied_slots;

    return n_slot;
}

Item* Inventory::gatherItem(const uint8_t n_slot, unsigned int& amount) {
    if (n_slot >= N_INVENTORY_SLOTS)
        throw InvalidInventorySlotNumberException();

    Slot& slot = this->slots[n_slot];

    if (slot.isEmpty())
        return nullptr;

    Item* item = slot.takeItem(amount);

    if (slot.isEmpty()) {
        this->id_slot_map.erase(item->getId());
        --this->occupied_slots;
    }

    return item;
}

unsigned int Inventory::_addGold(const uint32_t amount, unsigned int& slot_gold,
                                 unsigned int& slot_max_gold) {
    unsigned int added_gold = 0;

    if (slot_gold < slot_max_gold) {
        unsigned int slot_gold_capacity = slot_max_gold - slot_gold;

        if (slot_gold_capacity >= amount) {
            slot_gold += amount;
            added_gold = amount;
        } else {
            slot_gold += slot_gold_capacity;
            added_gold = slot_gold_capacity;
        }
    }

    return amount - added_gold;
}

unsigned int Inventory::_gatherGold(const uint32_t amount,
                                    unsigned int& slot_gold) {
    unsigned int gathered_gold = 0;

    if (slot_gold >= amount) {
        gathered_gold = amount;
        slot_gold -= amount;
    } else {
        gathered_gold = slot_gold;
        slot_gold = 0;
    }

    return gathered_gold;
}

void Inventory::addGold(const uint32_t amount) {
    unsigned int remaining =
        _addGold(amount, this->safe_gold, this->max_safe_gold);

    if (_addGold(remaining, this->excess_gold, this->max_excess_gold) > 0) {
        // Se llego al limite de capacidad y no se pudo agregar todo el oro
        throw GoldMaximumCapacityReachedException();
    }
}

void Inventory::gatherGold(const uint32_t amount) {
    // Los max gold deben estar actualizados.
    if ((this->safe_gold + this->excess_gold) < amount) {
        throw InsufficientGoldException();
    }

    unsigned int gathered = _gatherGold(amount, this->excess_gold);
    unsigned int remaining = amount - gathered;

    if (remaining) {
        _gatherGold(amount - gathered, this->safe_gold);
    }
}

void Inventory::dropAll(std::vector<DroppingSlot>& dropped_items) {
    for (unsigned int i = 0; i < slots.size(); ++i) {
        Slot& slot = slots[i];
        Id item_id = slot.getItemId();

        if (!item_id)
            continue;

        uint32_t amount = slot.getAmount();

        dropped_items.emplace_back(item_id, amount);

        slot.clearSlot();

        this->id_slot_map.erase(item_id);
        --this->occupied_slots;
    }
}

void Inventory::fillBroadcastData(PlayerData& data) const {
    data.safe_gold = this->safe_gold;
    data.excess_gold = this->excess_gold;

    for (int i = 0; i < N_INVENTORY_SLOTS; ++i) {
        if (!this->slots[i].getItemId())
            data.inventory[i] = {0, 0};

        data.inventory[i] = {this->slots[i].getItemId(),
                             this->slots[i].getAmount()};
    }
}

const char* FullInventoryException::what() const noexcept {
    return "No puede agregar más elementos al inventario.";
}

const char* InvalidInventorySlotNumberException::what() const noexcept {
    return "El numero de slot del inventario especificado es inválido.";
}

const char* InsufficientGoldException::what() const noexcept {
    return "No tienes suficiente oro en el inventario.";
}

const char* GoldMaximumCapacityReachedException::what() const noexcept {
    return "No puedes recoger más oro. Límite de capacidad alcanzado.";
}
