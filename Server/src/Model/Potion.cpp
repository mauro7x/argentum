#include "../../includes/Model/Potion.h"
//-----------------------------------------------------------------------------
#include "../../includes/Model/Character.h"
#include "../../includes/Model/Kind.h"
//-----------------------------------------------------------------------------

Potion::Potion(const Id id, std::string name, const unsigned int price,
               const unsigned int recovery_points)
    : Item(id, name, price), recovery_points(recovery_points) {}

Potion::~Potion() {}

Potion* PotionFactory::newPotion(const PotionCfg& data) {
    if (data.type == HEALTH) {
        return new HealthPotion(data);
    } else if (data.type == MANA) {
        return new ManaPotion(data);
    } else {
        throw UnknownPotionTypeException();
    }
}

HealthPotion::HealthPotion(const PotionCfg& data)
    : Potion(data.id, data.name, data.price, data.recovery_points) {}
HealthPotion::~HealthPotion() {}

void HealthPotion::equip(Character& equipper) {
    unsigned int points = this->recovery_points;

    if (!equipper.recoverHealth(points)) {
        equipper.takeItem((Item*) this);
        throw HealthPotionHasNoPointsToRecoverException();
    };
}

ManaPotion::ManaPotion(const PotionCfg& data)
    : Potion(data.id, data.name, data.price, data.recovery_points) {}
ManaPotion::~ManaPotion() {}

void ManaPotion::equip(Character& equipper) {
    try {
        equipper.doMagic();
    } catch (const KindCantDoMagicException& e) {
        equipper.takeItem((Item*)this);
        throw e;
    }

    unsigned int points = this->recovery_points;
    
    if (!equipper.recoverMana(points)) {
        equipper.takeItem((Item*) this);
        throw ManaPotionHasNoPointsToRecoverException();
    };
}

const char* UnknownPotionTypeException::what() const noexcept {
    return "El tipo de poción especificado en PotionCfg es inválido.";
}

const char* HealthPotionHasNoPointsToRecoverException::what() const noexcept {
    return "Ya tienes la barra de vida llena.";
}

const char* ManaPotionHasNoPointsToRecoverException::what() const noexcept {
    return "Ya tienes la barra de maná llena.";
}

