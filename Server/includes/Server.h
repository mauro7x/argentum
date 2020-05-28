#ifndef __SERVER_H__
#define __SERVER_H__

//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Server {
   private:
    // atributos del servidor

    // metodos privados

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: puerto al que conectarse, cantidad máxima de clientes a
     * encolar antes de aceptarlos.
     */
    Server(const std::string& port, unsigned int max_clients_queued);

    /* Deshabilitamos el constructor por copia. */
    Server(const Server&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Server& operator=(const Server&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Server(Server&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Server& operator=(Server&& other) = delete;

    /**
     * Descripción: pone a correr al servidor.
     *
     * Parámetros: -
     *
     * Retorno: -
     *
     */
    void run();

    /**
     * Descripción: destructor.
     */
    ~Server();
};

//-----------------------------------------------------------------------------

#endif  // __SERVER_H__