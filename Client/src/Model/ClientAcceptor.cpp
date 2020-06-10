#include "../../includes/Model/ClientAcceptor.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ClientAcceptor::ClientAcceptor(const std::string& hostname, const std::string& port)
    : acceptor_skt(Socket(hostname, port)), is_running(true) {}

void ClientAcceptor::run() {
    /* creando el protocolo */
    while (this->is_running) {
        /* procesa los comandos y mandar al sercidor */
    }
}

bool ClientAcceptor::isRunning() {
    return this->is_running;
}

ClientAcceptor::~ClientAcceptor() {
    this->acceptor_skt.close();
}