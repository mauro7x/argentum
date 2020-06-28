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
        data_index.emplace(player_info.username, DataIndex());
        data_index[player_info.username].index = player_info.index;
        std::string password = player_info.password;
        std::strncpy(data_index[player_info.username].password,
                     password.c_str(), sizeof(char) * NICKNAME_MAX_LENGTH - 1);
        // fprintf(stderr,"usuario es : %s\n",player_info.username);
        // fprintf(stderr,"contrasenia es : %s\n", player_info.password);
        // fprintf(stderr,"file_pointer es : %ld\n",file_pointer);
        size_actual += sizeof(player_info);
    }
    fprintf(stderr, "termina la escritura\n");
    file_info.close();
}

void Database::_persistPlayerInfo(const std::string& username) {
    fprintf(stderr, "la posicion es : %ld\n", data_index[username].index);
    PlayerInfo player_info;
    std::strncpy(player_info.username, username.c_str(),
                 sizeof(char) * NICKNAME_MAX_LENGTH - 1);
    std::string password = data_index[username].password;
    std::strncpy(player_info.password, password.c_str(),
                 sizeof(char) * NICKNAME_MAX_LENGTH - 1);
    player_info.index = file_pointer;
    fprintf(stderr, "usuario es : %s\n", player_info.username);
    fprintf(stderr, "contrasenia es : %s\n", player_info.password);
    fprintf(stderr, "file_pointer es : %ld\n", player_info.index);
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
    fprintf(stderr, "file_pointer es : %ld\n", player_.index);
    file_info.close();
}

void Database::_createDataInicial(const std::string& username, Id race,
                                  Id kind, CharacterCfg& character_data) {
    character_data.map = 0;
    character_data.x_tile = 0;
    character_data.y_tile = 0;
    std::strncpy(character_data.nickname, username.c_str(),
                 sizeof(char) * NICKNAME_MAX_LENGTH - 1);
    character_data.race = race;
    character_data.kind = kind;
    character_data.state = ALIVE;
    character_data.equipment = {0, 0, 0, 0};
    character_data.inventory = {
        InventorySlot({0, 0}), InventorySlot({0, 0}), InventorySlot({0, 0}),
        InventorySlot({0, 0}), InventorySlot({0, 0}), InventorySlot({0, 0}),
        InventorySlot({0, 0}), InventorySlot({0, 0}), InventorySlot({0, 0}),
        InventorySlot({0, 0}), InventorySlot({0, 0}), InventorySlot({0, 0}),
        InventorySlot({0, 0}), InventorySlot({0, 0}), InventorySlot({0, 0}),
        InventorySlot({0, 0})};
    character_data.health =
        races[race].initial_health + kinds[kind].initial_health;
    character_data.mana =
        races[race].initial_health + kinds[kind].initial_mana;
    character_data.safe_gold = 0;
    character_data.excess_gold = 0;
    character_data.level = 1;
    character_data.exp = 0;
    character_data.new_created = true;
}

void Database::_getPlayerData(const std::string& username,
                              CharacterCfg& character_data) {
    size_t position = data_index[username].index;
    fprintf(stderr, "position es : %ld\n", position);
    std::ifstream file_data(PLAYER_DATA_FILEPATH);
    file_data.seekg(position, std::ios::beg);
    file_data.read(reinterpret_cast<char*>(&character_data),
                   sizeof(character_data));
    fprintf(stderr, "race es : %d\n", character_data.race);
    fprintf(stderr, "id es : %d\n", character_data.kind);
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

    if (data_index.count(username) == 0) {
        throw LoginException(
            "El nombre de usuario no coincide con ningún usuario en nuestro "
            "sistema.");
    }

    if (data_index.at(username).password != password) {
        throw LoginException("La contraseña ingresada es incorrecta.");
    }

    _getPlayerData(username, character_data);

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

    if (data_index.count(username) > 0) {
        throw LoginException(
            "El nombre de usuario solicitado se encuentra en uso.");
    }
    data_index.emplace(username, DataIndex());
    data_index[username].index = file_pointer;
    std::strncpy(data_index[username].password, password.c_str(),
                 sizeof(char) * NICKNAME_MAX_LENGTH - 1);
    _persistPlayerInfo(username);
    // Crear un nuevo jugador (por ahora, proxy)
    _createDataInicial(username, race, kind, character_data);

    persistPlayerData(character_data);
    file_pointer += sizeof(character_data);

    // en caso de que haya llegado aca, devolver data
    // proxy:
    fprintf(stderr, "Has creado tu personaje. Bienvenido, %s\n",
            username.c_str());
}
void Database::persistPlayerData(CharacterCfg& data) {
    fprintf(stderr, "antes de guardar\n");
    fprintf(stderr, "race: %d\n", data.race);
    fprintf(stderr, "exp: %d\n", data.exp);
    std::ofstream file_data(PLAYER_DATA_FILEPATH);
    file_data.seekp(data_index[data.nickname].index, std::ios::beg);
    file_data.write(reinterpret_cast<char*>(&data), sizeof(data));
    // CharacterCfg character;
    // file_data.seekg(data_index[data.nickname].index, std::ios::beg);
    // file_data.read(reinterpret_cast<char*>(&character), sizeof(character));
    // fprintf(stderr, "despues de guardar\n");
    // fprintf(stderr, "race: %d\n", character.race);
    // fprintf(stderr, "exp: %d\n", character.exp);
}

Database::~Database() {}

//-----------------------------------------------------------------------------
