#ifndef __LIST_CONNECTED_PLAYERS_COMMAND_H__
#define __LIST_CONNECTED_PLAYERS_COMMAND_H__

#include "Command.h"

// comando para equipar los objetos del inventario
class ListConnectedPlayersCommand : public Command {
   private:
    InstanceId caller;

   public:
    ListConnectedPlayersCommand(const InstanceId caller);
    ~ListConnectedPlayersCommand();

    void exec(Game& game) override;
};

#endif  // __LIST_CONNECTED_PLAYERS_COMMAND_H__