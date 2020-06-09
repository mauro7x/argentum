#include "../includes/Race.h"

Race::Race(const RaceCfg& data): id(data.id), name(data.name),
                           health(data.health), recovery(data.recovery),
                           mana(data.mana) {}

Race::~Race() {}
