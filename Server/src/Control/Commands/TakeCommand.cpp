#include "../../../includes/Control/Commands/TakeCommand.h"

TakeCommand::TakeCommand(const InstanceId caller) : Command(caller) {}

TakeCommand::~TakeCommand() {}

void TakeCommand::exec(Game& game) {
    game.take(caller);
}
