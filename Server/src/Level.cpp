#include <math.h>
#include <algorithm>

#include "../includes/Level.h"
#include "../includes/Formulas.h"

#define INITIAL_LEVEL 1
#define INITIAL_XP 0

const unsigned int Level::calculateLevelUpXP() const {
    return Formulas::calculateLevelUpXP(this->level);
}

Level::Level():
    level(INITIAL_LEVEL),
    xp(INITIAL_XP),
    level_up_xp(calculateLevelUpXP()) {}

Level::~Level() {}

void Level::sumXP(const unsigned int points) {
    this->xp += points;

    // Subo de nivel si alcanzo el umbral
    while (this->xp >= this->level_up_xp) {
        level += 1;
        this->level_up_xp = calculateLevelUpXP();
    }
}

void Level::onAttackUpdate(const unsigned int damage, 
                           const unsigned int attacked_level) {
    unsigned int gained_xp = Formulas::calculateAttackXPGain(damage, 
                                attacked_level, this->level);

    this->sumXP(gained_xp);
}

void Level::onKillUpdate(const unsigned int attacked_max_health,
                         const unsigned int attacked_level) {
    unsigned int gained_xp = Formulas::calculateKillXPGain(attacked_max_health,
                                attacked_level, this->level);

    this->sumXP(gained_xp);
}

const unsigned int Level::getLevel() const {
    return this->level;
}
