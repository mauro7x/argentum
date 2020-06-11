#ifndef __CLIENTSENDER_H__
#define __CLIENTSENDER_H__

//-----------------------------------------------------------------------------

#include <queue>
#include <atomic>
#include "../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClientSender {
    // atributos del clientSender
   private:
    SocketWrapper sender_skt;
    std::atomic_bool keep_running;
    std::queue<std::string> queue_commands;
    

    // metodos privados

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: hostname, portname.
     */
    ClientSender(const std::string& hostname, const std::string& port,
                 std::queue<std::string>& queue);

    /* Deshabilitamos el constructor por copia. */
    ClientSender(const ClientSender&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientSender& operator=(const ClientSender&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ClientSender(ClientSender&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ClientSender& operator=(ClientSender&& other) = delete;

    /**
     * Descripción: poner a correr al clientSender.
     *
     * Parámetros: -
     *
     * Retorno: -
     *
     */
    void run();

    /**
     * Descripción: frenar el sender.
     *
     * Parámetros: -
     *
     * Retorno: -
     *
     */
    void stopRunning();

    /**
     * Descripción: destructor.
     */
    ~ClientSender();
};

//-----------------------------------------------------------------------------
#endif  //__CLIENTSENDER_H__