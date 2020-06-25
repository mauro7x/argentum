#ifndef __EQUIP_COMMAND_H__
#define __EQUIP_COMMAND_H__

#include "Command.h"

// comando para equipar los objetos del inventario
class EquipCommand : public Command {
   private:
    InstanceId caller;

    const uint8_t n_slot;

   public:
    EquipCommand(InstanceId caller, const uint8_t n_slot);
    ~EquipCommand();

    void exec(Game& game) override;
};

#endif  // __EQUIP_COMMAND_H__