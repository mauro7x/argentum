#include "../../../includes/Control/Commands/ListCommand.h"

ListCommand::ListCommand(const InstanceId caller, const uint32_t x_coord,
                         const uint32_t y_coord)
    : Command(caller), x_coord(x_coord), y_coord(y_coord) {}

ListCommand::~ListCommand() {}

void ListCommand::exec(Game& game) {
    game.list(caller, x_coord, y_coord);
}
