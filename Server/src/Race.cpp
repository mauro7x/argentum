#include "../includes/Race.h"

Race::Race(const RaceCfg& data): 
    id(data.id), name(data.name),
    max_health_factor(data.max_health_factor), 
    health_recovery_factor(data.health_recovery_factor),
    max_mana_factor(data.max_mana_factor),
    intelligence(data.intelligence),
    constitution(data.constitution),
    strength(data.strength),
    agility(data.agility) {}

Race::~Race() {}
