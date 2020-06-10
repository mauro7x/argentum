#ifndef __CLIENTACCEPTOR_H__
#define __CLIENTACCEPTOR_H__

//-----------------------------------------------------------------------------

#include "../../../Common/includes/Socket/Socket.h"
//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClientAcceptor {
   private:
    Socket acceptor_skt;
    bool is_running;
    // atributos del clientaceptador

    // metodos privados

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: hostname, portname.
     */
    ClientAcceptor(const std::string& hostname, const std::string& port);

    /* Deshabilitamos el constructor por copia. */
    ClientAcceptor(const ClientAcceptor&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientAcceptor& operator=(const ClientAcceptor&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ClientAcceptor(ClientAcceptor&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ClientAcceptor& operator=(ClientAcceptor&& other) = delete;

    /**
     * Descripción: poner a correr al clientaceptador
     *
     * Parámetros: -
     *
     * Retorno: -
     *
     */
    void run();

    /**
     * Descripción: checkar estado del clientaceptador.
     *
     * Parámetros: -
     *
     * Retorno: true si el clientaceptador sigue corriendo, si no false.
     *
     */
    bool isRunning();


    /**
     * Descripción: destructor.
     */
    ~ClientAcceptor();
};

//-----------------------------------------------------------------------------
#endif //__CLIENTACCEPTOR_H__