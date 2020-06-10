#include "../../includes/Model/ClientTalker.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ClientTalker::ClientTalker(const std::string& hostname, const std::string& port,
                           std::queue<std::string>& queue)
    : talker_skt(Socket(hostname, port)),
      is_running(true),
      queue_commands(queue) {}

void ClientTalker::run() {
    /* creando el protocolo */
    // Protocol protocol;
    while (this->is_running) {
        std::string command;
        if (this->queue_commands.empty()){continue;}
        command = this->queue_commands.front();
        this->queue_commands.pop();
        /* procesa los comandos y mandar al servidor */
        //protocol.send(this->talker_skt, command);
    }
}

bool ClientTalker::isRunning() {
    return this->is_running;
}

ClientTalker::~ClientTalker() {
    this->talker_skt.close();
}
