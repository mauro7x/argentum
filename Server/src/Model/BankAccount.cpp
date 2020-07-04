#include "../../includes/Model/BankAccount.h"
//-----------------------------------------------------------------------------

BankAccount::BankAccount(ItemsContainer& items) : gold(0), items(items) {}

BankAccount::~BankAccount() {}

void BankAccount::deposit(const Id item, const unsigned int amount) {
    if (this->saved_items.size() == N_BANK_ACCOUNT_SLOTS)
        throw FullBankAccountException();

    this->saved_items[item] += amount;
}

void BankAccount::depositGold(const unsigned int amount) {
    gold += amount;
}

Item* BankAccount::withdraw(const Id item_id, unsigned int& amount) {
    if (!this->saved_items.count(item_id))
        throw InvalidItemIdException();

    unsigned int& saved_amount = this->saved_items[item_id];

    if (saved_amount > amount) {
        saved_amount -= amount;
    } else {
        amount = saved_amount;
        saved_amount = 0;
    }

    if (!saved_amount)
        this->saved_items.erase(item_id);

    return items[item_id];
}

void BankAccount::withdrawGold(unsigned int& amount) {
    if (!gold)
        throw NoMoneyAvailableException();

    if (gold >= amount) {
        gold -= amount;
    } else {
        amount = gold;
        gold = 0;
    }
}

void BankAccount::list(std::string& init_msg,
                       std::list<std::string>& list_items) const {
    if (!saved_items.size() && !gold) {
        init_msg = "No tienes ni oro ni items guardados en el banco";
        return;
    }

    init_msg = "Items guardados en tu cuenta bancaria";

    std::string list_item;

    if (gold) {
        list_item = "Oro: " + std::to_string(gold);
        list_items.push_back(list_item);
    }

    std::unordered_map<Id, unsigned int>::const_iterator it =
        saved_items.begin();

    for (; it != saved_items.end(); ++it) {
        list_item = std::to_string(it->first) + ": ";
        list_item += items[it->first]->what() + " ";
        list_item += "(" + std::to_string(it->second) + ") ";
        list_items.push_back(list_item);
    }
}

void BankAccount::fillPersistenceData(CharacterCfg& data) const {
    data.bank_gold = gold;

    std::unordered_map<Id, unsigned int>::const_iterator iterator =
        this->saved_items.begin();

    size_t pos_actual = 0;

    while (iterator != this->saved_items.end()) {
        data.bank_account[pos_actual].item = iterator->first;
        data.bank_account[pos_actual].amount = iterator->second;
        ++pos_actual;
        ++iterator;
    }
}
//-----------------------------------------------------------------------------

const char* NoMoneyAvailableException::what() const noexcept {
    return "No tienes dinero en la cuenta.";
}

const char* InvalidItemIdException::what() const noexcept {
    return "No tienes ningún item con ese id en tu cuenta.";
}

const char* FullBankAccountException::what() const noexcept {
    return "Cuenta llena. No puedes guardar nuevos items.";
}

//-----------------------------------------------------------------------------
