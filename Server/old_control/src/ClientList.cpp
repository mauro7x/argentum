#include "../includes/ClientList.h"
#include <iostream>

ClientList::ClientList() {
}

bool ClientList::addPlayer(int player_id, SocketWrapper&& sock) {
	std::unique_lock<std::mutex> lock(m);
	auto search = connection_map.find(player_id);
	if (search != connection_map.end()) {
		std::cout << "Found connection" << std::endl;
		return false;
	}

	ClientConnection con(player_id, std::move(sock));
	connection_map.insert(std::make_pair(player_id, std::move(con) ));
	connection_map.at(player_id).start();
	std::cout << "Succesfull connection" << std::endl;
	return true;
}

ClientList::~ClientList() {
}