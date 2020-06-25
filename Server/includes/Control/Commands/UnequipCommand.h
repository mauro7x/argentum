#ifndef __UNEQUIP_COMMAND_H__
#define __UNEQUIP_COMMAND_H__

#include "Command.h"

// comando para desequipar los objetos del equipamiento
class UnequipCommand : public Command {
   private:
    InstanceId caller;

    const uint8_t n_slot;

   public:
    UnequipCommand(InstanceId caller, const uint8_t n_slot);
    ~UnequipCommand();

    void exec(Game& game) override;
};

#endif  // __UNEQUIP_COMMAND_H__