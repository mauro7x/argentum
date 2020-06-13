#include "../includes/Spell.h"
#include "../includes/Character.h" // Evito circular dependences.
#include "../includes/RandomNumberGenerator.h"

Spell::Spell(const int id,
             const std::string name,
             const unsigned int mana_usage_cost):
                id(id),
                name(name),
                mana_usage_cost(mana_usage_cost) {}
Spell::~Spell() {}

Spell* SpellFactory::newSpell(const SpellCfg& data) {
    if (data.type == ATTACKING) {
        return new AttackingSpell(data);
    } else if (data.type == HEALING) {
        return new HealingSpell(data);
    } else {
        throw UnknownSpellTypeException();
    }
}

AttackingSpell::AttackingSpell(const SpellCfg& data): 
    Spell(data.id, data.name, data.mana_usage_cost),
    attack_distance(data.attack_distance), 
    min_damage(data.min_damage),
    max_damage(data.max_damage) {}
AttackingSpell::~AttackingSpell() {}

const unsigned int AttackingSpell::cast(Character& caster) {
    // Consume el maná del jugador. Si no tiene suficiente,
    // consumeMana lanza excepción y no se efectua el hechizo.
    caster.consumeMana(this->mana_usage_cost);

    RandomNumberGenerator random_number_generator;
    return random_number_generator(min_damage, max_damage);
}

const unsigned int AttackingSpell::getRange() const {
    return this->attack_distance;
}

HealingSpell::HealingSpell(const SpellCfg& data): 
    Spell(data.id, data.name, data.mana_usage_cost),
    recovery_points(data.recovery_points) {}
HealingSpell::~HealingSpell() {}

const unsigned int HealingSpell::cast(Character& caster) {
    // Consume el maná del jugador. Si no tiene suficiente,
    // consumeMana lanza excepción y no se efectua el hechizo.
    caster.consumeMana(this->mana_usage_cost);

    caster.recoverHealth(this->recovery_points);

    return 0; // No hace daño otro jugador.
}

const unsigned int HealingSpell::getRange() const {
    return 0; // Se lanza sobre si mismo.
}

const char* UnknownSpellTypeException::what() const noexcept {
    return "El tipo de hechizo especificado en SpellCfg es inválido.";
}