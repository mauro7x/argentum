#include "../../includes/Model/ClientReceiver.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ClientReceiver::ClientReceiver(const std::string& hostname, const std::string& port)
    : receiver_skt(std::move(SocketWrapper(hostname, port))), keep_running(true) {}

void ClientReceiver::run() {
    /* creando el protocolo */
    while (this->keep_running) {
        /* recibir respuesta */
    }
}

void ClientReceiver::stopRunning() {
    this->keep_running = false;
}

ClientReceiver::~ClientReceiver() {
    this->receiver_skt.close();
}