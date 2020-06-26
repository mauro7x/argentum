#include "../../../includes/Model/Broadcasts/BroadcastFactory.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Broadcast* BroadcastFactory::newBroadcast(uint8_t broadcast_type,
                                          uint8_t entity_type,
                                          const SocketWrapper& socket) {
    switch (broadcast_type) {
        case NEW_BROADCAST: {
            std::vector<uint8_t> serialized;
            if (!(socket >> serialized)) {
                throw Exception(
                    "BroadcastFactory::newBroadcast: incomplete broadcast "
                    "received.");
            }
            json j = json::from_msgpack(serialized);

            switch (entity_type) {
                case PLAYER_TYPE: {
                    throw Exception(
                        "BroadcastFactory::newBroadcast: received new player "
                        "broadcast (invalid combination, this data should only "
                        "be received when connecting).");
                }

                case CHARACTER_TYPE: {
                    PlayerData received_data = j.get<PlayerData>();
                    return new NewCharacterBroadcast(
                        (CharacterData)received_data);
                }

                case CREATURE_TYPE: {
                    CreatureData received_data = j.get<CreatureData>();
                    return new NewCreatureBroadcast(received_data);
                }

                case ITEM_TYPE: {
                    ItemData received_data = j.get<ItemData>();
                    return new NewItemBroadcast(received_data);
                }

                default: {
                    throw Exception(
                        "BroadcastFactory::newBroadcast: invalid entity type "
                        "received.");
                }
            }
        }

        case UPDATE_BROADCAST: {
            std::vector<uint8_t> serialized;
            if (!(socket >> serialized)) {
                throw Exception(
                    "BroadcastFactory::newBroadcast: incomplete broadcast "
                    "received.");
            }
            json j = json::from_msgpack(serialized);

            switch (entity_type) {
                case PLAYER_TYPE: {
                    PlayerData received_data = j.get<PlayerData>();
                    return new UpdatePlayerBroadcast(received_data);
                }

                case CHARACTER_TYPE: {
                    PlayerData received_data = j.get<PlayerData>();
                    return new UpdateCharacterBroadcast(
                        (CharacterData)received_data);
                }

                case CREATURE_TYPE: {
                    CreatureData received_data = j.get<CreatureData>();
                    return new UpdateCreatureBroadcast(received_data);
                }

                case ITEM_TYPE: {
                    throw Exception(
                        "BroadcastFactory::newBroadcast: item update received "
                        "(items does not have updates!).");
                }

                default: {
                    throw Exception(
                        "BroadcastFactory::newBroadcast: invalid entity type "
                        "received.");
                }
            }
        }

        case DELETE_BROADCAST: {
            switch (entity_type) {
                case PLAYER_TYPE: {
                    throw Exception(
                        "BroadcastFactory::newBroadcast: player "
                        "delete-broadcast received.");
                }

                case CHARACTER_TYPE: {
                    uint32_t id;
                    if (!(socket >> id)) {
                        throw Exception(
                            "BroadcastFactory::newBroadcast: incomplete "
                            "broadcast received.");
                    }

                    return new DeleteCharacterBroadcast(id);
                }

                case CREATURE_TYPE: {
                    uint32_t id;
                    if (!(socket >> id)) {
                        throw Exception(
                            "BroadcastFactory::newBroadcast: incomplete "
                            "broadcast received.");
                    }
                    return new DeleteCreatureBroadcast(id);
                }

                case ITEM_TYPE: {
                    uint32_t x, y;
                    if (!(socket >> x)) {
                        throw Exception(
                            "BroadcastFactory::newBroadcast: incomplete "
                            "broadcast received.");
                    }

                    if (!(socket >> y)) {
                        throw Exception(
                            "BroadcastFactory::newBroadcast: incomplete "
                            "broadcast received.");
                    }

                    return new DeleteItemBroadcast(x, y);
                }

                default: {
                    throw Exception(
                        "BroadcastFactory::newBroadcast: invalid entity type "
                        "received.");
                }
            }
        }

        default: {
            throw Exception(
                "BroadcastFactory::newBroadcast: invalid broadcast type "
                "received.");
        }
    }
}

//-----------------------------------------------------------------------------
