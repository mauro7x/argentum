#ifndef __CLIENT_H__
#define __CLIENT_H__

//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Client {
   private:
    // atributos del cliente

    // metodos privados

   public:
    /**
     * Descripción: constructor.
     *
     * Parámetros: hostname y puerto al que conectarse.
     */
    Client(const std::string& hostname, const std::string& port);

    /* Deshabilitamos el constructor por copia. */
    Client(const Client&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Client& operator=(const Client&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Client(Client&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Client& operator=(Client&& other) = delete;

    /**
     * Descripción: poner a correr al cliente.
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
    ~Client();
};

//-----------------------------------------------------------------------------

#endif  // __CLIENT_H__
