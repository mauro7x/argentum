#include "../../includes/Control/Commands/DropCommand.h"

DropCommand::DropCommand(InstanceId caller, const uint8_t n_slot,
                         const uint32_t amount)
    : caller(caller), n_slot(n_slot), amount(amount) {}

DropCommand::~DropCommand() {}

void DropCommand::exec(Game& game) {
    game.drop(caller, n_slot, amount);
}
