#ifndef __SPELL_H__
#define __SPELL_H__

#include <string>
#include <exception>

#include "config_structs.h"

class Character; // Forward declaration p/ evitar dependencias circulares.

class UnknownSpellTypeException: public std::exception {
    public:
        virtual const char* what() const noexcept;
};

class Spell {
    protected:
        const int id;
        const std::string name;
        const unsigned int mana_usage_cost;

    public:
        Spell(const int id,
              const std::string name,
              const unsigned int mana_usage_cost);
        virtual ~Spell();

        virtual const unsigned int cast(Character& caster) = 0;
};

class SpellFactory {
    public:
        static Spell* newSpell(const SpellCfg& spell);
};

class AttackingSpell: public Spell {
    private:
        const unsigned int attack_distance;
        const unsigned int min_damage, max_damage;

    public:
        AttackingSpell(const SpellCfg& data);
        ~AttackingSpell();

        virtual const unsigned int cast(Character& caster) override;
};

class HealingSpell: public Spell {
    private:
        const unsigned int recovery_points;

    public:
        HealingSpell(const SpellCfg& data);
        ~HealingSpell();

        virtual const unsigned int cast(Character& caster) override;
};

#endif
