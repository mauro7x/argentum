#ifndef __DROP_COMMAND_H__
#define __DROP_COMMAND_H__

#include "Command.h"

// Dropea el objeto en el n_slot.
class DropCommand : public Command {
   private:
    const uint8_t n_slot;
    const uint32_t amount;

   public:
    DropCommand(const InstanceId caller, const uint8_t n_slot,
                const uint32_t amount);
    ~DropCommand();

    void exec(Game& game) override;
};

#endif  // __DROP_COMMAND_H__
