#include "../includes/ClientList.h"

ClientList::ClientList() {

}

ClientList::~ClientList() {
}


void ClientList::executeCommand() {
	std::unique_lock<std::mutex>(m);
	std::unordered_map<int, ClientConnection>::iterator it = connection_map.begin();
	// Iterate over the map using iterator
	while (it != connection_map.end()) {
		it->second.execute_command();
		it++;
	}
}