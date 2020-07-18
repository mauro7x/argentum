#include "../../includes/Model/Potion.h"
//-----------------------------------------------------------------------------
#include "../../includes/Model/Character.h"
#include "../../includes/Model/Kind.h"
//-----------------------------------------------------------------------------
#define NO_HEALTH_POINTS_TO_RECOVER_MSG "Ya tienes la barra de vida llena."
#define NO_MANA_POINTS_TO_RECOVER_MSG "Ya tienes la barra de maná llena."
//-----------------------------------------------------------------------------

Potion::Potion(const Id id, std::string name, const unsigned int price,
               const unsigned int recovery_points)
    : Item(id, name, price), recovery_points(recovery_points) {}

Potion::~Potion() {}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

Potion* PotionFactory::newPotion(const PotionCfg& data) {
    if (data.type == HEALTH) {
        return new HealthPotion(data);
    } else if (data.type == MANA) {
        return new ManaPotion(data);
    } else {
        throw UnknownPotionTypeException();
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

HealthPotion::HealthPotion(const PotionCfg& data)
    : Potion(data.id, data.name, data.price, data.recovery_points) {}
HealthPotion::~HealthPotion() {}

Response HealthPotion::equip(Character& equipper) {
    int points = this->recovery_points;

    if (!equipper.recoverHealth(points)) {
        equipper.takeItem((Item*)this);
        return Response(false, NO_HEALTH_POINTS_TO_RECOVER_MSG, ERROR_MSG);
    }

    std::string recovery_msg =
        "Te has curado " + std::to_string(points) + " de vida.";
    return Response(true, recovery_msg, SUCCESS_MSG);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

ManaPotion::ManaPotion(const PotionCfg& data)
    : Potion(data.id, data.name, data.price, data.recovery_points) {}
ManaPotion::~ManaPotion() {}

Response ManaPotion::equip(Character& equipper) {
    Response response = equipper.doMagic();
    if (!response.succeeded) {
        equipper.takeItem((Item*)this);
        return response;
    }

    unsigned int points = this->recovery_points;

    if (!equipper.recoverMana(points)) {
        equipper.takeItem((Item*)this);
        return Response(false, NO_MANA_POINTS_TO_RECOVER_MSG, ERROR_MSG);
    }

    std::string recovery_msg =
        "Te has curado " + std::to_string(points) + " de maná.";
    return Response(true, recovery_msg, SUCCESS_MSG);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

const char* UnknownPotionTypeException::what() const noexcept {
    return "El tipo de poción especificado en PotionCfg es inválido.";
}

//-----------------------------------------------------------------------------
