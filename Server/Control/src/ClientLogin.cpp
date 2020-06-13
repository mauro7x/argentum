#include "../includes/ClientLogin.h"
#include <iostream>

ClientLogin::ClientLogin(ClientList* cli_list, LoginValidator* login_val, SocketWrapper&& sock)
	: socket(std::move(sock)),
	log_val(login_val),
	client_list(cli_list) {
	is_running = false;
}

ClientLogin::ClientLogin(ClientLogin&& other) :
	socket(std::move(other.socket)),
	log_val(other.log_val),
	client_list(other.client_list) {
}

void ClientLogin::run() {
	int id;
	if ((id = log_val->loginConnection(&socket)) != -1) {
		client_list->addPlayer(id, std::move(socket));
	}
	is_running = false;
}

bool ClientLogin::isRunning() {
	return is_running;
}

ClientLogin::~ClientLogin() {
}
