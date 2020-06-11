#include "../includes/ClientConnection.h"

ClientConnection::ClientConnection(int player_id, SocketWrapper&& sock) {
	id = player_id;
}

ClientConnection::~ClientConnection() {
}