#ifndef __GENERAL_MESSAGE_COMMAND_H__
#define __GENERAL_MESSAGE_COMMAND_H__

#include "Command.h"

class GeneralMessageCommand : public Command {
   private:
    InstanceId caller;

    std::string message;

   public:
    GeneralMessageCommand(InstanceId caller, std::string message);
    ~GeneralMessageCommand();

    void exec(Game& game) override;
};

#endif  // __GENERAL_MESSAGE_COMMAND_H__
