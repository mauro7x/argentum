#include "../../includes/Control/ClientLogin.h"

//-----------------------------------------------------------------------------
// Métodos privados

CharacterCfg ClientLogin::_login() {
    bool connected = false;
    uint8_t opcode;
    // size_t received;
    std::string username, password;
    CharacterCfg character_data;
    // enviar info kind race;

    while (!connected) {
        if (!(peer >> opcode)) {
            throw Exception(
                "ClientLogin::_login: socket was closed before expected.");
        }

        if (!(peer >> username)) {
            throw Exception(
                "ClientLogin::_login: socket was closed before expected.");
        }

        if (!(peer >> password)) {
            throw Exception(
                "ClientLogin::_login: socket was closed before expected.");
        }
        switch (opcode) {
            case SIGN_IN_OPCODE: {
                ConnectionAckType status =
                    database.signIn(username, password, character_data);
                if (status == SUCCESS_ACK) {
                    connected = true;
                }

                // Le enviamos el ack al cliente
                if (!(peer << (uint8_t)CONNECTION_ACK_OPCODE)) {
                    throw Exception(
                        "ClientLogin::_login: socket was closed before "
                        "expected.");
                }

                if (!(peer << (uint8_t)status)) {
                    throw Exception(
                        "ClientLogin::_login: socket was closed before "
                        "expected.");
                }

                break;
            }

            case SIGN_UP_OPCODE: {
                uint32_t race, kind, head_id, body_id;

                if (!(peer >> race)) {
                    throw Exception(
                        "ClientLogin::_login: socket was closed before "
                        "expected.");
                }

                if (!(peer >> kind)) {
                    throw Exception(
                        "ClientLogin::_login: socket was closed before "
                        "expected.");
                }

                if (!(peer >> head_id)) {
                    throw Exception(
                        "ClientLogin::_login: socket was closed before "
                        "expected.");
                }

                if (!(peer >> body_id)) {
                    throw Exception(
                        "ClientLogin::_login: socket was closed before "
                        "expected.");
                }

                ConnectionAckType status =
                    database.signUp(username, password, race, kind, head_id,
                                    body_id, character_data);

                // Le enviamos el ack al cliente
                if (!(peer << (uint8_t)CONNECTION_ACK_OPCODE)) {
                    throw Exception(
                        "ClientLogin::_login: socket was closed before "
                        "expected.");
                }

                if (!(peer << (uint8_t)status)) {
                    throw Exception(
                        "ClientLogin::_login: socket was closed before "
                        "expected.");
                }

                break;
            }

            default: {
                throw Exception(
                    "ClientLogin::_login: invalid opcode received.");
            }
        }
    }

    return character_data;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ClientLogin::ClientLogin(SocketWrapper& peer, Database& database,
                         NonBlockingQueue<NewConnection*>& new_connections)
    : is_running(false),
      peer(std::move(peer)),
      database(database),
      new_connections(new_connections) {}

void ClientLogin::run() {
    is_running = true;

    try {
        // Comienza la ejecución del clientlogin
        fprintf(stderr, "Inicia la ejecución del clientlogin.\n");

        CharacterCfg init_data = _login();
        new_connections.push(new NewConnection(peer, init_data));

        // ACA NO SE DEBERÍA HACER NADA QUE FALLE, PORQUE EL SOCKET YA NO
        // ES NUESTRO, ASI QUE OJO

        // Finaliza la ejecución
        fprintf(stderr, "Finaliza la ejecución del clientlogin.\n");
    } catch (const std::exception& e) {
        // Error grave
        fprintf(stderr, "%s\n", e.what());
    } catch (...) {
        // Error desconocido
        fprintf(stderr, "Unknown error.\n");
    }

    is_running = false;
}

bool ClientLogin::isRunning() const {
    return is_running;
}

void ClientLogin::stop() {
    peer.shutdown();
    peer.close();
}

ClientLogin::~ClientLogin() {}

//-----------------------------------------------------------------------------
