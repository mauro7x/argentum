#ifndef __UNEQUIP_COMMAND_H__
#define __UNEQUIP_COMMAND_H__

#include "Command.h"

// Desequipar el objeto en el n_slot especificado.
class UnequipCommand : public Command {
    const uint8_t n_slot;

   public:
    UnequipCommand(const InstanceId caller, const uint8_t n_slot);
    ~UnequipCommand();

    void exec(Game& game) override;
};

#endif  // __UNEQUIP_COMMAND_H__