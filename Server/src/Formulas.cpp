#include <math.h>
#include <algorithm>

#include "../includes/Formulas.h"
#include "../includes/RandomNumberGenerator.h"

// Level
const unsigned int Formulas::calculateLevelUpXP(const unsigned int level) {
    return 1000 * pow(level, 1.8);
}

const unsigned int Formulas::calculateAttackXPGain(const unsigned int damage,
        const unsigned int attacked_level,
        const unsigned int attacker_level) {
    return damage * std::max(attacked_level - attacker_level + 10,
                             (unsigned int) 0);
}

const unsigned int Formulas::calculateKillXPGain(
        const unsigned int attacked_max_health,
        const unsigned int attacked_level,
        const unsigned int attacker_level) {
    RandomNumberGenerator random_number_generator;
    return random_number_generator(0, 0.1) * attacked_max_health *
           std::max(attacked_level - attacker_level + 10, (unsigned int) 0);
}

// Character
const unsigned int Formulas::calculateMaxHealth(
        const unsigned int constitution,
        const unsigned int kind_factor,
        const unsigned int race_factor,
        const unsigned int level) {
    return constitution * kind_factor * race_factor * level;
}

const unsigned int Formulas::calculateMaxMana(
        const unsigned int intelligence,
        const unsigned int kind_factor,
        const unsigned int race_factor,
        const unsigned int level) {
    return intelligence * kind_factor * race_factor * level;
}

const unsigned int Formulas::calculateHealthTimeRecovery(
        const unsigned int race_factor,
        const unsigned int seconds_elapsed) {
    return race_factor * seconds_elapsed;
}

const unsigned int Formulas::calculateManaTimeRecovery(
        const unsigned int race_factor,
        const unsigned int seconds_elapsed) {
    return race_factor * seconds_elapsed;
}

const bool Formulas::calculateAttackEluding(
        const unsigned int agility) {
    RandomNumberGenerator random_number_generator;
    return (std::pow(random_number_generator(0, 1), agility) < 0.001);
}

const unsigned int Formulas::calculateMaxSafeGold(
        const unsigned int level) {
    return 100 * pow(level, 1.1);
}

const unsigned int Formulas::calculateMaxExcessGold(
            const unsigned int max_safe_gold) {
    return max_safe_gold * 0.5;
}