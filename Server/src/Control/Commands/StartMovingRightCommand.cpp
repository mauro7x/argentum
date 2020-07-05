#include "../../../includes/Control/Commands/StartMovingRightCommand.h"

StartMovingRightCommand::StartMovingRightCommand(const InstanceId caller)
    : Command(caller) {}
StartMovingRightCommand::~StartMovingRightCommand() {}

void StartMovingRightCommand::exec(Game& game) {
    game.startMovingRight(caller);
}
