#ifndef __FORMULAS_H__
#define __FORMULAS_H__
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../paths.h"
//-----------------------------------------------------------------------------

struct FormulasCfg {
    float level_up_modifier;
    float level_up_exponent;
    int attack_xp_gain_level_difference;
    float kill_xp_gain_min_rand_modifier;
    float kill_xp_gain_max_rand_modifier;
    float newbies_max_level;
    float elude_attack_min_rand_base;
    float elude_attack_max_rand_base;
    float elude_attack_threshold;
    float critical_attack_min_rand;
    float critical_attack_max_rand;
    float critical_attack_threshold;
    float max_attacking_level_difference;
    float max_safe_gold_modifier;
    float max_safe_gold_exponent;
    float max_excess_gold_proportion;
    float creature_gold_drop_min_rand_modifier;
    float creature_gold_drop_max_rand_modifier;
    float creature_drop_nothing_prob;
    float creature_drop_gold_prob;
    float creature_drop_potion_prob;
    float creature_drop_weapon_prob;
    float creature_drop_wand_prob;
    float creature_drop_defence_prob;
};

//-----------------------------------------------------------------------------

enum Drop {
    DROP_NOTHING,
    DROP_GOLD,
    DROP_POTION,
    DROP_WEAPON,
    DROP_WAND,
    DROP_DEFENCE
};

//-----------------------------------------------------------------------------

class Formulas {
   private:
    FormulasCfg cfg;

    /* Carga la configuración del json correspondiente. */
    void _loadCfg();

   public:
    Formulas();
    ~Formulas();

    Formulas(const Formulas&) = delete;
    Formulas& operator=(const Formulas&) = delete;
    Formulas(Formulas&& other) = delete;
    Formulas& operator=(Formulas&& other) = delete;

    // Level
    const unsigned int calculateLevelUpXP(const unsigned int level);
    const unsigned int calculateAttackXPGain(const unsigned int damage,
                                             const unsigned int attacked_level,
                                             const unsigned int attacker_level);
    const unsigned int calculateKillXPGain(
        const unsigned int attacked_max_health,
        const unsigned int attacked_level, const unsigned int attacker_level);

    const bool isNewbie(const unsigned int level);

    // Character
    const unsigned int calculateMaxHealth(const unsigned int constitution,
                                          const unsigned int kind_factor,
                                          const unsigned int race_factor,
                                          const unsigned int level);
    const unsigned int calculateMaxMana(const unsigned int intelligence,
                                        const unsigned int kind_factor,
                                        const unsigned int race_factor,
                                        const unsigned int level);
    const unsigned int calculateHealthTimeRecovery(
        const unsigned int race_factor, const unsigned int seconds_elapsed);
    const unsigned int calculateManaTimeRecovery(
        const unsigned int race_factor, const unsigned int seconds_elapsed);
    const unsigned int calculateManaMeditationTimeRecovery(
        const unsigned int kind_meditation_factor,
        const unsigned int intelligence, const unsigned int seconds_elapsed);

    const bool isAttackEluded(const unsigned int agility);
    const bool isCriticalAttack();
    const bool canAttackByLevel(const unsigned int attacker_level,
                                const unsigned int attacked_level);

    // Inventory
    const unsigned int calculateMaxSafeGold(const unsigned int level);
    const unsigned int calculateMaxExcessGold(const unsigned int max_safe_gold);

    // Creature
    const unsigned int calculateGoldDroppingAmount(
        const unsigned int max_health);

    const Drop calculateDroppingItem() const;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
