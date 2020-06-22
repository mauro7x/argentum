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
    uint8_t opcode;
    size_t received = 0;

    received = (socket >> opcode);
    if (!received) {
        throw Exception(
            "Receiver::_receiveBroadcast: incomplete broadcast received.");
    }

    switch (opcode) {
        case NEW_BROADCAST: {
            _receiveNewBroadcast();
            break;
        }

        case UPDATE_BROADCAST: {
            _receiveUpdateBroadcast();
            break;
        }

        case DELETE_BROADCAST: {
            _receiveDeleteBroadcast();
            break;
        }

        default: {
            throw Exception(
                "Receiver::_receiveBroadcast: invalid broadcast type "
                "received.");
        }
    }
}

void Receiver::_receiveNewBroadcast() const {
    uint8_t entity_type = 0;
    std::vector<uint8_t> serialized_broadcast;
    size_t received = 0;

    received = (socket >> entity_type);
    if (!received) {
        throw Exception(
            "Receiver::_receiveNewBroadcast: incomplete broadcast received.");
    }

    received = (socket >> serialized_broadcast);
    if (!received) {
        throw Exception(
            "Receiver::_receiveNewBroadcast: incomplete broadcast received.");
    }

    switch (entity_type) {
        case PLAYER_TYPE: {
            // Por ahora ignoramos este bug del servidor, lanzando un warning
            fprintf(stderr,
                    "WARNING: Reiceved new player broadcast again. This should "
                    "not be received more than once.\n");
            break;

            // Lo que deberíamos hacer
            throw Exception(
                "Receiver::_receiveNewBroadcast: received new player broadcast "
                "(invalid combination, this data should only be received when "
                "connecting).");
        }

        case CHARACTER_TYPE: {
            PlayerData received_data;
            json j = json::from_msgpack(serialized_broadcast);
            received_data = j.get<PlayerData>();
            broadcasts.push(
                new NewCharacterBroadcast((CharacterData)received_data));
            break;
        }

        case CREATURE_TYPE: {
            // Falta el json conversion todavia

            // CreatureData received_data;
            // json j = json::from_msgpack(serialized_broadcast);
            // received_data = j.get<CreatureData>();
            // broadcasts.push(new NewCreatureBroadcast(received_data));
            break;
        }

        case ITEM_TYPE: {
            throw Exception(
                "Receiver::_receiveNewBroadcast: items not implemented yet.");
            break;
        }

        default: {
            throw Exception(
                "Receiver::_receiveNewBroadcast: invalid entity type "
                "received.");
        }
    }
}

void Receiver::_receiveUpdateBroadcast() const {
    uint8_t entity_type = 0;
    std::vector<uint8_t> serialized_broadcast;
    size_t received = 0;

    received = (socket >> entity_type);
    if (!received) {
        throw Exception(
            "Receiver::_receiveUpdateBroadcast: incomplete broadcast "
            "received.");
    }

    received = (socket >> serialized_broadcast);
    if (!received) {
        throw Exception(
            "Receiver::_receiveUpdateBroadcast: incomplete broadcast "
            "received.");
    }

    switch (entity_type) {
        case PLAYER_TYPE: {
            PlayerData received_data;
            json j = json::from_msgpack(serialized_broadcast);
            received_data = j.get<PlayerData>();
            broadcasts.push(new UpdatePlayerBroadcast(received_data));
            break;
        }

        case CHARACTER_TYPE: {
            PlayerData received_data;
            json j = json::from_msgpack(serialized_broadcast);
            received_data = j.get<PlayerData>();
            broadcasts.push(
                new UpdateCharacterBroadcast((CharacterData)received_data));
            break;
        }

        case CREATURE_TYPE: {
            // Falta el json conversion todavia

            // CreatureData received_data;
            // json j = json::from_msgpack(serialized_broadcast);
            // received_data = j.get<CreatureData>();
            // broadcasts.push(new UpdateCreatureBroadcast(received_data));
            break;
        }

        case ITEM_TYPE: {
            throw Exception(
                "Receiver::_receiveUpdateBroadcast: items not implemented "
                "yet.");
            break;
        }

        default: {
            throw Exception(
                "Receiver::_receiveUpdateBroadcast: invalid entity type "
                "received.");
        }
    }
}

void Receiver::_receiveDeleteBroadcast() const {
    uint8_t entity_type = 0;
    uint32_t id = 0;
    size_t received = 0;

    received = (socket >> entity_type);
    if (!received) {
        throw Exception(
            "Receiver::_receiveDeleteBroadcast: incomplete broadcast "
            "received.");
    }

    received = (socket >> id);
    if (!received) {
        throw Exception(
            "Receiver::_receiveDeleteBroadcast: incomplete broadcast "
            "received.");
    }

    switch (entity_type) {
        case PLAYER_TYPE: {
            throw Exception(
                "Receiver::_receiveDeleteBroadcast: received player delete "
                "broadcast.");
            break;
        }

        case CHARACTER_TYPE: {
            broadcasts.push(new DeleteCharacterBroadcast(id));
            break;
        }

        case CREATURE_TYPE: {
            broadcasts.push(new DeleteCreatureBroadcast(id));
            break;
        }

        case ITEM_TYPE: {
            throw Exception(
                "Receiver::_receiveDeleteBroadcast: items not implemented "
                "yet.");
            break;
        }

        default: {
            throw Exception(
                "Receiver::_receiveDeleteBroadcast: invalid entity type "
                "received.");
        }
    }
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
