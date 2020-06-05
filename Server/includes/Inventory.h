#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <vector>
#include "Item.h"

/*
 * El Inventario es un contenedor que guarda Items,
 * que pueden ser armas, hechizos o pociones.
 * Se accede a el mediante indices. 
 * Puede contener hasta INVENTORY_MAX_ITEMS elementos.
 */
class Inventory {
    private:
        std::vector<Item*> container;
        unsigned int items_quantity;

        unsigned int gold_quantity;

    public:
        Inventory();
        ~Inventory();

        Inventory(const Inventory&) = delete;
        Inventory& operator=(const Inventory&) = delete;
        Inventory(Inventory&&) = delete;
        Inventory& operator=(Inventory&&) = delete;

        Item getItem();
        void addItem();
};

#endif