#ifndef __HEAL_COMMAND_H__
#define __HEAL_COMMAND_H__

#include "Command.h"

class HealCommand : public Command {
   private:
    InstanceId caller;
    uint32_t x_coord;
    uint32_t y_coord;

   public:
    HealCommand(InstanceId caller, const uint32_t x_coord,
                     const uint32_t y_coord);
    ~HealCommand();

    void exec(Game& game) override;
};

#endif  // __HEAL_COMMAND_H__
