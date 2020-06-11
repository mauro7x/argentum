#include "../includes/ClientConnection.h"

ClientConnection::ClientConnection(int player_id, Socket&& sock) {
	id = player_id;
}

ClientConnection::~ClientConnection() {
}

void ClientConnection::execute_command() {

}
