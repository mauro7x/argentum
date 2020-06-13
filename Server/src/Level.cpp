#include <math.h>
#include <algorithm>

#include "../includes/Level.h"
#include "../includes/Formulas.h"
#include "../includes/Character.h"

#define INITIAL_LEVEL 1
#define INITIAL_XP 0

Level::Level():
    level(INITIAL_LEVEL),
    xp(INITIAL_XP),
    level_up_xp(Formulas::calculateLevelUpXP(this->level)) {}

Level::~Level() {}

void Level::sumXP(Character& character, const unsigned int points) {
    this->xp += points;

    // Subo de nivel si alcanzo el umbral
    while (this->xp >= this->level_up_xp) {
        level += 1;
        this->level_up_xp = Formulas::calculateLevelUpXP(this->level);
        character.updateLevelDependantAttributes();
    }
}

void Level::onAttackUpdate(Character& character,
                           const unsigned int damage, 
                           const unsigned int attacked_level) {
    if (!damage) {
        return;
    }

    unsigned int gained_xp = Formulas::calculateAttackXPGain(damage, 
                                attacked_level, this->level);

    this->sumXP(character, gained_xp);
}

void Level::onKillUpdate(Character& character,
                         const unsigned int attacked_max_health,
                         const unsigned int attacked_level) {
    unsigned int gained_xp = Formulas::calculateKillXPGain(attacked_max_health,
                                attacked_level, this->level);

    this->sumXP(character, gained_xp);
}

const unsigned int Level::getLevel() const {
    return this->level;
}

const unsigned int Level::isNewbie() const {
    return Formulas::isNewbie(this->level);
}
