#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <exception>

#include "Kind.h"
#include "Race.h"
#include "States.h"
#include "Inventory.h"
#include "Equipment.h"
#include "Level.h"
#include "Item.h"
#include "Wearable.h"

#include "config_structs.h"

/*
 * Representa al ente manejado por el jugador.
 * 
 *  -Se caracteriza mediante una Raza y Clase.
 *  -Tiene vida y mana, y sus respectivos limites.
 *  -Tiene un Estado [Vivo/Muerto], segun el cual
 * puede o no realizar ciertas acciones.
 *  -Tiene un Inventario en el cual almacena Items,
 * ya sea armas, baculos, pociones, o elementos de defensa.
 *  -Tiene un Equipamiento con los elementos de ataque y defensa que
 * porta.
 */
class Character {
    private:
        unsigned int health, mana;
        const unsigned int intelligence, constitution, strength, agility;
        unsigned int max_health, max_mana;
        Race race;
        Kind kind;
        State* state;
        Level level;
        Inventory inventory;
        Equipment equipment;

        void die();

    public:
        Character(const RaceCfg& race, const KindCfg& kind);
        ~Character();

        Character(const Character&) = delete;
        Character& operator=(const Character&) = delete;
        Character(Character&&) = delete;
        Character& operator=(Character&&) = delete;
        
        /*
         * Actualiza health y mana segun el paso del tiempo,
         * asi como sus valores maximos en funcion del nivel
         * [si cambio].
         */
        void updateStatus(const unsigned int seconds_elapsed);

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
         * Metodo llamado al usar baculos.
         */
        void doMagic();
        
        /*
         * Recibe los puntos de daño lanzados y si se trata de un
         * ataque eludible o no. Si es eludible, puede esquivarlo
         * y no recibir daño alguno (retornar 0).
         * Si no lo puede esquivar, absorbe lo que puede con su defensa.
         * 
         * Retorna los puntos de daño que efectivamente recibe.
         */
        const unsigned int receiveAttack(const unsigned int damage, 
                                         const bool eludible);

        /* IMPORTANTE: 
            - Al llamar a este metodo desde Game, se deben hacer
            verificaciones de nivel (newbie, diferencia), verificacion de rango
            (se consume el mana si el rango no es suficiente igual? y el danio es 0?),
            y si el rango es 0 es curativo a si mismo, asi que tener en cuenta.
            
            - El juego debe verificar si muere, y llamar al metodo die. */
        /*
         * Usa el arma que tiene equipada y retorna los puntos de daño
         * que genera. Si no tiene arma equipada, retorna 0.
         */
        const unsigned int attack();

        void debug();
};

class InsufficientManaException: std::exception {
    public:
        virtual const char* what() const noexcept;
};

#endif
