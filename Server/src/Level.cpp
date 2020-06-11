#include <math.h>

#include "../includes/Level.h"

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
