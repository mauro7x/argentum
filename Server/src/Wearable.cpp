#include <random>
#include "../includes/Wearable.h"

Wearable::Wearable(const unsigned int id, const unsigned int price,
                   WearableType type,
                   const unsigned int min_usage_points,
                   const unsigned int max_usage_points):
                        Item(id, price),
                        type(type),
                        min_usage_points(min_usage_points),
                        max_usage_points(max_usage_points) {}

const WearableType Wearable::getType() const {
    return this->type;
}

const unsigned int Wearable::getUsagePoints() {
    // Obtengo numero random del HW
    std::random_device rd;
    // Agrego seed al generador
    std::mt19937 generator(rd());
    // Defino el rango
    std::uniform_int_distribution<> distribution(min_usage_points, 
                                                 max_usage_points);
    return distribution(generator);
}

void Wearable::equip(Character& character) {
    character.equip(this);
}

