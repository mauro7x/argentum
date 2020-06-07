#ifndef __ITEM_H__
#define __ITEM_H__

#include "Character.h"

/*
 * Define la abstraccion Item.
 * Un Item es equipable y almacenable
 * en un inventario.
 */
class Item {
    protected:
        const unsigned int id;
        const unsigned int price;

    public:
        Item(const unsigned int id, const unsigned int price);
        virtual ~Item();

        // Poner las cosas que tienen en comun armas, hechizos y pociones.
        // Constituyen los objetos que se guardan en el inventario.

        // Cada item tiene que tener algun TIPO.

        virtual void equip(Character& character) = 0;
};

#endif
