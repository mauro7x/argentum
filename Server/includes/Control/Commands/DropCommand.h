#ifndef __DROP_COMMAND_H__
#define __DROP_COMMAND_H__

#include "Command.h"

class DropCommand : public Command {
   private:
    InstanceId caller;

    const uint8_t n_slot;
    const uint32_t amount;

   public:
    DropCommand(InstanceId caller, const uint8_t n_slot, const uint32_t amount);
    ~DropCommand();

    void exec(Game& game) override;
};

#endif  // __DROP_COMMAND_H__
