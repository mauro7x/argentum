#include "../../../includes/Control/Commands/SellItemCommand.h"

SellItemCommand::SellItemCommand(const InstanceId caller,
                                 const uint32_t x_coord, const uint32_t y_coord,
                                 const uint8_t n_slot, const uint32_t amount)
    : Command(caller),
      x_coord(x_coord),
      y_coord(y_coord),
      n_slot(n_slot),
      amount(amount) {}

SellItemCommand::~SellItemCommand() {}

void SellItemCommand::exec(Game& game) {
    game.sellItem(caller, x_coord, y_coord, n_slot, amount);
}
