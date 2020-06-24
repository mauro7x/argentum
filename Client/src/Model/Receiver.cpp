#include "../../includes/Model/Receiver.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Receiver::_receiveReply() const {
    // implementación proxy

    uint8_t opcode;
    std::string reply;
    size_t received = 0;

    received = (socket >> opcode);
    if (!received) {
        throw Exception("Receiver::_receiveReply: incomplete reply received.");
    }

    received = (socket >> reply);
    if (!received) {
        throw Exception("Receiver::_receiveReply: incomplete reply received.");
    }

    // switchear segun el tipo para el color
    fprintf(stderr, "REPLY RECIBIDA: %s\n", reply.c_str());
}

void Receiver::_receivePrivateMessage() const {
    throw Exception("Receiver::run: private messages not implemented yet.");
}

void Receiver::_receiveBroadcast() const {
    uint8_t broadcast_type, entity_type;
    size_t received = 0;

    received = (socket >> broadcast_type);
    if (!received) {
        throw Exception(
            "Receiver::_receiveBroadcast: incomplete broadcast received.");
    }

    received = (socket >> entity_type);
    if (!received) {
        throw Exception(
            "Receiver::_receiveBroadcast: incomplete broadcast received.");
    }

    Broadcast* new_broadcast =
        BroadcastFactory::newBroadcast(broadcast_type, entity_type, socket);
    broadcasts.push(new_broadcast);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Receiver::Receiver(const SocketWrapper& socket,
                   NonBlockingQueue<Broadcast*>& broadcasts,
                   std::atomic_bool& exit)
    : socket(socket), broadcasts(broadcasts), exit(exit) {}

void Receiver::run() {
    try {
        uint8_t opcode;
        while (socket >> opcode) {
            switch (opcode) {
                case REPLY_OPCODE: {
                    _receiveReply();
                    break;
                }

                case PRIVATE_MESSAGE_OPCODE: {
                    _receivePrivateMessage();
                    break;
                }

                case BROADCAST_OPCODE: {
                    _receiveBroadcast();
                    break;
                }

                default: {
                    throw Exception(
                        "Receiver::run: Unknown opcode received by "
                        "client.");
                }
            }
        }
    } catch (const std::exception& e) {
        fprintf(stderr, "Receiver // %s\n", e.what());
    } catch (...) {
        fprintf(stderr, "Receiver // Unknown error.\n");
    }

    // Avisamos que se cerró el socket y que hay que terminar
    exit = true;
}

Receiver::~Receiver() {}

//-----------------------------------------------------------------------------
