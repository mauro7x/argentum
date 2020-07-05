#include "../../../includes/Control/Commands/StartMovingDownCommand.h"

StartMovingDownCommand::StartMovingDownCommand(const InstanceId caller)
    : Command(caller) {}
StartMovingDownCommand::~StartMovingDownCommand() {}

void StartMovingDownCommand::exec(Game& game) {
    game.startMovingDown(caller);
}
