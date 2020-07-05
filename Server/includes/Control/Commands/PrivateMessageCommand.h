#ifndef __PRIVATE_MESSAGE_COMMAND_H__
#define __PRIVATE_MESSAGE_COMMAND_H__

#include "Command.h"

// Envía un mensaje al jugador con el nickname especificado. Chat privado.
class PrivateMessageCommand : public Command {
   private:
    const std::string to_nickname;
    const std::string message;

   public:
    PrivateMessageCommand(const InstanceId caller,
                          const std::string to_nickname,
                          const std::string message);
    ~PrivateMessageCommand();

    void exec(Game& game) override;
};

#endif  // __PRIVATE_MESSAGE_COMMAND_H__
