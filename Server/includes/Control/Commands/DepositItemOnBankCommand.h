#ifndef __DEPOSIT_ITEM_ON_BANK_COMMAND_H__
#define __DEPOSIT_ITEM_ON_BANK_COMMAND_H__

#include "Command.h"

class DepositItemOnBankCommand : public Command {
   private:
    InstanceId caller;

    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint8_t n_slot;
    const uint32_t amount;

   public:
    DepositItemOnBankCommand(InstanceId caller, const uint32_t x_coord,
                             const uint32_t y_coord, const uint8_t n_slot,
                             const uint32_t amount);
    ~DepositItemOnBankCommand();

    void exec(Game& game) override;
};

#endif  // __DEPOSIT_ITEM_ON_BANK_COMMAND_H__
