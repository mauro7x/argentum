#include "Inventory.h"

Inventory::Inventory(): items_quantity(0),
                        gold_quantity(0) {
    for (unsigned int i = 0; i < container.size(); ++i) {
        container[i] = nullptr;
    }
 }

Inventory::~Inventory() {}

Item* Inventory::gather(const unsigned int position) {
    //IMPLEMENTAR!
}
const unsigned int Inventory::add(Item* item) {
    if (this->items_quantity >= N_INVENTORY_ITEMS)
        throw FullInventoryException();

    //this->container.push_back(item);
}

const char* FullInventoryException::what() const noexcept {
    return "No puede agregar más elementos al inventario.";
}
