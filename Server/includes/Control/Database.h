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
#include "../Model/config_structs.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct PlayerInfo {
    char username[8] = {0};
    char password[8] = {0};
    size_t position;
};

class Database {
   private:
    bool initialized;
    size_t file_pointer;
    std::mutex m;
    std::unordered_map<std::string, CharacterCfg> clients; /* user: data */
    std::unordered_map<std::string, size_t> data_index;    /*user:data_index */
    std::unordered_map<std::string, std::string> player_infos; /*user:pass*/

    //-------------------------------------------------------------------------
    // Métodos privados
    /* Llenar las infos de cada jugador al map */
    void _fillInfo();

    /* Obtener el dato del jugador */
    void _getPlayerData(const std::string& username);

    /* Guardar info del jugador */
    void _persistPlayerInfo(const std::string& username);

    /* crear datos iniciales para el jugador nuevo*/
    void _createDataInicial(const std::string& username, Id race, Id kind);

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
                Id race, Id kind, CharacterCfg& character_data);
    //-------------------------------------------------------------------------

    /* Guardar los datos del jugador */
    void persistPlayerData(const std::string& username);

    /* Destructor */
    ~Database();
};

//-----------------------------------------------------------------------------

#endif  // __DATABASE_H__
