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
        // fprintf(stderr, "Inicia la ejecución del clientlogin.\n");

        CharacterCfg init_data = _login();

        // Si llegamos acá, el cliente ya se loggeo y podemos transferir
        // el socket con seguridad.

        new_connections.push(new NewConnection(peer, init_data));

        // Finaliza la ejecución
        // fprintf(stderr, "Finaliza la ejecución del clientlogin.\n");
    } catch (const std::exception& e) {
        // Error grave
        try {
            peer.shutdown();
        } catch (const Exception& e) {
            fprintf(stderr, "Warning: error while shutting-down client_login.");
        }
        peer.close();
        fprintf(stderr, "%s\n", e.what());
    } catch (...) {
        // Error desconocido
        try {
            peer.shutdown();
        } catch (const Exception& e) {
            fprintf(stderr, "Warning: error while shutting-down client_login.");
        }
        peer.close();
        fprintf(stderr, "Unknown error.\n");
    }

    is_running = false;
}

bool ClientLogin::isRunning() const {
    return is_running;
}

void ClientLogin::stop() {
    if (is_running) {
        // Soy consciente de que en este punto puede existir una RC muuy
        // oportuna, en el caso en que se cierre el server justo en el mismo
        // instante en que un cliente abandone la instancia de login. En este
        // caso, no habrá problema ya que el socket está preparado para hacer el
        // shutdown/close sólo si es necesario. -Mau.
        try {
            peer.shutdown();
        } catch (const Exception& e) {
            fprintf(stderr, "Warning: error while shutting-down client_login.");
        }
        peer.close();
    }
}

ClientLogin::~ClientLogin() {}

//-----------------------------------------------------------------------------
