#include "../../../includes/Control/Commands/StartMovingUpCommand.h"

StartMovingUpCommand::StartMovingUpCommand(InstanceId caller)
    : caller(caller) {}
StartMovingUpCommand::~StartMovingUpCommand() {}

void StartMovingUpCommand::exec(Game& game) {
    game.startMovingUp(caller);
}
