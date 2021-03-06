#ifndef __SOCKET_WRAPPER_H__
#define __SOCKET_WRAPPER_H__

//-----------------------------------------------------------------------------
#include <cstdint>
#include <string>
#include <vector>

#include "../Exceptions/ClosedSocketException.h"
#include "../Exceptions/Exception.h"
#include "Socket.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SocketWrapper : public Socket {
   private:
    /**
     * Diseñado para crear las instancias de socket peer.
     *
     * Descripción: constructor con fd.
     *
     * Parámetros: file descriptor valido correctamente inicializado.
     *
     */
    explicit SocketWrapper(const int fd);

   public:
    /** Constructor por defecto.
     * Diseñado para permitir su posterior inicialización utilizando move
     * semantics.
     */
    SocketWrapper();

    /** (SERVER-SIDE)
     * Diseñado para ser utilizado por el SERVIDOR para ponerse a la
     * escucha de conexiones entrantes.
     *
     * Descripción: constructor con un puerto y la cantidad de clientes
     * que pueden haber simultaneamente esperando a ser aceptados.
     *
     * Parámetros: puerto , máxima cantidad de clientes encolados.
     *
     * >THROW EXPLICITO DE EXCEPTION.
     */
    SocketWrapper(const std::string& port, const int max_queued_clients);

    /** (CLIENT-SIDE)
     * Diseñado para ser utilizado por el CLIENTE para conectarse
     * a un determinado servidor.
     *
     * Descripción: constructor con hostname y puerto.
     *
     * Parámetros: nombre del host y puerto al que conectarse.
     *
     * >THROW EXPLICITO DE EXCEPTION.
     */
    SocketWrapper(const std::string& hostname, const std::string& port);

    /* Deshabilitamos el constructor por copia. */
    SocketWrapper(const SocketWrapper&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    SocketWrapper& operator=(const SocketWrapper&) = delete;

    /* Habilitamos el constructor por movimiento. */
    SocketWrapper(SocketWrapper&& other);

    /* Habilitamos el operador= para movimiento. */
    SocketWrapper& operator=(SocketWrapper&& other);

    /** (SERVER-SIDE)
     * Descripción: acepta la conexión de un cliente.
     *
     * Parámetros: -
     *
     * Retorno: socket peer por movimiento.
     *
     * >THROW EXPLICITO DE EXCEPTION.
     */
    SocketWrapper accept() const;

    //---------------------------------------------------------------------
    // Sobrecarga de operadores >> << para envio y recepción de distintos
    // tipos de datos específicos. Aplica la misma documentación para
    // >> que para recv, y para << que para send.

    size_t operator<<(char c) const;
    size_t operator>>(char& c) const;

    size_t operator<<(const uint8_t& n) const;
    size_t operator>>(uint8_t& n) const;

    size_t operator<<(const uint32_t& n) const;
    size_t operator>>(uint32_t& n) const;

    size_t operator<<(const std::string& s) const;
    size_t operator>>(std::string& s) const;

    size_t operator<<(const std::vector<uint8_t>& v) const;
    size_t operator>>(std::vector<uint8_t>& v) const;

    //---------------------------------------------------------------------

    /**
     * Descripción: destructor.
     */
    ~SocketWrapper();
};

//-----------------------------------------------------------------------------

#endif  // __SOCKET_WRAPPER_H__
