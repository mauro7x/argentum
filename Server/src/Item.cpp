#include "../includes/Item.h"

Item::Item(const unsigned int id, const std::string name,
           const unsigned int price):
                id(id),
                name(name),
                price(price) {}

Item::~Item() {}
