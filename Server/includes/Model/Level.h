#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "../../../Common/includes/UnitData.h"

class Character; // Forward declaration para evitar circular dependences.

class Level {
    private:
        unsigned int level;
        unsigned int xp;
        unsigned int level_up_xp;

    public:
        Level();
        ~Level();

        Level(const Level&) = delete;
        Level& operator=(const Level&) = delete;
        Level(Level&&) = delete;
        Level& operator=(Level&&) = delete;

        void sumXP(Character& character, const unsigned int points);

        void onAttackUpdate(Character& character,
                            const unsigned int damage, 
                            const unsigned int attacked_level);
        
        void onKillUpdate(Character& character,
                          const unsigned int attacked_max_health,
                          const unsigned int attacked_level);

        const unsigned int getLevel() const;
        const unsigned int isNewbie() const;

        void fillBroadcastData(PlayerData& data) const;
};

#endif
