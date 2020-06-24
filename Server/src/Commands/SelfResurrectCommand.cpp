#include "../../includes/Control/Commands/SelfResurrectCommand.h"

SelfResurrectCommand::SelfResurrectCommand(InstanceId caller) : caller(caller) {}

SelfResurrectCommand::~SelfResurrectCommand() {}

void SelfResurrectCommand::exec(Game& game) {
    game.resurrect(caller);
}
