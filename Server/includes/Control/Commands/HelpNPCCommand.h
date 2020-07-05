#ifndef __HELP_NPC_COMMAND_H__
#define __HELP_NPC_COMMAND_H__

#include "Command.h"

// Muestra un mensaje de ayuda con los comandos del NPC en la posición (x_coord,
// y_coord).
class HelpNPCCommand : public Command {
   private:
    const uint32_t x_coord;
    const uint32_t y_coord;

   public:
    HelpNPCCommand(const InstanceId caller, const uint32_t x_coord,
                   const uint32_t y_coord);
    ~HelpNPCCommand();

    void exec(Game& game) override;
};

#endif  // __HELP_NPC_COMMAND_H__
