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
#include "../Model/Config.h"
#include "../Model/config_structs.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct PlayerInfo {
    char username[NICKNAME_MAX_LENGTH] = {0};
    char password[NICKNAME_MAX_LENGTH] = {0};
    size_t index;
};

struct DataIndex {
    char password[NICKNAME_MAX_LENGTH] = {0};
    size_t index;
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

    /* crear datos iniciales para el jugador nuevo*/
    void _createDataInicial(const std::string& username, Id race, Id kind,
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

    /* Intento de un cliente para loggearse (PROXY) */
    // El retorno va a tener que ser PlayerData en un futuro, por ahora proxy.
    void signIn(const std::string& username, const std::string& password,
                CharacterCfg& character_data);

    /* Un cliente crea un usuario nuevo (PROXY) */
    // El retorno va a tener que ser PlayerData en un futuro, por ahora proxy.
    void signUp(const std::string& username, const std::string& password,
                Id race, Id kind, Id head_id, Id body_id,
                CharacterCfg& character_data);
    //-------------------------------------------------------------------------

    /* Guardar los datos del jugador al archivo*/
    void persistPlayerData(CharacterCfg& data);

    /* Destructor */
    ~Database();
};

//-----------------------------------------------------------------------------

#endif  // __DATABASE_H__
