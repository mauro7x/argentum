#include "../includes/ClientConnection.h"
#include <iostream>

ClientConnection::ClientConnection(int player_id, SocketWrapper&& sock) :
	socket(std::move(sock)) {
	id = player_id;
}

ClientConnection::~ClientConnection() {
}

ClientConnection::ClientConnection(ClientConnection&& other) : 
	socket(std::move(other.socket)) {
	id = other.id;
}

ClientConnection& ClientConnection::operator=(ClientConnection&& other) {
	// TODO: insert return statement here
	socket = std::move(other.socket);
	id = other.id;
	return *this;
}

void ClientConnection::run() {
	//read
	char opcode;
	while (socket.recv(&opcode, 1)) {
		std::cout << opcode << std::endl;
	}
}