#ifndef __CLIENTLOGIN_H__
#define __CLIENTLOGIN_H__

//-----------------------------------------------------------------------------
#include <atomic>

#include "LoginValidator.h"
#include "ClientList.h"
#include "../../../Common/includes/Thread.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClientLogin : public Thread {
private:
    // Atributos del objeto activo.
    SocketWrapper socket;
    LoginValidator* log_val;
    ClientList* client_list;
    std::atomic_bool is_running;

public:
    /**
     * Descripción: constuctor.
     *
     * Parámetros: -
     */
    ClientLogin(ClientList* cli_list, LoginValidator* login_val, SocketWrapper&& sock);


    /**
     * Descripción: constuctor por movimiento.
     *
     * Parámetros: -
     */
    ClientLogin(ClientLogin&& other);

    /**
     * Descripción: código que se ejecutará al correr el hilo.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    virtual void run() override;

    /**
     * Descripción: chequea si el hilo se está ejecutando.
     *
     * Parámetros: -
     *
     * Retorno: TRUE si el hilo se está ejecutando, FALSE si no.
     */
    bool isRunning();

    /**
     * Descripción: destructor.
     */
    virtual ~ClientLogin();
};

//-----------------------------------------------------------------------------
#endif  // __CLIENTLOGIN_H__
