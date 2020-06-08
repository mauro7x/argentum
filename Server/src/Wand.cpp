#include "../includes/Wand.h"

Wand::Wand(const unsigned int id, const unsigned int price,
           const unsigned int min_damage, 
           const unsigned int max_damage,
           const unsigned int mana_usage_cost):
                    Wearable(id, price, WEAPON, min_damage, max_damage),
                    mana_usage_cost(mana_usage_cost) {}
Wand::~Wand() {}

const unsigned int Wand::use() {
    // IMPLEMENTAR LOGICA DE ATAQUE
    return getUsagePoints();
}
