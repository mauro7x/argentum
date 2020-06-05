#ifndef __RACE_H__
#define __RACE_H__

/**************************************
 * Dudas:
 * - Nombre como atributo?
 *************************************/ 

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
        // A priori no tienen comportamiento.
};

#endif
