#ifndef __WITHDRAW_ITEM_FROM_BANK_COMMAND_H__
#define __WITHDRAW_ITEM_FROM_BANK_COMMAND_H__

#include "Command.h"

class WithdrawItemFromBankCommand : public Command {
   private:
    InstanceId caller;
    char cmd;

    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint32_t item_id;
    const uint32_t amount;

   public:
    WithdrawItemFromBankCommand(InstanceId caller, char cmd,
                                const uint32_t x_coord, const uint32_t y_coord,
                                const uint32_t item_id, const uint32_t amount);
    ~WithdrawItemFromBankCommand() {}

    void exec(Game& game) override;
};

#endif  // __WITHDRAW_ITEM_FROM_BANK_COMMAND_H__
