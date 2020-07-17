#include "../../includes/Model/BankAccount.h"
//-----------------------------------------------------------------------------
#define FULL_BANK_ACCOUNT_ERROR_MSG \
    "Cuenta llena. No puedes guardar nuevos items."
#define NO_GOLD_AVAILABLE_ERROR_MSG "No tienes dinero en la cuenta."
//-----------------------------------------------------------------------------

BankAccount::BankAccount(ItemsContainer& items) : gold(0), items(items) {}

BankAccount::~BankAccount() {}

Response BankAccount::deposit(const Id item, const unsigned int amount) {
    if (this->saved_items.size() == N_BANK_ACCOUNT_SLOTS)
        return Response(false, FULL_BANK_ACCOUNT_ERROR_MSG, ERROR_MSG);

    this->saved_items[item] += amount;

    std::string reply_msg = "Se ha depositado " + items[item]->what() + " x" +
                            std::to_string(amount) + " en el banco";

    return Response(true, reply_msg, SUCCESS_MSG);
}

Response BankAccount::depositGold(const unsigned int amount) {
    gold += amount;

    std::string reply_msg =
        "Se han depositado " + std::to_string(amount) + " de oro en el banco";

    return Response(true, reply_msg, SUCCESS_MSG);
}

Item* BankAccount::withdraw(const Id item_id, unsigned int& amount) {
    if (!this->saved_items.count(item_id))
        return nullptr;

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

Response BankAccount::withdrawGold(unsigned int& amount) {
    if (!gold)
        return Response(false, NO_GOLD_AVAILABLE_ERROR_MSG, ERROR_MSG);

    std::string response_msg;

    if (gold >= amount) {
        gold -= amount;
    } else {
        amount = gold;
        gold = 0;
        response_msg += "No tenías tanto oro en la cuenta. ";
    }

    response_msg +=
        "Se ha extraído " + std::to_string(amount) + " oro del banco.";

    return Response(true, response_msg, SUCCESS_MSG);
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
