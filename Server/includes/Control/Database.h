#ifndef __DATABASE_H__
#define __DATABASE_H__

//-----------------------------------------------------------------------------
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <mutex>
#include <string>
#include <unordered_map>

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Exceptions/LoginException.h"
#include "../../../Common/includes/Protocol.h"
#include "../../../Common/includes/defs.h"
#include "../Model/Config.h"
#include "../Model/config_structs.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct PlayerInfo {
    char username[MAX_USERNAME_SIZE + 1] = {0};
    char password[MAX_PASSWORD_SIZE + 1] = {0};
    size_t index;

    PlayerInfo() : index{0} {}
};

struct DataIndex {
    char password[MAX_USERNAME_SIZE + 1] = {0};
    bool connected;
    size_t index;

    DataIndex(size_t index) : connected(false), index(index) {}
};

class Database {
   private:
    Config<RaceCfg> races;
    Config<KindCfg> kinds;
    bool initialized;
    size_t file_pointer;
    std::mutex m;
    std::unordered_map<std::string, DataIndex> data_index; /*user:data_index */

    //-------------------------------------------------------------------------
    // Métodos privados
    /* Llenar las infos de cada jugador al map */
    void _fillInfo();

    /* Obtener el dato del jugador */
    void _getPlayerData(const std::string& username,
                        CharacterCfg& character_data);

    /* Guardar info del jugador */
    void _persistPlayerInfo(const std::string& username);

    /* crear datos iniciales para el jugador nuevo */
    void _createInitialData(const std::string& username, Id race, Id kind,
                            Id head_id, Id body_id,
                            CharacterCfg& character_data);

    /* chequear si head_id es valido */
    bool _checkHeadId(Id race, Id head_id);

    /* chequear si body_id es valido */
    bool _checkBodyId(Id race, Id body_id);
    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    Database();

    /* Deshabilitamos el constructor por copia. */
    Database(const Database&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Database& operator=(const Database&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Database(Database&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Database& operator=(Database&& other) = delete;

    //-------------------------------------------------------------------------
    // Al ser un recurso compartido, su API debe ser thread-safe (C-S)

    /* Inicializa variables internas */
    void init();

    /* Intenta loggear al cliente. Devuelve 0 en caso de éxito, en caso
     * contrario devuelve el CONNECTION_ACK_TYPE correspondiente. */
    ConnectionAckType signIn(const std::string& username,
                             const std::string& password,
                             CharacterCfg& character_data);

    /* Intenta crear al personaje nuevo. Devuelve 0 en caso de éxito, en caso
     * contrario devuelve el CONNECTION_ACK_TYPE correspondiente. */
    ConnectionAckType signUp(const std::string& username,
                             const std::string& password, Id race, Id kind,
                             Id head_id, Id body_id,
                             CharacterCfg& character_data);
    //-------------------------------------------------------------------------

    /* Guardar los datos del jugador al archivo. Si el flag disconnected aparte
     * está en true, se marca como desconectado */
    void persistPlayerData(CharacterCfg& data, bool disconnected = false);

    /* Destructor */
    ~Database();
};

//-----------------------------------------------------------------------------

#endif  // __DATABASE_H__
