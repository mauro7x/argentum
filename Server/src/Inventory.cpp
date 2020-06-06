#include "Inventory.h"

Inventory::Inventory(): container(N_INVENTORY_ITEMS, nullptr),
                        items_quantity(0),
                        gold_quantity(0) {}

Inventory::~Inventory() {}

Item* Inventory::gatherItem(const unsigned int position) {
    //IMPLEMENTAR!
}
const unsigned int Inventory::addItem() {
    //IMPLEMENTAR!
}
