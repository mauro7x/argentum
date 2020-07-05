#include "../../../includes/Control/Commands/MeditateCommand.h"

MeditateCommand::MeditateCommand(const InstanceId caller) : Command(caller) {}

MeditateCommand::~MeditateCommand() {}

void MeditateCommand::exec(Game& game) {
    game.meditate(caller);
}
