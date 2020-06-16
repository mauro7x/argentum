#ifndef __ACCEPTER_H__
#define __ACCEPTER_H__

//-----------------------------------------------------------------------------
#include <atomic>
#include <exception>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/ClosedSocketException.h"
#include "../../../Common/includes/Queue.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/Thread.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ClientLogin.h"
#include "Database.h"
#include "NewConnection.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Accepter : public Thread {
   private:
    SocketWrapper socket;
    Database& database;
    Queue<NewConnection*>& new_connections;
    std::atomic_bool keep_accepting;
    std::vector<ClientLogin*> client_logins;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Acepta una conexión entrante */
    void _acceptClient();

    /* Joinea y libera los ClientLogin finalizados */
    void _joinFinishedLogins();

    /* Joinea los ClientLogin forzosamente (hayan o no terminado) */
    void _joinLogins();

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    Accepter(const std::string& port, const int max_clients_queued,
             Database& database, Queue<NewConnection*>& new_connections);

    /* Deshabilitamos el constructor por copia. */
    Accepter(const Accepter&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Accepter& operator=(const Accepter&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Accepter(Accepter&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Accepter& operator=(Accepter&& other) = delete;

    //-------------------------------------------------------------------------

    /* Hilo principal de ejecución */
    void run() override;

    /* Deja de aceptar clientes */
    void stop();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Accepter();
};

//-----------------------------------------------------------------------------

#endif  // __ACCEPTER_H__
