#include "../../../includes/Control/Commands/StopMovingCommand.h"

StopMovingCommand::StopMovingCommand(InstanceId caller)
    : caller(caller) {}
StopMovingCommand::~StopMovingCommand() {}

void StopMovingCommand::exec(Game& game) {
    game.stopMoving(caller);
}
