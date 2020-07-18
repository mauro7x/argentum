#include "../../includes/Model/Defence.h"
//-----------------------------------------------------------------------------
#include "../../includes/Model/Character.h"  // Evito circular dependeces.
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/RandomNumberGenerator.h"
//-----------------------------------------------------------------------------

Defence::Defence(const DefenceCfg& data)
    : Wearable(data.id, data.name, data.price, data.type),
      min_defence(data.min_defence),
      max_defence(data.max_defence) {}

Defence::~Defence() {}

Response Defence::use(Character& user, int& points) {
    RandomNumberGenerator random_number_generator;
    points = (int)random_number_generator((int)min_defence, (int)max_defence);
    return Response(true, "", SUCCESS_MSG);
}

const unsigned int Defence::getRange() const {
    throw Exception("Defence::getRange: Las defensas no tienen rango.");
}

const WeaponType Defence::getWeaponType() const {
    throw Exception("Defence::getWeaponType: Las defensas no son armas.");
}

//-----------------------------------------------------------------------------
