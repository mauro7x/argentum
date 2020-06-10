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

        void equip(unsigned int inventory_position);
        void equip(Wearable* item);

        const unsigned int takeItem(Item* item);
        Item* dropItem(const unsigned int inventory_position);

        /*
         * Efectua la accion curativa de las pociones de mana.
         * Aumenta los puntos de mana en los points especificados.
         */
        void recoverMana(const unsigned int points);

        /*
         * Efectua la accion curativa de las pociones de vida.
         * Aumenta los puntos de vida en los points especificados.
         */
        void recoverHealth(const unsigned int points);

        void consumeMana(const unsigned int mana_points);

        void debug();
};

class InsufficientManaException: std::exception {
    public:
        virtual const char* what() const noexcept;
};

#endif
