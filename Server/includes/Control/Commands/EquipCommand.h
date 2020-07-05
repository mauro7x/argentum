#ifndef __EQUIP_COMMAND_H__
#define __EQUIP_COMMAND_H__

#include "Command.h"

// Equipa el objeto en el n_slot.
class EquipCommand : public Command {
   private:
    const uint8_t n_slot;

   public:
    EquipCommand(const InstanceId caller, const uint8_t n_slot);
    ~EquipCommand();

    void exec(Game& game) override;
};

#endif  // __EQUIP_COMMAND_H__