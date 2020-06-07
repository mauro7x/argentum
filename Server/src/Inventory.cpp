#include "Inventory.h"

Inventory::Inventory(): container(N_INVENTORY_ITEMS, nullptr),
                        items_quantity(0),
                        gold_quantity(0) {}

Inventory::~Inventory() {}

Item* Inventory::gatherItem(const unsigned int position) {
    //IMPLEMENTAR!
}
const unsigned int Inventory::addItem(Item* item) {
    if (this->items_quantity >= N_INVENTORY_ITEMS) {
        throw FullInventoryException();
    }
    this->container.push_back(item);
}

const char* FullInventoryException::what() const noexcept {
    return "No puede agregar más elementos al inventario.";
}
