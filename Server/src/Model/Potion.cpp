#include "../../includes/Model/Potion.h"
#include "../../includes/Model/Character.h" // Se incluye aca para evitar dependencias 
                                   // circulares.

Potion::Potion(const Id id,
               std::string name,
               const unsigned int price,
               const unsigned int recovery_points):
    Item(id, name, price),
    recovery_points(recovery_points) {}

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

HealthPotion::HealthPotion(const PotionCfg& data): 
    Potion(data.id, data.name, 
           data.price, data.recovery_points) {}
HealthPotion::~HealthPotion() {}

void HealthPotion::equip(Character& equipper) {
    equipper.recoverHealth(this->recovery_points);
}

ManaPotion::ManaPotion(const PotionCfg& data): 
    Potion(data.id, data.name, 
           data.price, data.recovery_points) {}
ManaPotion::~ManaPotion() {}

void ManaPotion::equip(Character& equipper) {
    equipper.recoverMana(this->recovery_points);
}

const char* UnknownPotionTypeException::what() const noexcept {
    return "El tipo de poción especificado en PotionCfg es inválido.";
}
