#ifndef __TELEPORT_COMMAND_H__
#define __TELEPORT_COMMAND_H__

#include "Command.h"

// Usar un portal y teletransportarse a otro mapa.
class TeleportCommand : public Command {
   private:
    const uint32_t x_coord;
    const uint32_t y_coord;

   public:
    TeleportCommand(const InstanceId caller, const uint32_t x_coord,
                     const uint32_t y_coord);
    ~TeleportCommand();

    void exec(Game& game) override;
};

#endif  // __TELEPORT_COMMAND_H__
