#include "../../../includes/Control/Commands/UnequipCommand.h"

UnequipCommand::UnequipCommand(const InstanceId caller, const uint8_t n_slot)
    : Command(caller), n_slot(n_slot) {}

UnequipCommand::~UnequipCommand() {}

void UnequipCommand::exec(Game& game) {
    game.unequip(caller, n_slot);
}
