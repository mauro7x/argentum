#include "../../includes/Control/Database.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Database::Database() : initialized(false) {}

void Database::init() {
    std::unique_lock<std::mutex> l(m);
    if (initialized) {
        throw Exception("Database already initialized.");
    }

    // cargar el archivo json
    // por ahora hacemos el proxy:
    // {
    //     clients["mauro"] = "123";
    //     clients["juan"] = "321";
    // }

    initialized = true;
}

void Database::signIn(const std::string& username, const std::string& password,
                      CharacterCfg& character_data) {
    std::unique_lock<std::mutex> l(m);
    if (!initialized) {
        throw Exception("Database not initialized.");
    }

    if (clients.count(username) == 0) {
        throw LoginException(
            "El nombre de usuario no coincide con ningún usuario en nuestro "
            "sistema.");
    }

    if (clients.at(username).password != password) {
        throw LoginException("La contraseña ingresada es incorrecta.");
    }
    character_data = clients.at(username).character_data;

    // en caso de que haya llegado aca, devolver data
    // proxy:
    fprintf(stderr, "Bienvenido, %s\n", username.c_str());

    /* chequear que el jugador no esté conectado ya (por ahora no podemos)
    throw LoginException("El usuario solicitado se encuentra conectado.");
    */
}

void Database::signUp(const std::string& username, const std::string& password,
                      Id race, Id kind, CharacterCfg& character_data) {
    std::unique_lock<std::mutex> l(m);
    if (!initialized) {
        throw Exception("Database not initialized.");
    }

    if (clients.count(username) > 0) {
        throw LoginException(
            "El nombre de usuario solicitado se encuentra en uso.");
    }

    // Crear un nuevo jugador (por ahora, proxy)
    clients[username].password = password;
    clients[username].character_data.race = race;
    clients[username].character_data.kind = kind;
    clients[username].character_data.state = ALIVE;
    clients[username].character_data.equipment = {1100, 1302, 1402, 0};
    clients[username].character_data.inventory = {
            InventorySlot({1000, 1}),   InventorySlot({1001, 1}),
            InventorySlot({1102, 2}),   InventorySlot({1300, 9000}),
            InventorySlot({1401, 454}), InventorySlot({1500, 1}),
            InventorySlot({1400, 1}),   InventorySlot({1301, 1}),
            InventorySlot({0, 0}),      InventorySlot({0, 0}),
            InventorySlot({0, 0}),      InventorySlot({0, 0}),
            InventorySlot({0, 0}),      InventorySlot({0, 0}),
            InventorySlot({0, 0}),      InventorySlot({0, 0})};
    clients[username].character_data.health = 50;
    clients[username].character_data.mana = 100;
    clients[username].character_data.safe_gold = 100;
    clients[username].character_data.excess_gold = 50;
    clients[username].character_data.level = 10;
    clients[username].character_data.exp = 100;

    character_data = clients[username].character_data;
    /* una idea: cuando creamos, al jugador le pedimos que ingrese esto:

    username:
    password:
    clase:
    raza:

    y ahi ya podemos inicializar la data y devolverla.
    */

    // en caso de que haya llegado aca, devolver data
    // proxy:
    fprintf(stderr, "Has creado tu personaje. Bienvenido, %s\n",
            username.c_str());
}

Database::~Database() {}

//-----------------------------------------------------------------------------
