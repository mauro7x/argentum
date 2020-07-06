#ifndef __BANK_H__
#define __BANK_H__
//-----------------------------------------------------------------------------
#include <string>
#include <unordered_map>
//-----------------------------------------------------------------------------
#include "BankAccount.h"
#include "ItemsContainer.h"
//-----------------------------------------------------------------------------
class Bank {
   private:
    std::unordered_map<std::string, BankAccount> accounts;

    ItemsContainer& items;

   public:
    Bank(ItemsContainer& items);
    ~Bank();

    Bank(const Bank&) = delete;
    Bank& operator=(const Bank&) = delete;
    Bank(Bank&& other) = delete;
    Bank& operator=(Bank&& other) = delete;

    /*
     * Devuelve la cuenta asociada al nickname especificado.
     *
     * Si no existe, crea una nueva.
     */
    BankAccount& operator[](const std::string nickname);

    /*
     * Elimina la cuenta bancaria del jugador del sistema.
     */
    void removeAccount(const std::string& nickname);
};

#endif  // __BANK_H__
//-----------------------------------------------------------------------------
