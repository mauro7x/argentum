#include "../includes/Inventory.h"
#include "../includes/Formulas.h"

#include <iostream> // Para testear

Inventory::Inventory(Level& character_level): 
                        occupied_slots(0),
                        safe_gold(0),
                        excess_gold(0),
                        character_level(character_level) {
    for (unsigned int i = 0; i < slots.size(); ++i) {
        slots[i] = std::move(Slot());
    }
    
    // Establezco maximos de oro en exceso y seguro.
    this->updateMaxAmountsOfGold();
 }

Inventory::~Inventory() {}

void Inventory::updateMaxAmountsOfGold() {
    this->max_safe_gold = Formulas::calculateMaxSafeGold(
                            this->character_level.getLevel());
    this->max_excess_gold = Formulas::calculateMaxExcessGold(
                                this->max_safe_gold);
}

const unsigned int Inventory::_getNextFreeSlot() const {
    unsigned int n_slot = 0;

    for (; n_slot < this->slots.size(); ++n_slot) {
        if (this->slots[n_slot].isEmpty())
            break;
    }

    return n_slot;
}

const unsigned int Inventory::addItem(Item* item) {
    if (this->occupied_slots >= this->slots.size())
        throw FullInventoryException();

    unsigned int n_slot;
    const Id item_id = item->getId();
    
    // Me fijo si ya hay algún slot de este item.
    if (this->id_slot_map.count(item_id)) {
        n_slot = this->id_slot_map[item_id];
        this->slots[n_slot].addItem(item);
        return n_slot;
    }

    // Busco un slot libre y lo agrego.
    n_slot = _getNextFreeSlot();

    this->slots[n_slot].addItem(item);

    this->id_slot_map[item_id] = n_slot;
    ++this->occupied_slots;

    return n_slot;
}

Item* Inventory::gatherItem(const unsigned int n_slot) {
    if (n_slot >= N_INVENTORY_SLOTS)
        throw InvalidPositionException();

    Slot& slot = this->slots[n_slot];

    if (slot.isEmpty())
        return nullptr;
    
    Item* item = slot.takeItem();

    if (slot.isEmpty()) {
        this->id_slot_map.erase(item->getId());
        --this->occupied_slots;
    }
    
    return item;
}

unsigned int Inventory::_addGold(const unsigned int amount, 
                                unsigned int& slot_gold,
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

unsigned int Inventory::_gatherGold(const unsigned int amount,
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

void Inventory::addGold(const unsigned int amount) {
    unsigned int remaining = _addGold(amount, this->safe_gold, this->max_safe_gold);

    if (_addGold(remaining, this->excess_gold, this->max_excess_gold) > 0) {
        // Se llego al limite de capacidad y no se pudo agregar todo el oro
        throw GoldMaximumCapacityReachedException();
    }
}

void Inventory::gatherGold(const unsigned int amount) {
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

const char* FullInventoryException::what() const noexcept {
    return "No puede agregar más elementos al inventario.";
}

const char* InvalidPositionException::what() const noexcept {
    return "La posición del inventario especificada es inválida.";
}

const char* InsufficientGoldException::what() const noexcept {
    return "No tienes suficiente oro en el inventario.";
}

const char* GoldMaximumCapacityReachedException::what() const noexcept {
    return "No puedes recoger más oro. Límite de capacidad alcanzado.";
}

// void Inventory::debug() const {
//     std::cout << "Inventory: [" << this->items_quantity << " elements]" << std::endl;
//     for (unsigned int i = 0; i < this->container.size(); ++i) {
//         if (this->container[i]) {
//             std::cout << "Posicion " << i << ": ";
//             std::cout << this->container[i]->what() << std::endl;
//         }
//     }
// }
