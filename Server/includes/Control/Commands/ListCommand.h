#ifndef __LIST_COMMAND_H__
#define __LIST_COMMAND_H__

#include "Command.h"

// Listar los elementos que ofrece/contiene un NPC.
class ListCommand : public Command {
   private:
    const uint32_t x_coord;
    const uint32_t y_coord;

   public:
    ListCommand(const InstanceId caller, const uint32_t x_coord,
                const uint32_t y_coord);
    ~ListCommand();

    void exec(Game& game) override;
};

#endif  // __LIST_COMMAND_H__
