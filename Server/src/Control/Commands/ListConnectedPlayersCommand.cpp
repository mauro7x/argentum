#include "../../../includes/Control/Commands/ListConnectedPlayersCommand.h"

ListConnectedPlayersCommand::ListConnectedPlayersCommand(
    const InstanceId caller)
    : Command(caller) {}

ListConnectedPlayersCommand::~ListConnectedPlayersCommand() {}

void ListConnectedPlayersCommand::exec(Game& game) {
    game.listConnectedPlayers(caller);
}
