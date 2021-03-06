#include "../../../includes/Control/Commands/SelfResurrectCommand.h"

SelfResurrectCommand::SelfResurrectCommand(const InstanceId caller)
    : Command(caller) {}

SelfResurrectCommand::~SelfResurrectCommand() {}

void SelfResurrectCommand::exec(Game& game) {
    game.resurrect(caller);
}
