#ifndef __CHARACTER_CLASS_H__
#define __CHARACTER_CLASS_H__

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
class CharacterClass {
    private:
        const unsigned int life;
        const unsigned int meditation;
        const unsigned int manna;

    public:
        CharacterClass(const unsigned int life,
                   const unsigned int meditation,
                   const unsigned int manna);
        ~CharacterClass();

        CharacterClass(const CharacterClass&) = delete;
        CharacterClass& operator=(const CharacterClass&) = delete;
        CharacterClass(CharacterClass&&) = delete;
        CharacterClass& operator=(CharacterClass&&) = delete;

        // Establecer que reciben y que devuelven.
        void canEquip();
        void canMeditate();
};

#endif
