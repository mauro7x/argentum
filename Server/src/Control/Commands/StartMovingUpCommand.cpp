#include "../../../includes/Control/Commands/StartMovingUpCommand.h"

StartMovingUpCommand::StartMovingUpCommand(const InstanceId caller)
    : Command(caller) {}
StartMovingUpCommand::~StartMovingUpCommand() {}

void StartMovingUpCommand::exec(Game& game) {
    game.startMovingUp(caller);
}
