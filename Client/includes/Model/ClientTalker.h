#ifndef __CLIENTTALKER_H__
#define __CLIENTTALKER_H__

//-----------------------------------------------------------------------------

#include "../../../Common/includes/Socket/Socket.h"
//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ClientTalker {
   private:
    Socket talker_skt;
    bool is_running;
    // atributos del clienttalker

    // metodos privados

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: hostname, portname.
     */
    ClientTalker(const std::string& hostname, const std::string& port);

    /* Deshabilitamos el constructor por copia. */
    ClientTalker(const ClientTalker&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ClientTalker& operator=(const ClientTalker&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ClientTalker(ClientTalker&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ClientTalker& operator=(ClientTalker&& other) = delete;

    /**
     * Descripción: poner a correr al clienttalker.
     *
     * Parámetros: -
     *
     * Retorno: -
     *
     */
    void run();

    /**
     * Descripción: checkar estado del clienttalker.
     *
     * Parámetros: -
     *
     * Retorno: true si el clienttalker sigue corriendo, si no false.
     *
     */
    bool isRunning();


    /**
     * Descripción: destructor.
     */
    ~ClientTalker();
};

//-----------------------------------------------------------------------------
#endif //__CLIENTTALKER_H__