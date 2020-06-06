#ifndef __ITEM_H__
#define __ITEM_H__

#include "Character.h"

/*
 * Define la abstraccion Item.'
 * Un Item tiene un uso, y es almacenable
 * en un inventario.
 */
class Item {
    protected:
        const unsigned int id;
        const unsigned int buy_price, sell_price;

    public:
        Item(const unsigned int id, const unsigned int buy_price, 
             const unsigned int sell_price);
        // Poner las cosas que tienen en comun armas, hechizos y pociones.
        // Constituyen los objetos que se guardan en el inventario.

        // Cada item tiene que tener algun TIPO.

        virtual void equip(Character& character) = 0;
        virtual void use(Character& character) = 0;
};

#endif