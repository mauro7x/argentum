#ifndef __KIND_H__
#define __KIND_H__

#include <string>

#include "config_structs.h"

/*
 * Clase:
 *     Mago, clerigo, paladin, guerrero
 * 
 * Aportan factores a tener en cuenta en las ecuaciones de
 * vida, mana y meditacion. Ademas, son capaces (o no) de
 * meditar y/o hacer magia, y esto ultimo se ve reflejado
 * en si puede hacer hechizos o no.
 */
class Kind {
    private:
        const unsigned int id;
        const std::string name;
        const unsigned int health;
        const unsigned int meditation;
        const unsigned int mana;

    public:
        Kind(const KindCfg& data);
        ~Kind();

        Kind(const Kind&) = delete;
        Kind& operator=(const Kind&) = delete;
        Kind(Kind&&) = delete;
        Kind& operator=(Kind&&) = delete;

        // Establecer que reciben y que devuelven.
        void canEquip();
        void canMeditate();
};

#endif
