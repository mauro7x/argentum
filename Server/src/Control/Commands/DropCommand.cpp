#include "../../../includes/Control/Commands/DropCommand.h"

DropCommand::DropCommand(const InstanceId caller, const uint8_t n_slot,
                         const uint32_t amount)
    : Command(caller), n_slot(n_slot), amount(amount) {}

DropCommand::~DropCommand() {}

void DropCommand::exec(Game& game) {
    game.drop(caller, n_slot, amount);
}
