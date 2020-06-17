#include "../../includes/Model/Race.h"

Race::Race(const RaceCfg& data): 
    id(data.id), name(data.name),
    max_health_factor(data.max_health_factor),
    max_mana_factor(data.max_mana_factor),
    health_recovery_factor(data.health_recovery_factor),
    mana_recovery_factor(data.mana_recovery_factor) {}

Race::~Race() {}
