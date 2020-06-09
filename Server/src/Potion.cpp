#include "../includes/Potion.h"
#include "../includes/Character.h" // Se incluye aca para evitar dependencias 
                                   // circulares.

Potion::Potion(const unsigned int id,
               std::string name,
               const unsigned int price,
               const unsigned int recovery_points):
                    Item(id, name, price),
                    recovery_points(recovery_points) {}

Potion::~Potion() {}

Potion* PotionFactory::newPotion(PotionCfg data) {
    if (data.type == HEALTH) {
        return new HealthPotion(data);
    } else if (data.type == MANA) {
        return new ManaPotion(data);
    } else {
        throw UnknownPotionTypeException();
    }
}

HealthPotion::HealthPotion(PotionCfg data): Potion(data.id, 
                                                   data.name, 
                                                   data.price, 
                                                   data.recovery_points) {}
HealthPotion::~HealthPotion() {}

void HealthPotion::equip(Character& equipper) {
    equipper.recoverHealth(this->recovery_points);
}

ManaPotion::ManaPotion(PotionCfg data): Potion(data.id, 
                                               data.name, 
                                               data.price, 
                                               data.recovery_points) {}
ManaPotion::~ManaPotion() {}

void ManaPotion::equip(Character& equipper) {
    equipper.recoverMana(this->recovery_points);
}

const char* UnknownPotionTypeException::what() const noexcept {
    return "El tipo de poción especificado en PotionCfg es inválido.";
}
