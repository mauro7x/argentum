#include "../../includes/Model/Defence.h"
#include "../../includes/Model/Character.h" // Evito circular dependeces.
#include "../../includes/Model/RandomNumberGenerator.h"

Defence::Defence(const DefenceCfg& data):
    Wearable(data.id, data.name, data.price, data.type),
                min_defence(data.min_defence), 
                max_defence(data.max_defence) {}
Defence::~Defence() {}

const unsigned int Defence::use(Character& user) {
    RandomNumberGenerator random_number_generator;
    return random_number_generator(min_defence, max_defence);
}
