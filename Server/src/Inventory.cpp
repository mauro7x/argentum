#include "../includes/Inventory.h"

Inventory::Inventory(): items_quantity(0),
                        gold_quantity(0) {
    for (unsigned int i = 0; i < container.size(); ++i) {
        container[i] = nullptr;
    }
 }

Inventory::~Inventory() {}

Item* Inventory::gather(const unsigned int position) {
    if (position >= N_INVENTORY_ITEMS) {
        throw InvalidPositionException();
    }
    Item* item = this->container[position];
    this->container[position] = nullptr;
    return item;
}

const unsigned int Inventory::getNextFreeSlot() const {
    unsigned int pos = 0;
    for (; pos < this->container.size(); ++pos) {
        if (!container[pos])
            break;
    }
    return pos;
}

const unsigned int Inventory::add(Item* item) {
    if (this->items_quantity >= N_INVENTORY_ITEMS)
        throw FullInventoryException();
    
    unsigned int position = getNextFreeSlot();
    this->container[position] = item;
    return position;
}

const char* FullInventoryException::what() const noexcept {
    return "No puede agregar más elementos al inventario.";
}

const char* InvalidPositionException::what() const noexcept {
    return "La posición del inventario especificada es inválida.";
}
