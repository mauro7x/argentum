#include "../../includes/Model/Receiver.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Receiver::_receiveMessage() const {
    uint8_t message_type;

    if (!(socket >> message_type)) {
        throw Exception(
            "Receiver::_receiveMessage: incomplete message received.");
    }

    Message* new_message = MessageFactory::newMessage(message_type, socket);
    messages.push(new_message);
}

void Receiver::_receiveBroadcast() const {
    uint8_t broadcast_type, entity_type;

    if (!(socket >> broadcast_type)) {
        throw Exception(
            "Receiver::_receiveBroadcast: incomplete broadcast received.");
    }

    if (!(socket >> entity_type)) {
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
                   NonBlockingQueue<Message*>& messages, std::atomic_bool& exit)
    : socket(socket), broadcasts(broadcasts), messages(messages), exit(exit) {}

void Receiver::run() {
    try {
        uint8_t opcode;
        while (socket >> opcode) {
            switch (opcode) {
                case MESSAGE_OPCODE: {
                    _receiveMessage();
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
