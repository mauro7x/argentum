#include "../../includes/Model/Wearable.h"
#include "../../includes/Model/Character.h"

Wearable::Wearable(const Id id, const std::string name,
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
