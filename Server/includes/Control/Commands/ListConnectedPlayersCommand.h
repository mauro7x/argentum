#ifndef __LIST_CONNECTED_PLAYERS_COMMAND_H__
#define __LIST_CONNECTED_PLAYERS_COMMAND_H__

#include "Command.h"

// Listar los jugadores conectados en el juego.
class ListConnectedPlayersCommand : public Command {
   public:
    ListConnectedPlayersCommand(const InstanceId caller);
    ~ListConnectedPlayersCommand();

    void exec(Game& game) override;
};

#endif  // __LIST_CONNECTED_PLAYERS_COMMAND_H__