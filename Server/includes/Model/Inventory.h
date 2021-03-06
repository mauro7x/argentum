#ifndef __INVENTORY_H__
#define __INVENTORY_H__
//-----------------------------------------------------------------------------
#include <array>
#include <cstdio>
#include <exception>
#include <map>
#include <vector>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/DataStructs.h"
#include "../../../Common/includes/Inventory.h"
//-----------------------------------------------------------------------------
#include "Formulas.h"
#include "Item.h"
#include "ItemsContainer.h"
#include "Level.h"
#include "Response.h"
#include "Slot.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------

/*
 * El Inventario es un contenedor que guarda Items,
 * que pueden ser armas, hechizos o pociones.
 * Se accede a el mediante indices.
 * Puede contener hasta INVENTORY_MAX_ITEMS elementos.
 */
class Inventory {
   private:
    std::array<Slot, N_INVENTORY_SLOTS> slots;
    std::map<Id, uint8_t> id_slot_map;
    uint8_t occupied_slots;

    unsigned int safe_gold, excess_gold;
    unsigned int max_safe_gold, max_excess_gold;
    Level& character_level;

    Formulas& formulas;

    /*
     * Devuelve la el indice del menor slot libre del container del inventario.
     */
    const uint8_t _getNextFreeSlot() const;

    /*
     * Agrega la cantidad de oro especificada en slot_gold, sin superar el
     * slot_max_gold. Si la cantidad sobrepasa slot_max_gold, se guarda lo que
     * se puede, hasta llegar al limite.
     *
     * Retorna la cantidad de oro que no se pudo guardar.
     */
    unsigned int _addGold(const uint32_t amount, unsigned int& slot_gold,
                          unsigned int& slot_max_gold);

    /*
     * Retira la cantidad de oro especificada de slot_gold, en caso de haber
     * suficiente. 
     * 
     * Si no hay suficiente, retira todo lo que hay.
     *
     * Retorna la cantidad de oro que se pudo retirar.
     */
    unsigned int _gatherGold(const uint32_t amount, unsigned int& slot_gold);

   public:
    Inventory(const InventoryData& init_data, uint32_t init_safe_gold,
              uint32_t init_excess_gold, Level& level,
              ItemsContainer& items_container, Formulas& formulas);
    ~Inventory();

    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;
    Inventory(Inventory&&) = delete;
    Inventory& operator=(Inventory&&) = delete;

    void updateMaxAmountsOfGold();

    /*
     * Obtiene la cantidad especificada del item en la n_slot pasada por
     * parámetro.
     *
     * Si la cantidad de items en el slot es menor que amount, se configura en
     * amount la cantidad real droppeada.
     *
     * En caso de no haber item alguno, o bien si la posicion especificada es
     * inválida (fuera de rango), retorna nullptr.
     */
    Item* gatherItem(const uint8_t n_slot, unsigned int& amount);

    /*
     * Obtiene amount de gold del inventario.
     *
     * Devuelve Response ERROR si la cantidad de oro presente en el inventario
     * es menor a amount.
     */
    Response gatherGold(const uint32_t amount);

    /*
     * Agrega amount items (del mismo tipo) al inventario.
     *
     * Devuelve Response ERROR si el inventario está lleno y no se puede
     * agregar.
     */
    Response addItem(Item* item, const unsigned int amount);

    /*
     * Agrega amount de gold al inventario, llenando primero el oro seguro y
     * luego el oro en exceso, hasta llegar a sus respectivos limites.
     *
     * Setea en amount el oro efectivamente agregado.
     *
     * Devuelve Responser ERROR si no se pudo guardar todo el oro por llegar al
     * limite de capacidad.
     */
    Response addGold(uint32_t& amount);

    /*
     * Vacía el inventario, dropeando todos los items en el vector recibido por
     * parámetro.
     */
    void dropAll(std::vector<DroppingSlot>& dropped_items);

    /*
     * Llena los campos safe_gold, excess_gold e inventory de PlayerData para su
     * broadcast.
     */
    void fillBroadcastData(PlayerData& data) const;

    void fillPersistenceData(CharacterCfg& data) const;
};

#endif