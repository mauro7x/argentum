#include <math.h>
#include <algorithm>

#include "../includes/Level.h"
#include "../includes/RandomNumberGenerator.h"

#define INITIAL_LEVEL 1
#define INITIAL_XP 0

const unsigned int Level::calculateLevelUpXP() const {
    return 1000 * pow(this->level, 1.8);
}

Level::Level():
    level(INITIAL_LEVEL),
    xp(INITIAL_XP),
    level_up_xp(calculateLevelUpXP()) {}

Level::~Level() {}

void Level::sumXP(const unsigned int points) {
    this->xp += points;
    while (this->xp >= this->level_up_xp) {
        level += 1;
        this->level_up_xp = calculateLevelUpXP();
    }
}

void Level::onAttackUpdate(const unsigned int damage, 
                           const unsigned int attacked_level) {
    unsigned int gained_xp = damage * 
                             std::max(attacked_level - this->level + 10,
                                      (unsigned int) 0);
    this->sumXP(gained_xp);
}

void Level::onKillUpdate(const unsigned int attacked_max_health,
                         const unsigned int attacked_level) {
    RandomNumberGenerator random_number_generator;
    unsigned int gained_xp = random_number_generator(0, 0.1) *
                             attacked_max_health *
                             std::max(attacked_level - this->level + 10,
                                      (unsigned int) 0);
    this->sumXP(gained_xp);
}

const unsigned int Level::getLevel() const {
    return this->level;
}
