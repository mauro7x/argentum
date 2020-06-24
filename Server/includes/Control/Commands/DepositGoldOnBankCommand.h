#ifndef __DEPOSIT_GOLD_ON_BANK_H__
#define __DEPOSIT_GOLD_ON_BANK_H__

#include "Command.h"

class DepositGoldOnBankCommand : public Command {
   private:
    InstanceId caller;

    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint32_t amount;

   public:
    DepositGoldOnBankCommand(InstanceId caller, const uint32_t x_coord,
                             const uint32_t y_coord, const uint32_t amount);
    ~DepositGoldOnBankCommand();

    void exec(Game& game) override;
};

#endif  // __DEPOSIT_GOLD_ON_BANK_H__
