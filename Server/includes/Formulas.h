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

        // Character
        static const unsigned int calculateMaxHealth(
            const unsigned int constitution,
            const unsigned int kind_factor,
            const unsigned int race_factor,
            const unsigned int level);
        static const unsigned int calculateMaxMana(
            const unsigned int intelligence,
            const unsigned int kind_factor,
            const unsigned int race_factor,
            const unsigned int level);
        static const unsigned int calculateHealthTimeRecovery(
            const unsigned int race_factor,
            const unsigned int seconds_elapsed);
        static const unsigned int calculateManaTimeRecovery(
            const unsigned int race_factor,
            const unsigned int seconds_elapsed);

        static const bool calculateAttackEluding(
            const unsigned int agility);
};

#endif