#ifndef __ITEMS_CONTAINER_H__
#define __ITEMS_CONTAINER_H__

#include <unordered_map>

#include "../../../Common/includes/types.h" // Id declaration.

#include "Item.h"

/*
 * Contenedor de Items del juego.
 * 
 * A partir de los archivos de configuración de armas,
 * báculos, defensas y pociones, instancia los mismos 
 * en un unordered_map, para tener un acceso a los mismos 
 * mapeado por Id.
 */
class ItemsContainer {

    private:
        std::unordered_map<Id, Item*> container;
    
    public:
        /*
         * Lee los JSON, los parsea, y crea todos
         * los items, reservando memoria para ellos.
         */
        ItemsContainer();

        /*
         * Libera la memoria reservada para los items.
         */
        ~ItemsContainer();

        ItemsContainer(const ItemsContainer&) = delete;
        ItemsContainer& operator=(const ItemsContainer&) = delete;
        ItemsContainer(ItemsContainer&& other) = delete;
        ItemsContainer& operator=(ItemsContainer&& other) = delete;

        /*
         * Devuelve el Item* que hay mapeado en item_id.
         * 
         * Lanza Exception si el id no corresponde a ningún item.
         */
        Item* operator[](const Id item_id);
};

#endif