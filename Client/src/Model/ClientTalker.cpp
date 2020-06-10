#include "../../includes/Model/ClientTalker.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ClientTalker::ClientTalker(const std::string& hostname, const std::string& port)
    : talker_skt(Socket(hostname, port)), is_running(true) {}

void ClientTalker::run() {
    /* creando el protocolo */
    while (this->is_running) {
        /* procesa los comandos y mandar al sercidor */
    }
}

bool ClientTalker::isRunning() {
    return this->is_running;
}

ClientTalker::~ClientTalker() {
    this->talker_skt.close();
}
