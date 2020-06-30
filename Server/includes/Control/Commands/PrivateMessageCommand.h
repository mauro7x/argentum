#ifndef __PRIVATE_MESSAGE_COMMAND_H__
#define __PRIVATE_MESSAGE_COMMAND_H__

#include "Command.h"

class PrivateMessageCommand : public Command {
   private:
    InstanceId caller;

    std::string to_nickname;
    std::string message;

   public:
    PrivateMessageCommand(InstanceId caller, std::string to_nickname,
                          std::string message);
    ~PrivateMessageCommand();

    void exec(Game& game) override;
};

#endif  // __PRIVATE_MESSAGE_COMMAND_H__
