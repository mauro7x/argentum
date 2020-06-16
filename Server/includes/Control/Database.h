#ifndef __DATABASE_H__
#define __DATABASE_H__

//-----------------------------------------------------------------------------
#include <mutex>
#include <string>
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Exceptions/LoginException.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Database {
   private:
    bool initialized;
    std::mutex m;
    std::unordered_map<std::string, std::string> clients; /* user: pass */

    //-------------------------------------------------------------------------
    // Métodos privados
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
    void signIn(const std::string& username, const std::string& password);

    /* Un cliente crea un usuario nuevo (PROXY) */
    // El retorno va a tener que ser PlayerData en un futuro, por ahora proxy.
    void signUp(const std::string& username, const std::string& password);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Database();
};

//-----------------------------------------------------------------------------

#endif  // __DATABASE_H__
