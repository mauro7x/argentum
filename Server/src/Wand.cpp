#include "../includes/Wand.h"


Wand::Wand(WandCfg& data): Wearable(data.id, data.name, data.price, 
                                   data.type, data.min_damage, 
                                   data.max_damage),
                                   mana_usage_cost(data.mana_usage_cost) {}
Wand::~Wand() {}

const unsigned int Wand::use() {
    // IMPLEMENTAR LOGICA DE ATAQUE
    return getUsagePoints();
}
