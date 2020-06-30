#include "../../includes/Control/Commands/GeneralMessageCommand.h"

GeneralMessageCommand::GeneralMessageCommand(InstanceId caller,
                                             const std::string message)
    : caller(caller),
      message(std::move(message)) {}

GeneralMessageCommand::~GeneralMessageCommand() {}

void GeneralMessageCommand::exec(Game& game) {
    game.sendGeneralMessage(caller, message);
}
