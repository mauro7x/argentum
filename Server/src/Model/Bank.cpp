#include "../../includes/Model/Bank.h"
//-----------------------------------------------------------------------------

Bank::Bank(ItemsContainer& items) : items(items) {}
Bank::~Bank() {}

BankAccount& Bank::operator[](const std::string nickname) {
    if (!this->accounts.count(nickname)) {
        this->accounts.emplace(std::piecewise_construct,
                               std::forward_as_tuple(nickname),
                               std::forward_as_tuple(items));
    }

    return this->accounts.at(nickname);
}

void Bank::removeAccount(const std::string& nickname) {
    this->accounts.erase(nickname);
}

//-----------------------------------------------------------------------------
