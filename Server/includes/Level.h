#ifndef __LEVEL_H__
#define __LEVEL_H__

class Level {
    private:
        unsigned int level;
        unsigned int xp;
        unsigned int level_up_xp;

        const unsigned int calculateLevelUpXP() const;

    public:
        Level();
        ~Level();

        Level(const Level&) = delete;
        Level& operator=(const Level&) = delete;
        Level(Level&&) = delete;
        Level& operator=(Level&&) = delete;

        void sumXP(const unsigned int points);

        void onAttackUpdate(const unsigned int damage, 
                            const unsigned int attacked_level);
        
        void onKillUpdate(const unsigned int attacked_max_health,
                          const unsigned int attacked_level);
};

#endif
