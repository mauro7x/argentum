#ifndef __SPELL_H__
#define __SPELL_H__

#include <exception>
#include <string>

#include "config_structs.h"

class Character;  // Forward declaration p/ evitar dependencias circulares.

class UnknownSpellTypeException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

/*
 * Representa a los hechizos que son lanzados por los Wand.
 * Se identifican mediante un id y nombre, y para usarlos,
 * se requiere mana_usage_cost.
 *
 * Hay dos tipos: de ataque, que son aplicados sobre otros
 * jugadores, y curativos, que son aplicados sobre el caster.
 */
class Spell {
   protected:
    const int id;
    const std::string name;
    const unsigned int mana_usage_cost;
    const unsigned int range;
    const unsigned int cooldown;

   public:
    Spell(const int id, const std::string name,
          const unsigned int mana_usage_cost, const unsigned int range,
          const unsigned int cooldown);
    virtual ~Spell();

    
    /* Lanzamiento del hechizo. Retorna los puntos de ataque que provoca. */
    virtual const unsigned int cast(Character& caster) = 0;

    /* Devuelve si el hechizo es curativo o no */
    virtual const bool isHealing() const = 0;

    /* Retorna el rango del hechizo. */
    const unsigned int getRange() const;
};

/*
 * Crea un hechizo segun el tipo especificado en
 * SpellCfg.
 */
class SpellFactory {
   public:
    static Spell* newSpell(const SpellCfg& data);
};

class AttackingSpell : public Spell {
   private:
    const unsigned int min_damage, max_damage;

   public:
    AttackingSpell(const SpellCfg& data);
    ~AttackingSpell();

    /*
     * Si el caster tiene mana_usage_cost de mana, se lanza
     * el hechizo y devuelve los puntos de ataque del mismo.
     *
     * Si no tiene suficiente mana, lanza InsufficientManaException.
     */
    const unsigned int cast(Character& caster) override;

    const bool isHealing() const override;
};

class HealingSpell : public Spell {
   private:
    const unsigned int recovery_points;

   public:
    HealingSpell(const SpellCfg& data);
    ~HealingSpell();

    /*
     * Si el caster tiene mana_usage_cost de mana, se lanza
     * el hechizo devuelve los puntos de curación recovery_points.
     *
     * Si no tiene suficiente mana, lanza InsufficientManaException.
     */
    const unsigned int cast(Character& caster) override;

    const bool isHealing() const override;
};

#endif
