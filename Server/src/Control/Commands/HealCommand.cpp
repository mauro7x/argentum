#include "../../../includes/Control/Commands/HealCommand.h"

HealCommand::HealCommand(const InstanceId caller, const uint32_t x_coord,
                         const uint32_t y_coord)
    : caller(caller), x_coord(x_coord), y_coord(y_coord) {}

HealCommand::~HealCommand() {}

void HealCommand::exec(Game& game) {
    game.heal(caller, x_coord, y_coord);
}
