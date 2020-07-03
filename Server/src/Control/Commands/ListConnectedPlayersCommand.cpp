#include "../../../includes/Control/Commands/ListConnectedPlayersCommand.h"

ListConnectedPlayersCommand::ListConnectedPlayersCommand(InstanceId caller)
    : caller(caller) {}

ListConnectedPlayersCommand::~ListConnectedPlayersCommand() {}

void ListConnectedPlayersCommand::exec(Game& game) {
    game.listConnectedPlayers(caller);
}
