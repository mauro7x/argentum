#include "../../../includes/Control/Commands/GeneralMessageCommand.h"

GeneralMessageCommand::GeneralMessageCommand(const InstanceId caller,
                                             const std::string message)
    : Command(caller),
      message(std::move(message)) {}

GeneralMessageCommand::~GeneralMessageCommand() {}

void GeneralMessageCommand::exec(Game& game) {
    game.sendGeneralMessage(caller, message);
}
