#include "../../includes/Model/Spell.h"
//-----------------------------------------------------------------------------
#include "../../../Common/includes/RandomNumberGenerator.h"
#include "../../includes/Model/Character.h"  // Evito circular dependences.
//-----------------------------------------------------------------------------

Spell::Spell(const int id, const std::string name,
             const unsigned int mana_usage_cost, const unsigned int range,
             const unsigned int cooldown, const WeaponType weapon_type)
    : id(id),
      name(name),
      mana_usage_cost(mana_usage_cost),
      range(range),
      cooldown(cooldown),
      weapon_type(weapon_type) {}

Spell::~Spell() {}

const unsigned int Spell::getRange() const {
    return this->range;
}

const WeaponType Spell::getWeaponType() const {
    return this->weapon_type;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

Spell* SpellFactory::newSpell(const SpellCfg& data) {
    if (data.weapon_type == HEALING)
        return new HealingSpell(data);

    return new AttackingSpell(data);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

AttackingSpell::AttackingSpell(const SpellCfg& data)
    : Spell(data.id, data.name, data.mana_usage_cost, data.range, data.cooldown,
            data.weapon_type),
      min_damage(data.min_damage),
      max_damage(data.max_damage) {}

AttackingSpell::~AttackingSpell() {}

const unsigned int AttackingSpell::cast(Character& caster) {
    // Consume el maná del jugador. Si no tiene suficiente,
    // consumeMana lanza excepción y no se efectua el hechizo.
    caster.consumeMana(this->mana_usage_cost);

    caster.setAttackCooldown(this->cooldown);

    RandomNumberGenerator random_number_generator;
    return (int)random_number_generator((int)min_damage, (int)max_damage);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

HealingSpell::HealingSpell(const SpellCfg& data)
    : Spell(data.id, data.name, data.mana_usage_cost, data.range, data.cooldown,
            data.weapon_type),
      recovery_points(data.recovery_points) {}

HealingSpell::~HealingSpell() {}

const unsigned int HealingSpell::cast(Character& caster) {
    // Consume el maná del jugador. Si no tiene suficiente,
    // consumeMana lanza excepción y no se efectua el hechizo.
    caster.consumeMana(this->mana_usage_cost);

    caster.setAttackCooldown(this->cooldown);
    fprintf(stderr, "Devuelvo recovery_points=%i\n", recovery_points);
    return this->recovery_points;
}

//-----------------------------------------------------------------------------
