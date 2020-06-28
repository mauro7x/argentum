#include "../../includes/Model/Slot.h"

Slot::Slot() : item(nullptr), amount(0) {}
Slot::Slot(Item* item, uint32_t amount) : item(item), amount(amount) {}

Slot::~Slot() {}

Slot::Slot(Slot&& other) {
    this->item = other.item;
    this->amount = other.amount;

    other.item = nullptr;
    other.amount = 0;
}

Slot& Slot::operator=(Slot&& other) {
    this->item = other.item;
    this->amount = other.amount;

    other.item = nullptr;
    other.amount = 0;

    return *this;
}

const bool Slot::isEmpty() const {
    return (!this->amount);
}

void Slot::addItem(Item* item, const unsigned int amount) {
    if (this->isEmpty())
        this->item = item;

    this->amount += amount;
}

Item* Slot::takeItem(unsigned int& amount_to_take) {
    if (this->isEmpty())
        return nullptr;
    
    if (amount_to_take > this->amount)
        amount_to_take = this->amount;

    this->amount -= amount_to_take;
    Item* item = this->item;

    if (this->isEmpty())
        this->item = nullptr;

    return item;
}

void Slot::clearSlot() {
    this->item = nullptr;
    this->amount = 0;
}

const Id Slot::getItemId() const {
    if (!item) {
        return 0;
    }

    return this->item->getId();
}

const unsigned int Slot::getAmount() const {
    return this->amount;
}
