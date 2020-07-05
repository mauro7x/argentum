#ifndef __WITHDRAW_GOLD_FROM_BANK_COMMAND_H__
#define __WITHDRAW_GOLD_FROM_BANK_COMMAND_H__

#include "Command.h"

// Retirar oro del banco.
class WithdrawGoldFromBankCommand : public Command {
    const uint32_t x_coord;
    const uint32_t y_coord;
    const uint32_t amount;

   public:
    WithdrawGoldFromBankCommand(const InstanceId caller, const uint32_t x_coord,
                                const uint32_t y_coord, const uint32_t amount);
    ~WithdrawGoldFromBankCommand();

    void exec(Game& game) override;
};

#endif  // __WITHDRAW_GOLD_FROM_BANK_COMMAND_H__
