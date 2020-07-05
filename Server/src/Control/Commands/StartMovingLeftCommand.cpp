#include "../../../includes/Control/Commands/StartMovingLeftCommand.h"

StartMovingLeftCommand::StartMovingLeftCommand(const InstanceId caller)
    : Command(caller) {}
StartMovingLeftCommand::~StartMovingLeftCommand() {}

void StartMovingLeftCommand::exec(Game& game) {
    game.startMovingLeft(caller);
}
