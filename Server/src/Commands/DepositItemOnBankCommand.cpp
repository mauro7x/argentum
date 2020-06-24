#include "../../includes/Control/Commands/DepositItemOnBankCommand.h"

DepositItemOnBankCommand::DepositItemOnBankCommand(InstanceId caller,
                                                   const uint32_t x_coord,
                                                   const uint32_t y_coord,
                                                   const uint8_t n_slot,
                                                   const uint32_t amount)
    : caller(caller),
      x_coord(x_coord),
      y_coord(y_coord),
      n_slot(n_slot),
      amount(amount) {}

DepositItemOnBankCommand::~DepositItemOnBankCommand() {}

void DepositItemOnBankCommand::exec(Game& game) {
    game.depositItemOnBank(caller, x_coord, y_coord, n_slot, amount);
}
