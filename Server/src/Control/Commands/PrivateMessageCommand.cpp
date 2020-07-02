#include "../../../includes/Control/Commands/PrivateMessageCommand.h"

PrivateMessageCommand::PrivateMessageCommand(InstanceId caller,
                                             const std::string to_nickname,
                                             const std::string message)
    : caller(caller),
      to_nickname(std::move(to_nickname)),
      message(std::move(message)) {}

PrivateMessageCommand::~PrivateMessageCommand() {}

void PrivateMessageCommand::exec(Game& game) {
    game.sendPrivateMessage(caller, to_nickname, message);
}
