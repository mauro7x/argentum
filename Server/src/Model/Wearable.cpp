#include "../../includes/Model/Wearable.h"

#include "../../includes/Model/Character.h"

Wearable::Wearable(const Id id, const std::string name,
                   const unsigned int price, WearableType wearable_type)
    : Item(id, name, price), wearable_type(wearable_type) {}

Wearable::~Wearable() {}

const WearableType Wearable::getType() const {
    return this->wearable_type;
}

Response Wearable::equip(Character& character) {
    return character.equip(this);
}
