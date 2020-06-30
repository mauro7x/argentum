#include "../../../includes/GameModel/Messages/MessageFactory.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Message* MessageFactory::newMessage(uint8_t message_type,
                                    const SocketWrapper& socket) {
    switch (message_type) {
        case ERROR_MSG: {
            std::string content;
            if (!(socket >> content)) {
                throw Exception(
                    "MessageFactory::newMessage: incomplete message received.");
            }
            return new PlainMessage(content, ERROR_MSG_COLOR);
        }

        case INFO_MSG: {
            std::string content;
            if (!(socket >> content)) {
                throw Exception(
                    "MessageFactory::newMessage: incomplete message received.");
            }
            return new PlainMessage(content, INFO_MSG_COLOR);
        }

        case SUCCESS_MSG: {
            std::string content;
            if (!(socket >> content)) {
                throw Exception(
                    "MessageFactory::newMessage: incomplete message received.");
            }
            return new PlainMessage(content, SUCCESS_MSG_COLOR);
        }

        case LIST_MSG: {
            throw Exception(
                "MessageFactory::newMessage: list messages not implemented "
                "yet.");
        }

        case GENERAL_MSG: {
            std::string sender;
            if (!(socket >> sender)) {
                throw Exception(
                    "MessageFactory::newMessage: incomplete message received.");
            }

            std::string content;
            if (!(socket >> content)) {
                throw Exception(
                    "MessageFactory::newMessage: incomplete message received.");
            }

            return new SignedMessage(sender, content, GENERAL_SIGNED_MSG,
                                     GENERAL_MSG_COLOR);
        }

        case PRIVATE_MSG: {
            std::string sender;
            if (!(socket >> sender)) {
                throw Exception(
                    "MessageFactory::newMessage: incomplete message received.");
            }

            std::string content;
            if (!(socket >> content)) {
                throw Exception(
                    "MessageFactory::newMessage: incomplete message received.");
            }

            return new SignedMessage(sender, content, PRIVATE_SIGNED_MSG,
                                     PRIVATE_MSG_COLOR);
        }

        default: {
            throw Exception(
                "MessageFactory::newMessage: invalid message type received.");
        }
    }
}

//-----------------------------------------------------------------------------
