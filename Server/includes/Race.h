#ifndef __RACE_H__
#define __RACE_H__

#include <string>

#include "config_structs.h"

/*
 * Raza:
 *      Humanos, elfos, enanos, gnomos.
 * 
 * No tienen comportamiento alguno, simplemente
 * aportan factores a tener en cuenta en el calculo
 * de las ecuaciones de mana y vida.
 */
struct Race {
        const unsigned int id;
        const std::string name;
        const unsigned int health;
        const unsigned int recovery;
        const unsigned int mana;

        Race(const RaceCfg& data);
        ~Race();
};

#endif
