#include "../../includes/Model/Spell.h"

#include "../../../Common/includes/RandomNumberGenerator.h"
#include "../../includes/Model/Character.h"  // Evito circular dependences.

Spell::Spell(const int id, const std::string name,
             const unsigned int mana_usage_cost, const unsigned int range,
             const unsigned int cooldown)
    : id(id),
      name(name),
      mana_usage_cost(mana_usage_cost),
      range(range),
      cooldown(cooldown) {}
Spell::~Spell() {}

const unsigned int Spell::getRange() const {
    return this->range;
}

Spell* SpellFactory::newSpell(const SpellCfg& data) {
    if (data.type == ATTACKING) {
        return new AttackingSpell(data);
    } else if (data.type == HEALING) {
        return new HealingSpell(data);
    } else {
        throw UnknownSpellTypeException();
    }
}

AttackingSpell::AttackingSpell(const SpellCfg& data)
    : Spell(data.id, data.name, data.mana_usage_cost, data.range,
            data.cooldown),
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

const bool AttackingSpell::isHealing() const {
    return false;
}

HealingSpell::HealingSpell(const SpellCfg& data)
    : Spell(data.id, data.name, data.mana_usage_cost, data.range,
            data.cooldown),
      recovery_points(data.recovery_points) {}
HealingSpell::~HealingSpell() {}

const unsigned int HealingSpell::cast(Character& caster) {
    // Consume el maná del jugador. Si no tiene suficiente,
    // consumeMana lanza excepción y no se efectua el hechizo.
    caster.consumeMana(this->mana_usage_cost);

    caster.setAttackCooldown(this->cooldown);

    return this->recovery_points;
}

const char* UnknownSpellTypeException::what() const noexcept {
    return "El tipo de hechizo especificado en SpellCfg es inválido.";
}

const bool HealingSpell::isHealing() const {
    return true;
}
