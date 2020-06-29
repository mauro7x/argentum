#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__

#include <array>
#include <vector>

#include "../../../Common/includes/DataStructs.h"
#include "../../../Common/includes/Inventory.h"
#include "ItemsContainer.h"
#include "Wearable.h"
#include "config_structs.h"

class Character;  // Forward declaration p/evitar circular dependences.

/*
 * Contiene los items con los que el jugador
 * esta equipado.
 * Puede tener un elemento de cada tipo:
 *  -item de ataque: Arma/Baculo
 *  -armor
 *  -helmet
 *  -shield
 */
class Equipment {
   private:
    std::array<Wearable*, N_WEARABLE_ITEMS> container;

   public:
    Equipment(const EquipmentData& init_data, ItemsContainer& items_container);
    ~Equipment();

    Equipment(const Equipment&) = delete;
    Equipment& operator=(const Equipment&) = delete;
    Equipment(Equipment&&) = delete;
    Equipment& operator=(Equipment&&) = delete;

    /*
     * Agrega un elemento al container, segun
     * la ubicacion del cuerpo.
     *
     * En caso de haber un elemento en dicha ubicacion,
     * lo pisa y retorna el anterior. Caso contrario,
     * devuelve nullptr.
     */
    Wearable* add(Wearable* item);

    /*
     * Elimina el wearable en la posición n_slot del container,
     * y lo devuelve.
     *
     * En caso de no haber wearable en dicha posición, retorna nullptr.
     *
     * Lanza InvalidEquipmentSlotNumberException si el n_slot
     * recibido como argumento es inválido.
     */
    Wearable* remove(unsigned int n_slot);

    /*
     * Vacía el equipment, dropeando todos los wearables en el vector 
     * recibido por parámetro.
     */
    void dropAll(std::vector<DroppingSlot>& dropped_items);

    /*
     * Devuelve los puntos de daño que causan
     * los elementos de ataque que el personaje lleva.
     *
     * En caso de no tener nada que pueda causar daño, devuelve 0.
     *
     * Lanza InsufficientManaException si el arma es un hechizo y
     * el jugador no tiene suficiente maná como para usarlo.
     */
    const unsigned int useAttackItem(Character& attacker);

    /*
     * Devuelve el rango de ataque del arma equipada.
     *
     * Si se trata de un arma curativa, su rango es 0, ya que se
     * aplica sobre el mismo Character.
     */
    const unsigned int getAttackRange() const;

    /*
     * Devuelve los puntos de defensa que pueden absorber
     * los elementos de defensa que el personaje tiene puestos.
     * [armadura, escudo y/o casco]
     *
     * En caso de no tener nada con lo que pueda defenderse, devuelve 0.
     */
    const unsigned int getDefensePoints(Character& defender);

    /* Devuelve si tiene un arma equipada o no */
    const bool hasAWeaponEquipped() const;

    /* Devuelve si el arma equipada es curativa o no */
    const bool isWeaponHealing() const;

    void fillBroadcastData(PlayerData& data) const;
};

class InvalidEquipmentSlotNumberException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

#endif