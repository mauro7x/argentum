#ifndef __GENERAL_MESSAGE_COMMAND_H__
#define __GENERAL_MESSAGE_COMMAND_H__

#include "Command.h"

// Envía un mensaje a todos los jugadores. Chat general.
class GeneralMessageCommand : public Command {
   private:
    const std::string message;

   public:
    GeneralMessageCommand(const InstanceId caller, const std::string message);
    ~GeneralMessageCommand();

    void exec(Game& game) override;
};

#endif  // __GENERAL_MESSAGE_COMMAND_H__
