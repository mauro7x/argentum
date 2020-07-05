#include "../../../includes/Control/Commands/StopMovingCommand.h"

StopMovingCommand::StopMovingCommand(const InstanceId caller)
    : Command(caller) {}
StopMovingCommand::~StopMovingCommand() {}

void StopMovingCommand::exec(Game& game) {
    game.stopMoving(caller);
}
