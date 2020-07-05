#ifndef __RESURRECT_COMMAND_H__
#define __RESURRECT_COMMAND_H__

#include "Command.h"

// Resucita a un jugador muerto mediante el sacerdote en la posición (x_coord,
// y_coord).
class ResurrectCommand : public Command {
   private:
    const uint32_t x_coord;
    const uint32_t y_coord;

   public:
    ResurrectCommand(const InstanceId caller, const uint32_t x_coord,
                     const uint32_t y_coord);
    ~ResurrectCommand();

    void exec(Game& game) override;
};

#endif  // __RESURRECT_COMMAND_H__
