#include "../../includes/Control/Commands/MeditateCommand.h"

MeditateCommand::MeditateCommand(InstanceId caller) : caller(caller) {}

MeditateCommand::~MeditateCommand() {}

void MeditateCommand::exec(Game& game) {
    game.meditate(caller);
}
