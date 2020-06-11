#ifndef __CLIENTRECEIVER_H__
#define __CLIENTRECEIVER_H__

//-----------------------------------------------------------------------------

#include "../../../Common/includes/Socket/SocketWrapper.h"
#include <atomic>
//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClientReceiver{
    // atributos del clientreceiver
   private:
    SocketWrapper receiver_skt;
    std::atomic_bool keep_running;
    //una cola o dos ?
    //cola 1: respuesta
    //cola 2: abstracto update

    // metodos privados

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: hostname, portname.
     */
    ClientReceiver(const std::string& hostname, const std::string& port);

    /* Deshabilitamos el constructor por copia. */
    ClientReceiver(const ClientReceiver&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientReceiver& operator=(const ClientReceiver&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ClientReceiver(ClientReceiver&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ClientReceiver& operator=(ClientReceiver&& other) = delete;

    /**
     * Descripción: poner a correr al clientareceiver
     *
     * Parámetros: -
     *
     * Retorno: -
     *
     */
    void run();

    /**
     * Descripción: frenar el receiver.
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
    ~ClientReceiver();
};

//-----------------------------------------------------------------------------
#endif //__CLIENTRECEIVER_H__