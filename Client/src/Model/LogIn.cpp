#include "../../includes/Model/LogIn.h"

//-----------------------------------------------------------------------------
// Métodos privados

void LogInProxy::_login() const {
    bool sucessed = false;
    std::string opcode, username, password;
    size_t sended;
    // deberia recibir info de race y kind
    while (!sucessed) {
        std::cout << "\n> Ingrese 'signin' para entrar al juego, o 'signup' "
                     "para crear el personaje, o presione entrer para salir\n";
        std::cout << "\t> Option: ";
        std::getline(std::cin, opcode);
        if (opcode == "signin") {
            sended = socket << (char)SIGN_IN_OPCODE;
            if (!sended) {
                throw Exception("error in LogInProxt send opcode");
            }
        } else if (opcode == "signup") {
            sended = socket << (char)SIGN_UP_OPCODE;
            if (!sended) {
                throw Exception("error in LogInProxt send opcode");
            }
        } else {
            throw;
        }
        std::cout << "\n> Ingrese username y password\n";
        std::cout << "\t> Username: ";
        std::getline(std::cin, username);
        sended = socket << username;
        if (!sended) {
            throw Exception("error in LogInProxt send username");
        }
        std::cout << "\t> Password: ";
        std::getline(std::cin, password);
        sended = socket << password;
        if (!sended) {
            throw Exception("error in LogInProxt send password");
        }

        std::cout << "[DEBUG] Username ingresado: " << username << "\n";
        std::cout << "[DEBUG] Password ingresado: " << password << "\n";

        if (opcode == "signup") {
            std::string race, kind, head_id, body_id;
            std::cout << "\n> Ingrese race y kind para crear el personaje \n";
            std::cout << "\t> Race: ";
            std::getline(std::cin, race);
            sended = socket << uint32_t(std::stoi(race));
            if (!sended) {
                throw Exception("error in LogInProxt send race");
            }
            std::cout << "\t> Kind: ";
            std::getline(std::cin, kind);
            sended = socket << uint32_t(std::stoi(kind));
            if (!sended) {
                throw Exception("error in LogInProxt send kind");
            }
            std::cout << "\t> Head_id: ";
            std::getline(std::cin, head_id);
            sended = socket << uint32_t(std::stoi(head_id));
            if (!sended) {
                throw Exception("error in LogInProxt send kind");
            }
            std::cout << "\t> Body_id: ";
            std::getline(std::cin, body_id);
            sended = socket << uint32_t(std::stoi(body_id));
            if (!sended) {
                throw Exception("error in LogInProxt send kind");
            }
        }

        uint8_t typecode;
        size_t received;
        std::string message, try_again;
        received = socket >> typecode;
        if (!received) {
            throw Exception("error in LogInProxt receive typecode of message");
        }
        
        received = socket >> typecode;
        if (!received) {
            throw Exception("error in LogInProxt receive typecode of message");
        }

        received = socket >> message;
        if (!received) {
            throw Exception("error in LogInProxt receive  message");
        }

        if (typecode == SUCCESS_REPLY) {
            sucessed = true;
        } else {
            std::cout << "\n> " << message
                      << "Ingrese 'y' para volver a "
                         "intentar, o presione enter para salir: \n";
            std::getline(std::cin, try_again);
            if (try_again != "y") {
                throw;
            }
        }
    }
}
void LogInProxy::_receiveFirstPackage() const {
    uint8_t opcode = 0, entity_type = 0;
    size_t received;

    // Recibimos el primer paquete según el procolo:
    // OP (1) - BROADCAST_OP (1) - ENTITY_TYPE (1) - LENGTH (4) - DATA
    // (LENGTH)

    received = (socket >> opcode);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first "
            "package data "
            "(socket was closed).");
    } else if (opcode != BROADCAST_OPCODE) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: received another "
            "package before "
            "the initial data.");
    }

    received = (socket >> opcode);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first "
            "package data "
            "(socket was closed).");
    } else if (opcode != NEW_BROADCAST) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: received another "
            "package before "
            "the initial data.");
    }

    received = (socket >> entity_type);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first "
            "package data "
            "(socket was closed).");
    } else if (entity_type != PLAYER_TYPE) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: received another "
            "package before "
            "the initial data.");
    }

    std::vector<uint8_t> serialized_data;
    received = (socket >> serialized_data);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first "
            "package data "
            "(socket was closed).");
    }

    PlayerData received_data;
    json j = json::from_msgpack(serialized_data);
    received_data = j.get<PlayerData>();
    broadcasts.push(new NewPlayerBroadcast(received_data));
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

LogInProxy::LogInProxy(SocketWrapper& socket,
                       NonBlockingQueue<Broadcast*>& broadcasts)
    : socket(socket), broadcasts(broadcasts) {}

void LogInProxy::operator()() const {
    /* Acá debe ir toda la lógica del sign-in / sign-up. */

    /* Por ahora el server nos enviará la data apenas nos conectemos al
     * mismo, sin esta parte, por lo que la salteamos asumiendo que la
     * próxima data que nos llegue del mismo será la data inicial de
     * nuestro jugador. */
    _login();
    _receiveFirstPackage();
}

LogInProxy::~LogInProxy() {}

//-----------------------------------------------------------------------------
