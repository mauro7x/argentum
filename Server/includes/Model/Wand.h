#ifndef __WAND_H__
#define __WAND_H__

#include "Wearable.h"
#include "Spell.h"
#include "config_structs.h"

/*
 * Representa un báculo/vara.
 * 
 * Se caracteriza por lanzar un spell (hechizo) que puede
 * ser de ataque (dañino) o curativo.
 * 
 * Es menester contar con suficiente maná para lanzar el hechizo.
 * 
 * Cada hechizo tiene su propio rango.
 */
class Wand: public Wearable {
    private:
        Spell* spell;
        
    public:
         Wand(const WandCfg& wand_data, 
              const SpellCfg& spell_data);
        ~Wand();

        Wand(const Wand&) = delete;
        Wand& operator=(const Wand&) = delete;
        Wand(Wand&&) = delete;
        Wand& operator=(Wand&&) = delete;
        
        /*
         * Castea el spell.
         * 
         * Retorna los puntos de ataque/curación que provoca el hechizo.
         * 
         * Lanza InsufficientManaException si el user no tiene
         * suficiente mana para castearlo.
         * 
         *       KindCantDoMagicException si el user no puede hacer magia
         * debido a su kind.
         */
        virtual const unsigned int use(Character& user) override;

        /*
         * Devuelve el rango del hechizo.
         */
        const unsigned int getRange() const override;

        /* Devuelve si el hechizo es curativo o no */
        const bool isHealing() const override;
};

#endif
