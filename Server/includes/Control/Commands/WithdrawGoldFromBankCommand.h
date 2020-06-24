#ifndef __WITHDRAW_GOLD_FROM_BANK_COMMAND_H__
#define __WITHDRAW_GOLD_FROM_BANK_COMMAND_H__

#include "Command.h"

class WithdrawGoldFromBankCommand : public Command {
   private:
    InstanceId caller;

    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint32_t amount;

   public:
    WithdrawGoldFromBankCommand(InstanceId caller, const uint32_t x_coord,
                             const uint32_t y_coord, const uint32_t amount);
    ~WithdrawGoldFromBankCommand();

    void exec(Game& game) override;
};

#endif // __WITHDRAW_GOLD_FROM_BANK_COMMAND_H__

