#include "../../includes/Model/Formulas.h"
//-----------------------------------------------------------------------------
#include <math.h>

#include <algorithm>
#include <cstdlib>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/RandomNumberGenerator.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

Formulas::Formulas() {
    _loadCfg();
}

Formulas::~Formulas() {}

void Formulas::_loadCfg() {
    json config =
        JSON::loadJsonFile(paths::config(FORMULAS_MODIFIERS_FILEPATH));
    cfg.level_up_modifier = config["level_up_modifier"];
    cfg.level_up_exponent = config["level_up_exponent"];
    cfg.attack_xp_gain_level_difference =
        config["attack_xp_gain_level_difference"];
    cfg.kill_xp_gain_min_rand_modifier =
        config["kill_xp_gain_min_rand_modifier"];
    cfg.kill_xp_gain_max_rand_modifier =
        config["kill_xp_gain_max_rand_modifier"];
    cfg.newbies_max_level = config["newbies_max_level"];
    cfg.elude_attack_min_rand_base = config["elude_attack_min_rand_base"];
    cfg.elude_attack_max_rand_base = config["elude_attack_max_rand_base"];
    cfg.elude_attack_threshold = config["elude_attack_threshold"];
    cfg.critical_attack_min_rand = config["critical_attack_min_rand"];
    cfg.critical_attack_max_rand = config["critical_attack_max_rand"];
    cfg.critical_attack_threshold = config["critical_attack_threshold"];
    cfg.max_attacking_level_difference =
        config["max_attacking_level_difference"];
    cfg.max_safe_gold_modifier = config["max_safe_gold_modifier"];
    cfg.max_safe_gold_exponent = config["max_safe_gold_exponent"];
    cfg.max_excess_gold_proportion = config["max_excess_gold_proportion"];
    cfg.creature_gold_drop_min_rand_modifier =
        config["creature_gold_drop_min_rand_modifier"];
    cfg.creature_gold_drop_max_rand_modifier =
        config["creature_gold_drop_max_rand_modifier"];
    cfg.creature_drop_nothing_prob = config["creature_drop_nothing_prob"];
    cfg.creature_drop_gold_prob = config["creature_drop_gold_prob"];
    cfg.creature_drop_potion_prob = config["creature_drop_potion_prob"];
    cfg.creature_drop_weapon_prob = config["creature_drop_weapon_prob"];
    cfg.creature_drop_wand_prob = config["creature_drop_wand_prob"];
    cfg.creature_drop_defence_prob = config["creature_drop_defence_prob"];
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Level
//-----------------------------------------------------------------------------

const unsigned int Formulas::calculateLevelUpXP(const unsigned int level) {
    return cfg.level_up_modifier * pow(level, cfg.level_up_exponent);
}

const unsigned int Formulas::calculateAttackXPGain(
    const unsigned int damage, const unsigned int attacked_level,
    const unsigned int attacker_level) {
    return damage * std::max((int)(attacked_level - attacker_level +
                                   cfg.attack_xp_gain_level_difference),
                             0);
}

const unsigned int Formulas::calculateKillXPGain(
    const unsigned int attacked_max_health, const unsigned int attacked_level,
    const unsigned int attacker_level) {
    RandomNumberGenerator gen;
    return (float)gen(cfg.kill_xp_gain_min_rand_modifier,
                      cfg.kill_xp_gain_max_rand_modifier) *
           attacked_max_health *
           std::max((int)(attacked_level - attacker_level +
                          cfg.attack_xp_gain_level_difference),
                    0);
}

const bool Formulas::isNewbie(const unsigned int level) {
    return (level <= cfg.newbies_max_level);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Character
//-----------------------------------------------------------------------------

const unsigned int Formulas::calculateMaxHealth(const unsigned int constitution,
                                                const unsigned int kind_factor,
                                                const unsigned int race_factor,
                                                const unsigned int level) {
    return constitution * kind_factor * race_factor * level;
}

const unsigned int Formulas::calculateMaxMana(const unsigned int intelligence,
                                              const unsigned int kind_factor,
                                              const unsigned int race_factor,
                                              const unsigned int level) {
    return intelligence * kind_factor * race_factor * level;
}

const unsigned int Formulas::calculateHealthTimeRecovery(
    const unsigned int race_factor, const unsigned int ms_elapsed) {
    return (race_factor * ms_elapsed) / 1000;
}

const unsigned int Formulas::calculateManaTimeRecovery(
    const unsigned int race_factor, const unsigned int ms_elapsed) {
    return (race_factor * ms_elapsed) / 1000;
}

const unsigned int Formulas::calculateManaMeditationTimeRecovery(
    const unsigned int kind_meditation_factor, const unsigned int intelligence,
    const unsigned int ms_elapsed) {
    return (kind_meditation_factor * intelligence * ms_elapsed) / 1000;
}

const bool Formulas::isAttackEluded(const unsigned int agility) {
    RandomNumberGenerator gen;
    return (std::pow((float)gen(cfg.elude_attack_min_rand_base,
                                cfg.elude_attack_max_rand_base),
                     agility) < cfg.elude_attack_threshold);
}

const bool Formulas::isCriticalAttack() {
    RandomNumberGenerator gen;
    return (
        (float)gen(cfg.critical_attack_min_rand, cfg.critical_attack_max_rand) <
        cfg.critical_attack_threshold);
}

const bool Formulas::canAttackByLevel(const unsigned int attacker_level,
                                      const unsigned int attacked_level) {
    return (abs((int)(attacker_level - attacked_level)) <=
            cfg.max_attacking_level_difference);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inventory
//-----------------------------------------------------------------------------

const unsigned int Formulas::calculateMaxSafeGold(const unsigned int level) {
    return cfg.max_safe_gold_modifier * pow(level, cfg.max_safe_gold_exponent);
}

const unsigned int Formulas::calculateMaxExcessGold(
    const unsigned int max_safe_gold) {
    return max_safe_gold * cfg.max_excess_gold_proportion;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Creature
//-----------------------------------------------------------------------------

const unsigned int Formulas::calculateGoldDroppingAmount(
    const unsigned int max_health) {
    RandomNumberGenerator gen;
    return (float)gen(cfg.creature_gold_drop_min_rand_modifier,
                      cfg.creature_gold_drop_max_rand_modifier) *
           max_health;
}

const Drop Formulas::calculateDroppingItem() const {
    RandomNumberGenerator gen;
    float prob = gen((float)0, (float)1);

    float acum = cfg.creature_drop_nothing_prob;

    if (prob < acum)
        return DROP_NOTHING;

    acum += cfg.creature_drop_gold_prob;

    if (prob < acum)
        return DROP_GOLD;

    acum += cfg.creature_drop_potion_prob;
    
    if (prob < acum)
        return DROP_POTION;

    acum += cfg.creature_drop_weapon_prob;
    
    if (prob < acum)
        return DROP_WEAPON;

    acum += cfg.creature_drop_wand_prob;
    
    if (prob < acum)
        return DROP_WAND;

    return DROP_DEFENCE;
}

//-----------------------------------------------------------------------------
