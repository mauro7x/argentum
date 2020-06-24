#include "../../includes/Control/Commands/StartMovingLeftCommand.h"

StartMovingLeftCommand::StartMovingLeftCommand(InstanceId caller)
    : caller(caller) {}
StartMovingLeftCommand::~StartMovingLeftCommand() {}

void StartMovingLeftCommand::exec(Game& game) {
    game.startMovingLeft(caller);
}
