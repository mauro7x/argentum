#include <math.h>
#include <algorithm>

#include "../includes/Formulas.h"
#include "../includes/RandomNumberGenerator.h"

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
