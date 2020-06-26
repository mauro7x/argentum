#include "../../includes/Control/Database.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Database::_fillInfo() {
    std::ifstream file_info(PLAYER_INFO_FILEPATH);
    file_info.seekg(0, std::ios::end);
    size_t size = file_info.tellg();
    file_info.seekg(0, std::ios::beg);
    size_t size_actual = 0;
    PlayerInfo player_info;
    while (size_actual < size) {
        fprintf(stderr, "empieza la escritura\n");
        file_info.read(reinterpret_cast<char*>(&player_info),
                       sizeof(player_info));
        data_index[player_info.username] = player_info.position;
        player_infos[player_info.username] = player_info.password;
        // fprintf(stderr,"usuario es : %s\n",player_info.username);
        // fprintf(stderr,"contrasenia es : %s\n", player_info.password);
        // fprintf(stderr,"file_pointer es : %ld\n",file_pointer);
        size_actual += sizeof(player_info);
    }
    fprintf(stderr, "termina la escritura\n");
    file_info.close();
}

void Database::_persistPlayerInfo(const std::string& username) {
    fprintf(stderr, "la posicion es : %ld\n", data_index[username]);
    PlayerInfo player_info;
    std::strncpy(player_info.username, username.c_str(),
                 sizeof(player_info.username) - 1);
    std::strncpy(player_info.password, player_infos[username].c_str(),
                 sizeof(player_info.password) - 1);
    player_info.position = file_pointer;
    fprintf(stderr, "usuario es : %s\n", player_info.username);
    fprintf(stderr, "contrasenia es : %s\n", player_info.password);
    fprintf(stderr, "file_pointer es : %ld\n", player_info.position);
    std::fstream file_info(PLAYER_INFO_FILEPATH);
    file_info.seekg(0, std::ios::end);
    fprintf(stderr, "antes de write\n");
    file_info.write(reinterpret_cast<char*>(&player_info), sizeof(player_info));
    fprintf(stderr, "despues de write\n");
    PlayerInfo player_;
    file_info.seekp(0, std::ios::beg);
    file_info.read(reinterpret_cast<char*>(&player_), sizeof(player_));
    fprintf(stderr, "despues de guardar\n");
    fprintf(stderr, "usuario es : %s\n", player_.username);
    fprintf(stderr, "contrasenia es : %s\n", player_.password);
    fprintf(stderr, "file_pointer es : %ld\n", player_.position);
    file_info.close();
}

void Database::_createDataInicial(const std::string& username, Id race, Id kind) {
    clients.emplace(username, CharacterCfg());
    clients[username].race = race;
    clients[username].kind = kind;
    clients[username].state = ALIVE;
    clients[username].equipment = {1100, 1302, 1402, 0};
    clients[username].inventory = {
        InventorySlot({1000, 1}),   InventorySlot({1001, 1}),
        InventorySlot({1102, 2}),   InventorySlot({1300, 9000}),
        InventorySlot({1401, 454}), InventorySlot({1500, 1}),
        InventorySlot({1400, 1}),   InventorySlot({1301, 1}),
        InventorySlot({0, 0}),      InventorySlot({0, 0}),
        InventorySlot({0, 0}),      InventorySlot({0, 0}),
        InventorySlot({0, 0}),      InventorySlot({0, 0}),
        InventorySlot({0, 0}),      InventorySlot({0, 0})};
    clients[username].health = 50;
    clients[username].mana = 100;
    clients[username].safe_gold = 100;
    clients[username].excess_gold = 50;
    clients[username].level = 10;
    clients[username].exp = 100;
}

void Database::_getPlayerData(const std::string& username){
    clients.emplace(username, CharacterCfg());
    size_t position = data_index[username];
    fprintf(stderr, "position es : %ld\n", position);
    std::ifstream file_data(PLAYER_DATA_FILEPATH);
    file_data.seekg(position, std::ios::beg);
    file_data.read(reinterpret_cast<char*>(&clients[username]),
                   sizeof(clients[username]));
    fprintf(stderr, "race es : %d\n", clients[username].race);
    fprintf(stderr, "id es : %d\n", clients[username].kind);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Database::Database() : initialized(false), file_pointer(0) {}

void Database::init() {
    std::unique_lock<std::mutex> l(m);
    if (initialized) {
        throw Exception("Database already initialized.");
    }

    std::ifstream file_data(PLAYER_DATA_FILEPATH);
    file_data.seekg(0, std::ios::end);
    file_pointer = file_data.tellg();
    file_data.close();
    _fillInfo();

    initialized = true;
}

void Database::signIn(const std::string& username, const std::string& password,
                      CharacterCfg& character_data) {
    std::unique_lock<std::mutex> l(m);
    if (!initialized) {
        throw Exception("Database not initialized.");
    }

    if (player_infos.count(username) == 0) {
        throw LoginException(
            "El nombre de usuario no coincide con ningún usuario en nuestro "
            "sistema.");
    }

    if (player_infos.at(username) != password) {
        throw LoginException("La contraseña ingresada es incorrecta.");
    }
    _getPlayerData(username);
    character_data = clients[username];

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

    if (player_infos.count(username) > 0) {
        throw LoginException(
            "El nombre de usuario solicitado se encuentra en uso.");
    }
    data_index[username] = file_pointer;
    player_infos[username] = password;
    _persistPlayerInfo(username);
    // Crear un nuevo jugador (por ahora, proxy)
    _createDataInicial(username, race, kind);
    
    persistPlayerData(username);
    character_data = clients[username];
    file_pointer += sizeof(clients[username]);

    // en caso de que haya llegado aca, devolver data
    // proxy:
    fprintf(stderr, "Has creado tu personaje. Bienvenido, %s\n",
            username.c_str());
}
void Database::persistPlayerData(const std::string& username){
    fprintf(stderr, "antes de guardar\n");
    fprintf(stderr, "race: %d\n", clients[username].race);
    fprintf(stderr, "exp: %d\n", clients[username].exp);
    std::fstream file_data(PLAYER_DATA_FILEPATH);
    file_data.seekg(0, std::ios::end);
    file_data.write(reinterpret_cast<char*>(&clients[username]),
                    sizeof(clients[username]));
    CharacterCfg character;
    file_data.seekg(data_index[username], std::ios::beg);
    file_data.read(reinterpret_cast<char*>(&character), sizeof(character));
    fprintf(stderr, "despues de guardar\n");
    fprintf(stderr, "race: %d\n", character.race);
    fprintf(stderr, "exp: %d\n", character.exp);
}

Database::~Database() {}

//-----------------------------------------------------------------------------
