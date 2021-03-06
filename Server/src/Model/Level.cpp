#include "../../includes/Model/Level.h"
//-----------------------------------------------------------------------------
#include <math.h>

#include <algorithm>
//-----------------------------------------------------------------------------
#include "../../includes/Model/Character.h"
#include "../../includes/Model/Formulas.h"
//-----------------------------------------------------------------------------

Level::Level(uint32_t init_level, uint32_t init_xp, Formulas& formulas)
    : level(init_level),
      xp(init_xp),
      formulas(formulas),
      level_up_xp(formulas.calculateLevelUpXP(this->level)) {
    if (!init_xp && init_level > 1)
        this->xp = formulas.calculateLevelUpXP(init_level - 1);
}

Level::~Level() {}

void Level::sumXP(Character& character, const unsigned int points) {
    this->xp += points;

    // Subo de nivel si alcanzo el umbral
    while (this->xp >= this->level_up_xp) {
        level += 1;
        this->level_up_xp = formulas.calculateLevelUpXP(this->level);
        character.updateLevelDependantAttributes();
    }
}

void Level::onAttackUpdate(Character& character, const unsigned int damage,
                           const unsigned int attacked_level) {
    if (!damage) {
        return;
    }

    unsigned int gained_xp =
        formulas.calculateAttackXPGain(damage, attacked_level, this->level);

    this->sumXP(character, gained_xp);
}

void Level::onKillUpdate(Character& character,
                         const unsigned int attacked_max_health,
                         const unsigned int attacked_level) {
    unsigned int gained_xp = formulas.calculateKillXPGain(
        attacked_max_health, attacked_level, this->level);

    this->sumXP(character, gained_xp);
}

const unsigned int Level::getLevel() const {
    return this->level;
}

const bool Level::isNewbie() const {
    return formulas.isNewbie(this->level);
}

void Level::fillBroadcastData(PlayerData& data) const {
    data.level = this->level;
    data.exp = this->xp;
    data.levelup_exp = this->level_up_xp;
    data.prev_levelup_exp = formulas.calculateLevelUpXP(this->level - 1);
}

void Level::fillPersistenceData(CharacterCfg& data) const {
    data.level = this->level;
    data.exp = this->xp;
}
