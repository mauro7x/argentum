#include "../../includes/Model/BankAccount.h"
//-----------------------------------------------------------------------------

BankAccount::BankAccount(ItemsContainer& items) : items(items) {}

BankAccount::~BankAccount() {}

void BankAccount::deposit(const Id item, const unsigned int amount) {
    this->saved_items[item] += amount;
}

void BankAccount::withdraw(const Id item, unsigned int& amount) {
    if (!this->saved_items.count(item))
        throw InvalidItemIdException();

    unsigned int& saved_amount = this->saved_items[item];

    if (saved_amount > amount) {
        saved_amount -= amount;
    } else {
        amount = saved_amount;
        saved_amount = 0;
    }

    if (!saved_amount)
        this->saved_items.erase(item);
}

void BankAccount::list(std::string& list) const {
    if (!saved_items.size()) {
        list += "No tienes ningún item guardado en el banco.";
        return;
    }

    std::unordered_map<Id, unsigned int>::const_iterator it =
        saved_items.begin();

    for (; it != saved_items.end(); ++it) {
        list += std::to_string(it->first) + ": ";
        list += items[it->first]->what() + " ";
        list += "(" + std::to_string(it->second) + ") ";
    }
}

//-----------------------------------------------------------------------------

const char* InvalidItemIdException::what() const noexcept {
    return "No tienes ningún item con ese id en tu cuenta.";
}

//-----------------------------------------------------------------------------
