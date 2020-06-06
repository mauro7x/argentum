#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Kind.h"
#include "Race.h"
#include "Inventory.h"
#include "States.h"
#include "Equipment.h"
#include "Weapon.h"
#include "Wand.h"

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
        unsigned int health, manna;
        Race race;
        Kind kind;
        State* state;
        Inventory inventory;
        Equipment equipment;

    public:
        Character();
        ~Character();

        Character(const Character&) = delete;
        Character& operator=(const Character&) = delete;
        Character(Character&&) = delete;
        Character& operator=(Character&&) = delete;

        // Metodos/Acciones.
        void equip(unsigned int inventory_position);
        void equip(Weapon* weapon);
        void equip(Wand* wand);

        void recoverManna(unsigned int points); // Pociones
        void recoverHealth(unsigned int points);  // Pociones

        const unsigned int getMannaPoints() const; // Baculos verifican si tiene suficientes puntos antes de ser usados.
};

#endif