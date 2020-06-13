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
     * Descripci�n: constuctor.
     *
     * Par�metros: -
     */
    ClientLogin(ClientList* cli_list, LoginValidator* login_val, SocketWrapper&& sock);


    /**
     * Descripci�n: constuctor por movimiento.
     *
     * Par�metros: -
     */
    ClientLogin(ClientLogin&& other);

    /**
     * Descripci�n: c�digo que se ejecutar� al correr el hilo.
     *
     * Par�metros: -
     *
     * Retorno: -
     */
    virtual void run() override;

    /**
     * Descripci�n: chequea si el hilo se est� ejecutando.
     *
     * Par�metros: -
     *
     * Retorno: TRUE si el hilo se est� ejecutando, FALSE si no.
     */
    bool isRunning();

    /**
     * Descripci�n: destructor.
     */
    virtual ~ClientLogin();
};

//-----------------------------------------------------------------------------
#endif  // __CLIENTLOGIN_H__
