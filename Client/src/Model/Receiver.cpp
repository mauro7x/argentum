#include "../../includes/Model/Receiver.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Receiver::_receiveReply() const {
    throw Exception("Receiver::run: replies not implemented yet.");
}

void Receiver::_receivePrivateMessage() const {
    throw Exception("Receiver::run: private messages not implemented yet.");
}

void Receiver::_receiveBroadcast() const {
    uint8_t opcode;
    ssize_t received = 0;

    received = (socket >> opcode);
    if (!received) {
        throw Exception(
            "Receiver::_receiveBroadcast: incomplete broadcast received.");
    }

    fprintf(stderr, "recibiendo broadcast. opcode = %i\n", opcode);

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
    ssize_t received = 0;

    received = (socket >> entity_type);
    if (!received) {
        throw Exception(
            "Receiver::_receiveNewBroadcast: incomplete broadcast received.");
    }

    fprintf(stderr, "recibiendo new. entity_type = %i\n", entity_type);

    received = (socket >> serialized_broadcast);
    if (!received) {
        throw Exception(
            "Receiver::_receiveNewBroadcast: incomplete broadcast received.");
    }

    fprintf(stderr, "recibiendo new. ya recibimos el broadcast\n");

    switch (entity_type) {
        case PLAYER_TYPE: {
            if (!first_package_received) {
                PlayerData received_data;
                json j = json::from_msgpack(serialized_broadcast);
                received_data = j.get<PlayerData>();
                broadcasts.push(new NewPlayerBroadcast(received_data));
                first_package_received = true;
            }

            break;
        }

        case CHARACTER_TYPE: {
            if (first_package_received) {
                PlayerData received_data;
                json j = json::from_msgpack(serialized_broadcast);
                received_data = j.get<PlayerData>();
                broadcasts.push(
                    new NewCharacterBroadcast((CharacterData)received_data));
            }
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
    ssize_t received = 0;

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
            if (first_package_received) {
                PlayerData received_data;
                json j = json::from_msgpack(serialized_broadcast);
                received_data = j.get<PlayerData>();
                broadcasts.push(new UpdatePlayerBroadcast(received_data));
            }
            break;
        }

        case CHARACTER_TYPE: {
            if (first_package_received) {
                PlayerData received_data;
                json j = json::from_msgpack(serialized_broadcast);
                received_data = j.get<PlayerData>();
                broadcasts.push(
                    new UpdateCharacterBroadcast((CharacterData)received_data));
            }

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
    ssize_t received = 0;

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
            if (first_package_received) {
                broadcasts.push(new DeleteCharacterBroadcast(id));
            }

            break;
        }

        case CREATURE_TYPE: {
            if (first_package_received) {
                broadcasts.push(new DeleteCreatureBroadcast(id));
            }

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
                   std::atomic_bool& exit,
                   std::atomic_bool& first_package_received)
    : socket(socket),
      broadcasts(broadcasts),
      exit(exit),
      first_package_received(first_package_received) {}

void Receiver::run() {
    try {
        uint8_t opcode;
        while (socket >> opcode) {
            fprintf(stderr, "recibi un paquete. opcode = %i\n", opcode);
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
