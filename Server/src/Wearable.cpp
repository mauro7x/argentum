#include "../includes/Wearable.h"
#include "../includes/Character.h"

Wearable::Wearable(const unsigned int id, const std::string name,
                   const unsigned int price,
                   WearableType type):
                        Item(id, name, price),
                        type(type) {}

Wearable::~Wearable() {}

const WearableType Wearable::getType() const {
    return this->type;
}

void Wearable::equip(Character& character) {
    character.equip(this);
}

const unsigned int Wearable::getRange() const {
    return 0; // Defensas no tienen rango.
              // Las armas overridean este metodo.
}
