#include "../../includes/Control/Database.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Database::_fillInfo() {
    std::ifstream file_info(PLAYER_INFO_FILEPATH);
    if (!file_info.is_open()) {
        return;
    }
    file_info.seekg(0, std::ios::end);
    size_t size = file_info.tellg();
    file_info.seekg(0, std::ios::beg);
    size_t size_actual = 0;
    PlayerInfo player_info;
    while (size_actual < size) {
        file_info.read(reinterpret_cast<char*>(&player_info),
                       sizeof(player_info));
        data_index.emplace(player_info.username, DataIndex());
        data_index[player_info.username].index = player_info.index;
        std::string password = player_info.password;
        std::strncpy(data_index[player_info.username].password,
                     password.c_str(), sizeof(char) * NICKNAME_MAX_LENGTH - 1);
        size_actual += sizeof(player_info);
    }
    file_info.close();
}

void Database::_persistPlayerInfo(const std::string& username) {
    PlayerInfo player_info;
    std::strncpy(player_info.username, username.c_str(),
                 sizeof(char) * NICKNAME_MAX_LENGTH - 1);
    std::string password = data_index[username].password;
    std::strncpy(player_info.password, password.c_str(),
                 sizeof(char) * NICKNAME_MAX_LENGTH - 1);
    player_info.index = file_pointer;
    std::ofstream file_info(PLAYER_INFO_FILEPATH);
    file_info.seekp(0, std::ios::end);
    file_info.write(reinterpret_cast<char*>(&player_info), sizeof(player_info));
    file_info.close();
}

void Database::_createDataInicial(const std::string& username, Id race, Id kind,
                                  Id head_id, Id body_id,
                                  CharacterCfg& character_data) {
    character_data.map = 0;
    character_data.x_tile = 0;
    character_data.y_tile = 0;
    std::strncpy(character_data.nickname, username.c_str(),
                 sizeof(char) * NICKNAME_MAX_LENGTH - 1);
    character_data.race = race;
    character_data.kind = kind;
    character_data.head_id = head_id;
    character_data.body_id = body_id;
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
    character_data.mana = races[race].initial_health + kinds[kind].initial_mana;
    character_data.safe_gold = 0;
    character_data.excess_gold = 0;
    character_data.level = 1;
    character_data.exp = 0;
    character_data.new_created = true;
}

void Database::_getPlayerData(const std::string& username,
                              CharacterCfg& character_data) {
    size_t position = data_index[username].index;
    std::ifstream file_data(PLAYER_DATA_FILEPATH);
    file_data.seekg(position, std::ios::beg);
    file_data.read(reinterpret_cast<char*>(&character_data),
                   sizeof(character_data));
}

bool Database::_checkHeadId(Id race, Id head_id) {
    bool valid = false;
    for (size_t i = 0; i < races[race].head_ids.size(); i++) {
        if (head_id == races[race].head_ids[i]) {
            valid = true;
        }
    }
    return valid;
}

bool Database::_checkBodyId(Id race, Id body_id) {
    bool valid = false;
    for (size_t i = 0; i < races[race].body_ids.size(); i++) {
        if (body_id == races[race].body_ids[i]) {
            valid = true;
        }
    }
    return valid;
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
    if (file_data.is_open()) {
        file_data.seekg(0, std::ios::end);
        file_pointer = file_data.tellg();
        file_data.close();
    }
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
                      Id race, Id kind, Id head_id, Id body_id,
                      CharacterCfg& character_data) {
    std::unique_lock<std::mutex> l(m);
    if (!initialized) {
        throw Exception("Database not initialized.");
    }

    if (data_index.count(username) > 0) {
        throw LoginException(
            "El nombre de usuario solicitado se encuentra en uso.");
    }
    // check if race o kind is valid
    if (races.count(race) == 0 || kinds.count(kind) == 0) {
        throw Exception("Id de race o/y de kind invalido/s.");
    }

    if (!_checkHeadId(race, head_id) || !_checkBodyId(race, body_id)) {
        throw Exception("Id de head o/y de body invalido/s.");
    }

    data_index.emplace(username, DataIndex());
    data_index[username].index = file_pointer;
    std::strncpy(data_index[username].password, password.c_str(),
                 sizeof(char) * NICKNAME_MAX_LENGTH - 1);
    _persistPlayerInfo(username);
    // Crear un nuevo jugador (por ahora, proxy)
    _createDataInicial(username, race, kind, head_id, body_id, character_data);

    persistPlayerData(character_data);
    file_pointer += sizeof(character_data);

    // en caso de que haya llegado aca, devolver data
    // proxy:
    fprintf(stderr, "Has creado tu personaje. Bienvenido, %s\n",
            username.c_str());
}
void Database::persistPlayerData(CharacterCfg& data) {
    std::ofstream file_data(PLAYER_DATA_FILEPATH);
    file_data.seekp(data_index[data.nickname].index, std::ios::beg);
    file_data.write(reinterpret_cast<char*>(&data), sizeof(data));
    file_data.close();
}

Database::~Database() {}

//-----------------------------------------------------------------------------
