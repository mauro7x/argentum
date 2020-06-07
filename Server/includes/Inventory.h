#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#define N_INVENTORY_ITEMS 10

#include <array>
#include <exception>

#include "Item.h"

class FullInventoryException: public std::exception {
    public:
        virtual const char *what() const noexcept;
};

/*
 * El Inventario es un contenedor que guarda Items,
 * que pueden ser armas, hechizos o pociones.
 * Se accede a el mediante indices. 
 * Puede contener hasta INVENTORY_MAX_ITEMS elementos.
 */
class Inventory {
    private:
        std::array<Item*, N_INVENTORY_ITEMS> container;
        unsigned int items_quantity;

        unsigned int gold_quantity;

    public:
        Inventory();
        ~Inventory();

        Inventory(const Inventory&) = delete;
        Inventory& operator=(const Inventory&) = delete;
        Inventory(Inventory&&) = delete;
        Inventory& operator=(Inventory&&) = delete;

        /*
         * Obtiene el item en la posicion especificada.
         * En caso de no haber, retorna nullptr.
         */
        Item* gather(const unsigned int position);

        /*
         * Agrega un item al inventario.
         * Retorna la posicion en la que se agrego.
         * 
         * Lanza FullInventoryException si el inventario esta
         * lleno.
         */
        const unsigned int add(Item* item);
};

#endif