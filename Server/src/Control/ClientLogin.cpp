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
    size_t received;
    std::string username, password;
    CharacterCfg character_data;
    //enviar info kind race;
    while (!connected) {
        received = peer >> opcode;
        if (! received) {
            throw Exception("socket is closed");
        }
        received = peer >> username;
        if (! received) {
            throw Exception("socket is closed");
        }
        received = peer >> password;
        if (! received) {
            throw Exception("socket is closed");
        }
        switch (opcode) {
            case SIGN_IN_OPCODE:
                try {
                    database.signIn(username, password, character_data);
                    connected = true;
                } catch (const Exception& e) {
                    NotificationReply reply(ERROR_REPLY, e.what());
                    reply.send(0,peer);
                }

                break;
            case SIGN_UP_OPCODE:
                uint32_t race, kind;
                received = peer >> race;
                if (! received) {
                    throw Exception("socket is closed");
                }
                received = peer >> kind;
                if (! received) {
                    throw Exception("socket is closed");
                }                
                try {
                    database.signUp(username, password, race, kind, character_data);
                    connected = true;
                } catch (const Exception& e) {
                    NotificationReply reply(ERROR_REPLY, e.what());
                    reply.send(0,peer);
                }
                break;

            default:
                throw Exception("Opcode invalid") ;
                break;
        }
    }
    std::string msg = "entrando al juego";
    NotificationReply reply(SUCCESS_REPLY, msg);
    reply.send(0,peer);
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

        // Loggear al usuario o crear al usuario
        // por ahora proxy:
        // _loginProxy();

        // En este punto, vamos a tener la data del jugador conectado, por lo
        // que agregamos la petición de crear jugador a la cola y terminamos

        // esta data deberia venir el login
        CharacterCfg init_data;
        init_data = _login();
        // init_data.race = 100;
        // init_data.kind = 201;
        // init_data.state = ALIVE;
        // init_data.equipment = {1100, 1302, 1402, 0};
        // init_data.inventory = {
        //     InventorySlot({1000, 1}),   InventorySlot({1001, 1}),
        //     InventorySlot({1102, 2}),   InventorySlot({1300, 9000}),
        //     InventorySlot({1401, 454}), InventorySlot({1500, 1}),
        //     InventorySlot({1400, 1}),   InventorySlot({1301, 1}),
        //     InventorySlot({0, 0}),      InventorySlot({0, 0}),
        //     InventorySlot({0, 0}),      InventorySlot({0, 0}),
        //     InventorySlot({0, 0}),      InventorySlot({0, 0}),
        //     InventorySlot({0, 0}),      InventorySlot({0, 0})};
        // init_data.health = 50;
        // init_data.mana = 100;
        // init_data.safe_gold = 100;
        // init_data.excess_gold = 50;
        // init_data.level = 10;
        // init_data.exp = 100;
        fprintf(stderr, "race es: %i\n", init_data.race);
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
