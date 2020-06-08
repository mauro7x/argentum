#include "../includes/Defence.h"

Defence::Defence(DefenceCfg& data):
                    Wearable(data.id, data.name, data.price, data.type,
                             data.min_defence, data.max_defence) {}
Defence::~Defence() {}

const unsigned int Defence::use() {
    // IMPLEMENTAR LOGICA DE DEFENSA.
    return getUsagePoints();
}
