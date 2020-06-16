#include "../includes/ClientConnection.h"
#include <iostream>

ClientConnection::ClientConnection(int player_id, SocketWrapper&& sock) :
	socket(std::move(sock)) {
	id = player_id;
	socket_open = true;
}

ClientConnection::~ClientConnection() {
	if (t_receiver.joinable())
		t_receiver.join();
	if (t_sender.joinable())
		t_sender.join();
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

void ClientConnection::start() {
	t_receiver = std::thread(&ClientConnection::receive, this);
	t_sender = std::thread(&ClientConnection::send, this);
}

void ClientConnection::receive() {
	//read
	char opcode;
	while (socket.recv(&opcode, 1)) {
		// aca hay que crear comandos
		std::cout << opcode << std::endl;
	}
	socket_open = false;
}

void ClientConnection::send() {
	while (socket_open) {
		try {
			std::unique_lock<std::mutex> lock(send_mutex);
			while(response_queue.size() != 0){
				socket << response_queue.front();
				response_queue.pop();
			}
			send_cond_var.wait(lock);
		} catch (...) {
			socket_open = false;
		}
	}
}

void ClientConnection::writeResponse(bool good, std::string message) {
	std::unique_lock<std::mutex> lock(send_mutex);
	char start[1];
	start[0] = good;
	response_queue.push(std::string(start) + message);
	send_cond_var.notify_one();
}

void ClientConnection::writeBroadcast(bool initial, json status) {
	/*
	msg pack to do
	*/
}
