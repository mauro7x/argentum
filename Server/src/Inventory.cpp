#include "../includes/Inventory.h"
#include "../includes/Formulas.h"

#include <iostream> // Para testear

Inventory::Inventory(Level& character_level): 
                        items_quantity(0),
                        safe_gold(0),
                        excess_gold(0),
                        character_level(character_level) {
    for (unsigned int i = 0; i < container.size(); ++i) {
        container[i] = nullptr;
    }
    this->updateMaxAmountsOfGold();
 }

Inventory::~Inventory() {
    // Elimino los items restantes.
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (container[i]) {
            delete container[i];
        }
    }
}

void Inventory::updateMaxAmountsOfGold() {
    this->max_safe_gold = Formulas::calculateMaxSafeGold(
                            this->character_level.getLevel());
    this->max_excess_gold = Formulas::calculateMaxExcessGold(
                                this->max_safe_gold);
}

const unsigned int Inventory::getNextFreeSlot() const {
    unsigned int pos = 0;
    for (; pos < this->container.size(); ++pos) {
        if (!container[pos])
            break;
    }
    return pos;
}

const unsigned int Inventory::addItem(Item* item) {
    if (this->items_quantity >= N_INVENTORY_ITEMS)
        throw FullInventoryException();
    
    unsigned int position = getNextFreeSlot();
    this->container[position] = item;
    ++this->items_quantity;
    return position;
}

Item* Inventory::gatherItem(const unsigned int position) {
    if (position >= N_INVENTORY_ITEMS) {
        throw InvalidPositionException();
    }
    Item* item = this->container[position];
    this->container[position] = nullptr;
    --this->items_quantity;
    return item;
}

unsigned int Inventory::addGold(const unsigned int amount, 
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

unsigned int Inventory::gatherGold(const unsigned int amount,
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
    unsigned int remaining = addGold(amount, this->safe_gold, this->max_safe_gold);

    if (addGold(remaining, this->excess_gold, this->max_excess_gold) > 0) {
        // Se llego al limite de capacidad y no se pudo agregar todo el oro
        throw GoldMaximumCapacityReachedException();
    }
}

void Inventory::gatherGold(const unsigned int amount) {
    // Los max gold deben estar actualizados.
    if ((this->safe_gold + this->excess_gold) < amount) {
        throw InsufficientGoldException();
    }
    unsigned int gathered = gatherGold(amount, this->excess_gold);
    unsigned int remaining = amount - gathered;
    if (remaining) {
        gatherGold(amount - gathered, this->safe_gold);
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

void Inventory::debug() const {
    std::cout << "Inventory: [" << this->items_quantity << " elements]" << std::endl;
    for (unsigned int i = 0; i < this->container.size(); ++i) {
        if (this->container[i]) {
            std::cout << "Posicion " << i << ": ";
            std::cout << this->container[i]->what() << std::endl;
        }
    }
}
