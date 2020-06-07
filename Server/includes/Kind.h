#ifndef __KIND_H__
#define __KIND_H__

/**************************************
 * Dudas:
 * - Nombre como atributo?
 *************************************/ 

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
        const unsigned int health;
        const unsigned int meditation;
        const unsigned int mana;

    public:
        Kind(const unsigned int health,
             const unsigned int meditation,
             const unsigned int mana);
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
