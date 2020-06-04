#ifndef PROFESSION_H
#define PROFESSION_H

/*
 * Clase:
 *     Mago, clerigo, paladin, guerrero
 * 
 * Aportan factores a tener en cuenta en las ecuaciones de
 * vida, mana y meditacion. Ademas, son capaces (o no) de
 * meditar y/o hacer magia, y esto ultimo se ve reflejado
 * en si puede hacer hechizos o no.
 */
class Profession {
    private:
        const unsigned int life;
        const unsigned int meditation;
        const unsigned int manna;

    public:
        Profession();
        ~Profession();

        Profession(const Profession&) = delete;
        Profession& operator=(const Profession&) = delete;
        Profession(Profession&&) = delete;
        Profession& operator=(Profession&&) = delete;

        // Establecer que reciben y que devuelven.
        void canEquip();
        void canMeditate(); 
};

#endif