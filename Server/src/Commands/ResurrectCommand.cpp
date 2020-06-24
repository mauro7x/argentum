#include "../../includes/Control/Commands/ResurrectCommand.h"

ResurrectCommand::ResurrectCommand(InstanceId caller) : caller(caller) {}

ResurrectCommand::~ResurrectCommand() {}

void ResurrectCommand::exec(Game& game) {
    game.resurrect(caller);
}
