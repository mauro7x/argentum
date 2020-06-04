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
        const unsigned int life;
        const unsigned int recovery;
        const unsigned int manna;

        Race(const unsigned int life,
             const unsigned int recovery,
             const unsigned int manna);
        ~Race();

        Race(const Race&) = delete;
        Race& operator=(const Race&) = delete;
        Race(Race&&) = delete;
        Race& operator=(Race&&) = delete;

        // A priori no tienen comportamiento.
};

#endif