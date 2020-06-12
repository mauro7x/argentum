#ifndef __FORMULAS_H__
#define __FORMULAS_H__

class Formulas {
    public:
        // Level
        static const unsigned int calculateLevelUpXP(const unsigned int level);
        static const unsigned int calculateAttackXPGain(
            const unsigned int damage,
            const unsigned int attacked_level,
            const unsigned int attacker_level);
        static const unsigned int calculateKillXPGain(
            const unsigned int attacked_max_health,
            const unsigned int attacked_level,
            const unsigned int attacker_level);
};

#endif