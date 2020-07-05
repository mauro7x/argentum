#ifndef __HEAL_COMMAND_H__
#define __HEAL_COMMAND_H__

#include "Command.h"

// /curar del sacerdote. Cura puntos de maná y vida al máximo.
class HealCommand : public Command {
   private:
    const uint32_t x_coord;
    const uint32_t y_coord;

   public:
    HealCommand(const InstanceId caller, const uint32_t x_coord,
                const uint32_t y_coord);
    ~HealCommand();

    void exec(Game& game) override;
};

#endif  // __HEAL_COMMAND_H__
