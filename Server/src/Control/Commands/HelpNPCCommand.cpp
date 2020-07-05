#include "../../../includes/Control/Commands/HelpNPCCommand.h"

HelpNPCCommand::HelpNPCCommand(const InstanceId caller, const uint32_t x_coord,
                               const uint32_t y_coord)
    : Command(caller), x_coord(x_coord), y_coord(y_coord) {}

HelpNPCCommand::~HelpNPCCommand() {}

void HelpNPCCommand::exec(Game& game) {
    game.help(caller, x_coord, y_coord);
}
