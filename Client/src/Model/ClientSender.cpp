#include "../../includes/Model/ClientSender.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ClientSender::ClientSender(const std::string& hostname, const std::string& port,
                           std::queue<std::string>& queue)
    : sender_skt(std::move(SocketWrapper(hostname, port))),
      keep_running(true),
      queue_commands(queue) {}

void ClientSender::run() {
    /* creando el protocolo */
    // Protocol protocol;
    while (this->keep_running) {
        std::string command;
        if (this->queue_commands.empty()){continue;}
        command = this->queue_commands.front();
        this->queue_commands.pop();
        /* procesa los comandos y mandar al servidor */
        //protocol.send(this->talker_skt, command);
    }
}

void ClientSender::stopRunning() {
    this->keep_running = false;
}

ClientSender::~ClientSender() {
    this->sender_skt.close();
}
