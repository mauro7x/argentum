#include "../../includes/Control/Commands/StartMovingDownCommand.h"

StartMovingDownCommand::StartMovingDownCommand(InstanceId caller)
    : caller(caller) {}
StartMovingDownCommand::~StartMovingDownCommand() {}

void StartMovingDownCommand::exec(Game& game) {
    game.startMovingDown(caller);
}
