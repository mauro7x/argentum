#include "../../includes/Control/Commands/WithdrawItemFromBankCommand.h"

WithdrawItemFromBankCommand::WithdrawItemFromBankCommand(InstanceId caller,
                                                         const uint32_t x_coord,
                                                         const uint32_t y_coord,
                                                         const uint32_t item_id,
                                                         const uint32_t amount)
    : caller(caller),
      x_coord(x_coord),
      y_coord(y_coord),
      item_id(item_id),
      amount(amount) {}

WithdrawItemFromBankCommand::~WithdrawItemFromBankCommand() {}

void WithdrawItemFromBankCommand::exec(Game& game) {
    game.withdrawItemFromBank(caller, x_coord, y_coord, item_id, amount);
}
