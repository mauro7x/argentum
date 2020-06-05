#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "CharacterClass.h"
#include "Race.h"
#include "Inventory.h"
#include "State.h"
#include "Equipment.h"

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
        unsigned int life, manna;
        Race& race;
        CharacterClass& character_class;
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
};

#endif