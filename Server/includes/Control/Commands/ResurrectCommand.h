#ifndef __RESURRECT_COMMAND_H__
#define __RESURRECT_COMMAND_H__

#include "Command.h"

class ResurrectCommand : public Command {
   private:
    InstanceId caller;
    uint32_t x_coord;
    uint32_t y_coord;

   public:
    ResurrectCommand(InstanceId caller, const uint32_t x_coord,
                     const uint32_t y_coord);
    ~ResurrectCommand();

    void exec(Game& game) override;
};

#endif  // __RESURRECT_COMMAND_H__
