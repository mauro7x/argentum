#include "../../includes/Model/Defence.h"

#include "../../includes/Model/Character.h"  // Evito circular dependeces.
//------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/RandomNumberGenerator.h"

Defence::Defence(const DefenceCfg& data)
    : Wearable(data.id, data.name, data.price, data.type),
      min_defence(data.min_defence),
      max_defence(data.max_defence) {}
Defence::~Defence() {}

const unsigned int Defence::use(Character& user) {
    RandomNumberGenerator random_number_generator;
    return (int)random_number_generator((int)min_defence,
                                          (int)max_defence);
}

const unsigned int Defence::getRange() const {
    throw Exception("Defence::getRange: Las defensas no tienen rango.");
}

const bool Defence::isHealing() const {
    throw Exception(
        "Defence::isHealing: Las defensas no pueden ser curativas.");
}
