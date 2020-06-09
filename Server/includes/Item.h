#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>

class Character; // Evito dependencias circulares debido a double dispatch

/*
 * Define la abstraccion Item.
 * Un Item es equipable y almacenable
 * en un inventario.
 */
class Item {
    protected:
        const unsigned int id;
        const std::string name;
        const unsigned int price;

    public:
        Item(const unsigned int id, const std::string name,
             const unsigned int price);
        virtual ~Item();

        // Poner las cosas que tienen en comun armas, hechizos y pociones.
        // Constituyen los objetos que se guardan en el inventario.

        // Cada item tiene que tener algun TIPO.

        virtual void equip(Character& equipper) = 0;
        
        virtual const std::string what() const;
};

#endif
