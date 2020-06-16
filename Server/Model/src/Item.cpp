#include "../includes/Item.h"

Item::Item(const Id id, const std::string name,
           const unsigned int price):
                id(id),
                name(name),
                price(price) {}

Item::~Item() {}

const std::string Item::what() const {
     return this->name;
}
