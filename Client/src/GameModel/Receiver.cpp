#include "../../includes/GameModel/Receiver.h"

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

void Receiver::_receiveEvent() const {
    uint8_t event_type;
    uint32_t x;
    uint32_t y;

    if (!(socket >> event_type)) {
        throw Exception("Receiver::_receiveEvent: incomplete event received.");
    }

    if (!(socket >> x)) {
        throw Exception("Receiver::_receiveEvent: incomplete event received.");
    }

    if (!(socket >> y)) {
        throw Exception("Receiver::_receiveEvent: incomplete event received.");
    }

    GameEvent* new_event = new GameEvent(event_type, x, y);
    events.push(new_event);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Receiver::Receiver(const SocketWrapper& socket,
                   NonBlockingQueue<Broadcast*>& broadcasts,
                   NonBlockingQueue<Message*>& messages,
                   NonBlockingQueue<GameEvent*>& events, GameView& game_view)
    : socket(socket),
      broadcasts(broadcasts),
      messages(messages),
      events(events),
      game_view(game_view) {}

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

                case EVENT_OPCODE: {
                    _receiveEvent();
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
    game_view.quit();
}

Receiver::~Receiver() {}

//-----------------------------------------------------------------------------
