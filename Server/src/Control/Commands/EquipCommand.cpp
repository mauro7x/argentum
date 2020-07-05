#include "../../../includes/Control/Commands/EquipCommand.h"

EquipCommand::EquipCommand(const InstanceId caller, const uint8_t n_slot)
    : Command(caller), n_slot(n_slot) {}

EquipCommand::~EquipCommand() {}

void EquipCommand::exec(Game& game) {
    game.equip(caller, n_slot);
}
