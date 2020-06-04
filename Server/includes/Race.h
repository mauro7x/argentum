#ifndef RACE_H
#define RACE_H

/*
 * Raza:
 *      Humanos, elfos, enanos, gnomos.
 * 
 * No tienen comportamiento alguno, simplemente
 * aportan factores a tener en cuenta en el calculo
 * de las ecuaciones de mana y vida.
 */
struct Race {
        unsigned int life;
        unsigned int recovery;
        unsigned int manna;

        Race(unsigned int life,
             unsigned int recovery,
             unsigned int manna);
        ~Race();

        Race(const Race&) = delete;
        Race& operator=(const Race&) = delete;
        Race(Race&&) = delete;
        Race& operator=(Race&&) = delete;

        // A priori no tienen comportamiento.
};

#endif