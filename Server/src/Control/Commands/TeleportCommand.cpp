#include "../../../includes/Control/Commands/TeleportCommand.h"

TeleportCommand::TeleportCommand(const InstanceId caller, const uint32_t x_coord,
                         const uint32_t y_coord)
    : Command(caller), x_coord(x_coord), y_coord(y_coord) {}

TeleportCommand::~TeleportCommand() {}

void TeleportCommand::exec(Game& game) {
    game.teleport(caller, x_coord, y_coord);
}
