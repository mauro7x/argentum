#ifndef __SPELL_H__
#define __SPELL_H__
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "Response.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------
class Character;  // Forward declaration p/ evitar dependencias circulares.
//-----------------------------------------------------------------------------

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
    WeaponType weapon_type;

   public:
    Spell(const int id, const std::string name,
          const unsigned int mana_usage_cost, const unsigned int range,
          const unsigned int cooldown, const WeaponType weapon_type);
    virtual ~Spell();

    /* Lanzamiento del hechizo. Retorna los puntos de ataque que provoca. */
    virtual Response cast(Character& caster, int& points) = 0;

    /* Devuelve el tipo de arma del hechizo. */
    const WeaponType getWeaponType() const;

    /* Retorna el rango del hechizo. */
    const unsigned int getRange() const;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/*
 * Crea un hechizo segun el tipo especificado en
 * SpellCfg.
 */
class SpellFactory {
   public:
    static Spell* newSpell(const SpellCfg& data);
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

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
    Response cast(Character& caster, int& points) override;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

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
    Response cast(Character& caster, int& points) override;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
