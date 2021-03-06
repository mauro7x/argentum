#include "../../../includes/Control/Commands/WithdrawGoldFromBankCommand.h"

WithdrawGoldFromBankCommand::WithdrawGoldFromBankCommand(const InstanceId caller,
                                                         const uint32_t x_coord,
                                                         const uint32_t y_coord,
                                                         const uint32_t amount)
    : Command(caller), x_coord(x_coord), y_coord(y_coord), amount(amount) {}

WithdrawGoldFromBankCommand::~WithdrawGoldFromBankCommand() {}

void WithdrawGoldFromBankCommand::exec(Game& game) {
    game.withdrawGoldFromBank(caller, x_coord, y_coord, amount);
}
