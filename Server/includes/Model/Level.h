#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "../../../Common/includes/DataStructs.h"
#include "config_structs.h"

class Character;  // Forward declaration para evitar circular dependences.

class Level {
   private:
    uint32_t level;
    uint32_t xp;
    uint32_t level_up_xp;

   public:
    Level(uint32_t init_level, uint32_t init_xp);
    ~Level();

    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;
    Level(Level&&) = delete;
    Level& operator=(Level&&) = delete;

    void sumXP(Character& character, const unsigned int points);

    void onAttackUpdate(Character& character, const unsigned int damage,
                        const unsigned int attacked_level);

    void onKillUpdate(Character& character,
                      const unsigned int attacked_max_health,
                      const unsigned int attacked_level);

    const uint32_t getLevel() const;
    const bool isNewbie() const;

    void fillBroadcastData(PlayerData& data) const;
    void fillPersistenceData(CharacterCfg& data) const;
};

#endif
