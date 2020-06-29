#ifndef __ITEMS_CONTAINER_H__
#define __ITEMS_CONTAINER_H__

#include <unordered_map>
#include <vector>

#include "../../../Common/includes/types.h"  // Id declaration.
#include "Item.h"

#define GOLD_BAG_ID 2000

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

    std::vector<Id> weapons_id;
    std::vector<Id> wands_id;
    std::vector<Id> defences_id;
    std::vector<Id> potions_id;

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

    const std::vector<Id>& getWeaponsId() const;
    const std::vector<Id>& getWandsId() const;
    const std::vector<Id>& getDefencesId() const;
    const std::vector<Id>& getPotionsId() const;
};

#endif