#include "../../../includes/Control/Commands/BuyItemCommand.h"

BuyItemCommand::BuyItemCommand(const InstanceId caller, const uint32_t x_coord,
                               const uint32_t y_coord, const uint32_t item_id,
                               const uint32_t amount)
    : Command(caller),
      x_coord(x_coord),
      y_coord(y_coord),
      item_id(item_id),
      amount(amount) {}

BuyItemCommand::~BuyItemCommand() {}

void BuyItemCommand::exec(Game& game) {
    game.buyItem(caller, x_coord, y_coord, item_id, amount);
}
