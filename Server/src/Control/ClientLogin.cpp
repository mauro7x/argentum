#include "../../includes/Control/ClientLogin.h"

//-----------------------------------------------------------------------------
// Métodos privados

// void ClientLogin::_loginProxy() {
//     bool connected = false;

//     while (!connected) {
//         try {
//             database.signIn("mauro", "123");
//             connected = true;
//         } catch (const LoginException& e) {
//             fprintf(stderr, "LOGIN_ERROR: %s\n", e.what());
//         }
//     }
// }

//-----------------------------------------------------------------------------
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
                try {
                    database.signIn(username, password, character_data);
                    connected = true;
                } catch (const LoginException& e) {
                    Reply reply(ERROR_REPLY, e.what());
                    reply.send(0, peer);
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

                try {
                    database.signUp(username, password, race, kind, head_id,
                                    body_id, character_data);
                    std::string msg_create =
                        "Personaje fue creado exitosamente!";
                    Reply reply_create(SUCCESS_REPLY, msg_create);
                    reply_create.send(0, peer);
                } catch (const LoginException& e) {
                    Reply reply(ERROR_REPLY, e.what());
                    reply.send(0, peer);
                }
                break;
            }

            default: {
                throw Exception(
                    "ClientLogin::_login: invalid opcode received.");
            }
        }
    }
    std::string msg = "Entrando al juego !";
    Reply reply(SUCCESS_REPLY, msg);
    reply.send(0, peer);
    return character_data;
}
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
        peer.shutdown();
        peer.close();
        fprintf(stderr, "%s\n", e.what());
    } catch (...) {
        // Error desconocido
        peer.shutdown();
        peer.close();
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
