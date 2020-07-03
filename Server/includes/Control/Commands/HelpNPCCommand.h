#ifndef __HELP_NPC_COMMAND_H__
#define __HELP_NPC_COMMAND_H__

#include "Command.h"

class HelpNPCCommand : public Command {
   private:
    InstanceId caller;
    uint32_t x_coord;
    uint32_t y_coord;

   public:
    HelpNPCCommand(InstanceId caller, const uint32_t x_coord,
                const uint32_t y_coord);
    ~HelpNPCCommand();

    void exec(Game& game) override;
};

#endif  // __HELP_NPC_COMMAND_H__
