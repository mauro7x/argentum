#ifndef __CLIENT_LOGIN_H__
#define __CLIENT_LOGIN_H__

//-----------------------------------------------------------------------------
#include <stdio.h>  // para debug

#include <atomic>
#include <exception>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Exceptions/LoginException.h"
#include "../../../Common/includes/NonBlockingQueue.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/Thread.h"
#include "../../../Common/includes/Protocol.h"
#include "../Model/config_structs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Database.h"
#include "NewConnection.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClientLogin : public Thread {
   private:
    std::atomic_bool is_running;
    SocketWrapper peer;
    Database& database;
    NonBlockingQueue<NewConnection*>& new_connections;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Proxy para el login */
    void _loginProxy();

    /* Ejecutar el proceso de login de un jugador
     * genera un nuevo character config si es un usuario nuevo,
     * si el usuario ya esta creado, devuelvo los datos correspondes.
    */
    CharacterCfg _login();

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    ClientLogin(SocketWrapper& peer, Database& database,
                NonBlockingQueue<NewConnection*>& new_connections);

    /* Deshabilitamos el constructor por copia. */
    ClientLogin(const ClientLogin&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientLogin& operator=(const ClientLogin&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ClientLogin(ClientLogin&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ClientLogin& operator=(ClientLogin&& other) = delete;

    //-------------------------------------------------------------------------

    /* Handler para el log-in de un cliente */
    void run() override;

    /* Chequea si el hilo se sigue ejecutando */
    bool isRunning() const;

    /* Termina la conexión de manera forzosa */
    void stop();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ClientLogin();
};

//-----------------------------------------------------------------------------

#endif  // __CLIENT_LOGIN_H__
