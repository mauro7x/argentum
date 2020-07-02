#include "../../../includes/Control/Commands/TakeCommand.h"

TakeCommand::TakeCommand(InstanceId caller) : caller(caller) {}

TakeCommand::~TakeCommand() {}

void TakeCommand::exec(Game& game) {
    game.take(caller);
}
