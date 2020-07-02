#include "../../../includes/Control/Commands/DepositGoldOnBankCommand.h"

DepositGoldOnBankCommand::DepositGoldOnBankCommand(InstanceId caller,
                                                   const uint32_t x_coord,
                                                   const uint32_t y_coord,
                                                   const uint32_t amount)
    : caller(caller), x_coord(x_coord), y_coord(y_coord), amount(amount) {}

DepositGoldOnBankCommand::~DepositGoldOnBankCommand() {}

void DepositGoldOnBankCommand::exec(Game& game) {
    game.depositGoldOnBank(caller, x_coord, y_coord, amount);
}
