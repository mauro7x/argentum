#ifndef __BANK_ACCOUNT_H__
#define __BANK_ACCOUNT_H__
//-----------------------------------------------------------------------------
#include <exception>
#include <list>
#include <string>
#include <unordered_map>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "Item.h"
#include "ItemsContainer.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------

class BankAccount {
   private:
    std::unordered_map<Id, unsigned int> saved_items;
    unsigned int gold;
    ItemsContainer& items;

   public:
    BankAccount(ItemsContainer& items);
    ~BankAccount();

    BankAccount(const BankAccount&) = delete;
    BankAccount& operator=(const BankAccount&) = delete;
    BankAccount(BankAccount&& other) = delete;
    BankAccount& operator=(BankAccount&& other) = delete;

    /*
     * Deposita la cantidad especificada del item en la cuenta.
     */
    void deposit(const Id item, const unsigned int amount);

    /*
     * Deposita el oro.
     */
    void depositGold(const unsigned int amount);

    /*
     * Retira la cantidad especificada de un mismo item de la cuenta.
     *
     * Retorna el item.
     *
     * Si la cantidad especificada a tomar es menor a la cantidad presente en la
     * cuenta, se setea el valor de amount al efectivamente retirado.
     *
     * Si no quedan más items, se borra el Id del saved_items.
     *
     * Lanza InvalidItemIdException si no hay items guardados con el Id
     * especificado.
     */
    Item* withdraw(const Id item, unsigned int& amount);

    /*
     * Retira la cantidad de oro;
     */
    void withdrawGold(unsigned int& amount);

    /*
     * Escribe en el string recibido los ids de items en la cuenta con su
     * descripción.
     */
    void list(std::string& init_msg, std::list<std::string>& list_items) const;

    void fillPersistenceData(CharacterCfg& data) const;
};

//-----------------------------------------------------------------------------

class NoMoneyAvailableException : public std::exception {
   public:
    const char* what() const noexcept;
};

class InvalidItemIdException : public std::exception {
   public:
    const char* what() const noexcept;
};

class FullBankAccountException : public std::exception {
   public:
    const char* what() const noexcept;
};

#endif  // __BANK_ACCOUNT_H__
//-----------------------------------------------------------------------------
