#include "ClientLogin.h"

ClientLogin::ClientLogin(ClientList& cli_list, LoginValidator& login_val, SocketWrapper&& sock)
	: socket(sock) {
	log_val = login_val;
	client_list = cli_list;
}
