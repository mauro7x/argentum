#include "Defence.h"

Defence::Defence(const unsigned int id, const unsigned int price, 
                 WearableType type,
                 const unsigned int min_defence, 
                 const unsigned int max_defence):
                    Wearable(id, price, type,
                             min_defence, max_defence) {}
Defence::~Defence() {}

const unsigned int Defence::use() {
    // IMPLEMENTAR LOGICA DE DEFENSA.
    return getUsagePoints();
}
