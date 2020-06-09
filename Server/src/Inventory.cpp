#include "../includes/Inventory.h"

Inventory::Inventory(): items_quantity(0),
                        gold_quantity(0) {
    for (unsigned int i = 0; i < container.size(); ++i) {
        container[i] = nullptr;
    }
 }

Inventory::~Inventory() {
    // Elimino los items restantes.
    for (unsigned int i = 0; i < container.size(); ++i) {
        if (container[i]) {
            delete container[i];
        }
    }
}

const unsigned int Inventory::getNextFreeSlot() const {
    unsigned int pos = 0;
    for (; pos < this->container.size(); ++pos) {
        if (!container[pos])
            break;
    }
    return pos;
}

Item* Inventory::gatherItem(const unsigned int position) {
    if (position >= N_INVENTORY_ITEMS) {
        throw InvalidPositionException();
    }
    Item* item = this->container[position];
    this->container[position] = nullptr;
    return item;
}

void Inventory::gatherGold(const unsigned int amount) {
    if (this->gold_quantity < amount) {
        throw InsufficientGoldException();
    }
    this->gold_quantity -= amount;
}

const unsigned int Inventory::addItem(Item* item) {
    if (this->items_quantity >= N_INVENTORY_ITEMS)
        throw FullInventoryException();
    
    unsigned int position = getNextFreeSlot();
    this->container[position] = item;
    return position;
}

void Inventory::addGold(const unsigned int amount) {
    this->gold_quantity += amount;
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
