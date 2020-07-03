#include "../../../includes/Control/Commands/ResurrectCommand.h"

ResurrectCommand::ResurrectCommand(const InstanceId caller,
                                   const uint32_t x_coord,
                                   const uint32_t y_coord)
    : caller(caller), x_coord(x_coord), y_coord(y_coord) {}

ResurrectCommand::~ResurrectCommand() {}

void ResurrectCommand::exec(Game& game) {
    game.resurrect(caller, x_coord, y_coord);
}
