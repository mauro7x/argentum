#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <exception>

#include "Kind.h"
#include "Race.h"
#include "Inventory.h"
#include "States.h"
#include "Equipment.h"
#include "Wearable.h"

#include "config_structs.h"

/*
 * Representa al ente manejado por el jugador.
 * 
 *  -Se caracteriza mediante una Raza y Clase.
 *  -Tiene vida y mana.
 *  -Tiene un Estado [Vivo/Muerto], segun el cual
 * puede o no realizar ciertas acciones.
 *  -Tiene un Inventario en el cual almacena Items,
 * ya sea armas, baculos, pociones, o elementos de defensa.
 *  -Tiene un equipamiento con los elementos de ataque y defensa que
 * porta.
 */
class Character {
    private:
        unsigned int health, mana;
        Race race;
        Kind kind;
        State* state;
        Inventory inventory;
        Equipment equipment;

    public:
        Character(const RaceCfg& race, const KindCfg& kind);
        ~Character();

        Character(const Character&) = delete;
        Character& operator=(const Character&) = delete;
        Character(Character&&) = delete;
        Character& operator=(Character&&) = delete;

        /*
         * Recibe la posicion del item en el inventario
         * que se quiere equipar, y lo equipa.
         * Si no hay item en dicha posicion, no hace nada.
         */
        void equip(unsigned int inventory_position);

        /*
         * Equipa un Wearable al Equipment en su slot, almacenando en el
         * inventario el Wearable anterior.
         */
        void equip(Wearable* item);

        /*
         * Toma un item y lo agrega al inventario.
         * Retorna la posicion en la que se agrego el item.
         * 
         * Lanza FullInventoryException si el inventario esta lleno
         * y no pudo agregarse.
         */
        const unsigned int takeItem(Item* item);

        /*
         * Devuelve el item en la inventory_position especificada.
         * Si no hay un item en dicha posicion, retorna nullptr.
         * 
         * Lanza InvalidPositionException si la posicion
         * especificada es invalida (fuera de rango).
         */
        Item* dropItem(const unsigned int inventory_position);

        /*
         * Efectua la accion curativa de las pociones de mana.
         * Aumenta los puntos de mana en los points especificados,
         * siempre y cuando el limite de mana no se supere.
         */
        void recoverMana(const unsigned int points);

        /*
         * Efectua la accion curativa de las pociones/hechizos de vida.
         * Aumenta los puntos de vida en los points especificados,
         * siempre y cuando el limite de health no se supere.
         */
        void recoverHealth(const unsigned int points);

        /*
         * Si hay suficiente mana, consume mana_points de mana.
         * 
         * Lanza InsufficientManaException si no hay suficiente mana.
         */
        void consumeMana(const unsigned int mana_points);

        /*
         * Lanza Exception si el Kind no puede hacer magia.
         */
        void doMagic();

        void debug();
};

class InsufficientManaException: std::exception {
    public:
        virtual const char* what() const noexcept;
};

#endif
