#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__

#include <array>
#include "Wearable.h"

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
        std::array<Wearable*, N_WEARABLE_ITEMS> container; //Container de 4 posiciones
    
    public:
        Equipment();
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
         * Devuelve los puntos de daño que causan
         * los elementos de ataque que el personaje lleva.
         * 
         * En caso de no tener nada que pueda causar daño, devuelve 0.
         */
        const unsigned int getDamagePoints() const;

        /*
         * Devuelve los puntos de defensa que pueden absorber
         * los elementos de defensa que el personaje tiene puestos.
         * [armadura, escudo y/o casco]
         * 
         * En caso de no tener nada con lo que pueda defenderse, devuelve 0.
         */
        const unsigned int getDefensePoints() const;
};

#endif