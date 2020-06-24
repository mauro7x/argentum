#include "../../includes/Control/Commands/StartMovingRightCommand.h"

StartMovingRightCommand::StartMovingRightCommand(InstanceId caller)
    : caller(caller) {}
StartMovingRightCommand::~StartMovingRightCommand() {}

void StartMovingRightCommand::exec(Game& game) {
    game.startMovingRight(caller);
}
