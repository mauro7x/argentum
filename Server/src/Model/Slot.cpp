#include "../../includes/Model/Slot.h"

Slot::Slot():
    item(nullptr),
    amount(0) {}

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

void Slot::addItem(Item* item) {
    if (this->isEmpty())
        this->item = item;

    ++this->amount;
}

Item* Slot::takeItem() {
    if (this->isEmpty())
        return nullptr;

    --this->amount;
    Item* item = this->item;

    if (this->isEmpty())
        this->item = nullptr;

    return item;
}

const Id Slot::getItemId() const {
    return this->item->getId();
}
const unsigned int Slot::getAmount() const {
    return this->amount;
}
