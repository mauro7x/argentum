#include "../../includes/Control/Database.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Database::_fillInfo() {
    std::ifstream file_info(PLAYER_INFO_FILEPATH, std::fstream::binary);
    if (!file_info.is_open()) {
        throw Exception("Database::_fillInfo: error opening info file.");
    }

    // Calculamos el size de lectura
    file_info.seekg(0, std::fstream::end);
    size_t size = file_info.tellg();

    // Volvemos al inicio
    file_info.seekg(0, std::fstream::beg);
    size_t size_actual = 0;
    PlayerInfo player_info;
    while (size_actual < size) {
        file_info.read(reinterpret_cast<char*>(&player_info),
                       sizeof(player_info));

        data_index.emplace(std::piecewise_construct,
                           std::forward_as_tuple(player_info.username),
                           std::forward_as_tuple(player_info.index));

        DataIndex& new_data_idx = data_index.at(player_info.username);
        std::strncpy(new_data_idx.password, player_info.password,
                     sizeof(new_data_idx.password) - 1);

        size_actual += sizeof(player_info);
    }
    file_info.close();
}

void Database::_persistPlayerInfo(const std::string& username) {
    PlayerInfo player_info;

    // Llenamos los campos
    std::strncpy(player_info.username, username.c_str(),
                 sizeof(player_info.username) - 1);
    std::strncpy(player_info.password, data_index.at(username).password,
                 sizeof(player_info.password) - 1);
    player_info.index = file_pointer;

    // Lo guardamos al final
    std::ofstream file_info(PLAYER_INFO_FILEPATH,
                            std::fstream::app | std::fstream::binary);
    file_info.write(reinterpret_cast<char*>(&player_info), sizeof(player_info));
    file_info.close();
}

void Database::_createInitialData(const std::string& username, Id race, Id kind,
                                  Id head_id, Id body_id,
                                  CharacterCfg& character_data) {
    character_data.map = 0;
    character_data.x_tile = 0;
    character_data.y_tile = 0;
    std::strncpy(character_data.nickname, username.c_str(),
                 sizeof(character_data.nickname) - 1);
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
    character_data.mana =
        (kinds[kind].initial_mana)
            ? races[race].initial_mana + kinds[kind].initial_mana
            : 0;
    character_data.safe_gold = 0;
    character_data.excess_gold = 0;
    character_data.level = 1;
    character_data.exp = 0;
    character_data.new_created = true;
}

void Database::_getPlayerData(const std::string& username,
                              CharacterCfg& character_data) {
    std::streampos position = data_index.at(username).index;
    std::ifstream file_data(PLAYER_DATA_FILEPATH, std::fstream::binary);
    file_data.seekg(position);
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

    // En caso de que los archivos no existan, los creamos
    {
        // Archivo de índices
        std::fstream file_info(PLAYER_INFO_FILEPATH,
                               std::fstream::in | std::fstream::out |
                                   std::fstream::app | std::fstream::binary);
        file_info.seekg(0, std::fstream::end);
        size_t size = file_info.tellg();
        // Chequeamos que sea multiplo del size que necesitamos
        if ((size % sizeof(PlayerInfo))) {
            fprintf(stderr, "Debug (info_data) | size: %lu, sizeof: %lu\n",
                    size, sizeof(PlayerInfo));
            throw Exception("Database::_fillInfo: invalid index file size.");
        }
        file_info.close();

        // Archivo de data (actualizamos nuestro file pointer)
        std::fstream file_data(PLAYER_DATA_FILEPATH,
                               std::fstream::in | std::fstream::out |
                                   std::fstream::app | std::fstream::binary);
        file_data.seekg(0, std::fstream::end);
        file_pointer = file_data.tellg();
        // Chequeamos que sea multiplo del size que necesitamos
        if ((file_pointer % sizeof(CharacterCfg))) {
            fprintf(stderr, "Debug (info_data) | fp: %lu, sizeof: %lu\n",
                    file_pointer, sizeof(CharacterCfg));
            throw Exception("Database::_fillInfo: invalid data file size.");
        }
        file_data.close();
    }

    // Llenamos la data del archivo de info
    _fillInfo();

    initialized = true;
}

ConnectionAckType Database::signIn(const std::string& username,
                                   const std::string& password,
                                   CharacterCfg& character_data) {
    std::unique_lock<std::mutex> l(m);
    if (!initialized) {
        throw Exception("Database:: not initialized.");
    }

    if (data_index.count(username) == 0) {
        return ERROR_INVALID_USERNAME_ACK;
    }

    if (data_index.at(username).password != password) {
        return ERROR_INVALID_PASSWORD_ACK;
    }

    if (data_index.at(username).connected) {
        return ERROR_USERNAME_CONNECTED_ACK;
    }

    _getPlayerData(username, character_data);

    // Lo marcamos como conectado
    data_index.at(username).connected = true;

    return SUCCESS_ACK;
}

ConnectionAckType Database::signUp(const std::string& username,
                                   const std::string& password, Id race,
                                   Id kind, Id head_id, Id body_id,
                                   CharacterCfg& character_data) {
    std::unique_lock<std::mutex> l(m);
    if (!initialized) {
        throw Exception("Database:: not initialized.");
    }

    if (data_index.count(username) > 0) {
        return ERROR_USERNAME_TAKEN_ACK;
    }

    // check if race o kind is valid
    if (races.count(race) == 0 || kinds.count(kind) == 0) {
        throw Exception("Database::signUp: invalid race/kind id received.");
    }

    if (!_checkHeadId(race, head_id) || !_checkBodyId(race, body_id)) {
        throw Exception("Database::signUp: invalid head/body id received.");
    }

    // Lo construimos
    data_index.emplace(std::piecewise_construct,
                       std::forward_as_tuple(username),
                       std::forward_as_tuple(file_pointer));
    DataIndex& new_data = data_index.at(username);

    std::strncpy(new_data.password, password.c_str(),
                 sizeof(new_data.password) - 1);

    _persistPlayerInfo(username);
    _createInitialData(username, race, kind, head_id, body_id, character_data);
    persistPlayerData(character_data);
    file_pointer += sizeof(CharacterCfg);

    return SUCCESS_ACK;
}

void Database::persistPlayerData(CharacterCfg& data, bool disconnect) {
    std::fstream file_data(PLAYER_DATA_FILEPATH, std::fstream::in |
                                                     std::fstream::out |
                                                     std::fstream::binary);
    std::streampos idx = data_index.at(data.nickname).index;
    file_data.seekp(idx, std::fstream::beg);
    file_data.write(reinterpret_cast<char*>(&data), sizeof(data));
    file_data.close();

    if (disconnect) {
        data_index.at(data.nickname).connected = false;
    }
}

Database::~Database() {}

//-----------------------------------------------------------------------------
