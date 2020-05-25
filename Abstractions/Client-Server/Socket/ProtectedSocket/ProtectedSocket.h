#ifndef __PROTECTED_SOCKET_H__
#define __PROTECTED_SOCKET_H__

//-----------------------------------------------------------------------------
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>

#include "ClosedSocketException.h"
#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mensajes de error
#define SET_SERVER_ADDRESS_ERROR "Error in function: Socket::_setServerAddres()"
#define SET_CLIENT_ADDRESSES_ERROR \
    "Error in function: Socket::_setClientAddresses()"
#define SET_FD_ERROR "Error in function: Socket::_setFd()"
#define FIX_TIMEWAIT_ERROR "Error in function: Socket::_fixTimeWait()"
#define BIND_ERROR "Error in function: Socket::_bind()"
#define LISTEN_ERROR "Error in function: Socket::_listen()"
#define CLIENT_OUT_OF_ADDRESSES                                        \
    "Error in function: Socket::_tryToConnectTo(). Client ran out of " \
    "addresses to try."
#define ACCEPT_CLOSED_ERROR "Error in function: Socket::accept()"
#define SEND_ERROR "Error in function: Socket::send()"
#define RECV_ERROR "Error in function: Socket::recv()"
#define SHUTDOWN_ERROR "Error in function: Socket::shutdown()"
#define CLOSE_ERROR "Error in function: Socket::_closeFdIfValid()"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ProtectedSocket {
   private:
    int fd;
    std::atomic_bool fd_valid;
    std::mutex m;

    /** (SERVER-SIDE)
     * Descripción: obtiene la dirección donde se abrira el servidor a la
     * escucha de conexiones remotas.
     *
     * Parámetros: puerto, estructura de direccion.
     *
     * Retorno: -
     *
     * Se debe liberar la memoria reservada apuntada por address.
     * >THROW EXPLICITO DE EXCEPTION (no es necesario liberar address).
     */
    void _setServerAddress(const std::string& port, addrinfo** address);

    /** (CLIENT-SIDE)
     * Descripción: obtiene las direcciones a las que se tratará de
     * conectar el servidor.
     *
     * Parámetros: hostname, puerto, estructura de direcciones.
     *
     * Retorno: -
     *
     * Se debe liberar la memoria reservada apuntada por addresses.
     * >THROW EXPLICITO DE EXCEPTION (no es necesario liberar addresses).
     */
    void _setClientAddresses(const std::string& hostname,
                             const std::string& port, addrinfo** addresses);

    /**
     * Descripción: llama a la syscall socket() para obtener el fd.
     *
     * Parámetros: estructura de direccion para crear el socket,
     * estructura de direcciones a liberar en caso de error.
     *
     * Retorno: -
     *
     * En caso de error libera la memoria de all_addresses.
     * >THROW EXPLICITO DE EXCEPTION.
     */
    void _setFd(addrinfo* address, addrinfo* all_addresses);

    /** (SERVER-SIDE)
     * Descripción: arregla el problema de TIMEWAIT de existir.
     *
     * Parámetros: estructura de dirección.
     *
     * Retorno: -
     *
     * En caso de error libera la memoria de address.
     * >THROW EXPLICITO DE EXCEPTION.
     */
    void _fixTimeWait(addrinfo* address);

    /** (SERVER-SIDE)
     * Descripción: realiza el bind del socket a address.
     *
     * Parámetros: estructura de dirección a la que bindear el socket.
     *
     * Retorno: -
     *
     * Libera la memoria de address.
     * >THROW EXPLICITO DE EXCEPTION.
     */
    void _bind(addrinfo* address);

    /** (SERVER-SIDE)
     * Descripción: pone a la escucha al servidor. Se establece
     * @max_queued_clients como limite a la cantidad de clientes encolados
     * esperando ser aceptados.
     *
     * Parámetros: cantidad límite de clientes encolados.
     *
     * Retorno: -
     *
     * >THROW EXPLICITO DE EXCEPTION.
     */
    void _listen(const int max_queued_clients);

    /** (CLIENT-SIDE)
     * Descripción: intenta conectarse a todas las direcciones en la
     * estructura de direcciones recibida. Si no lo logra, lanza excepción.
     *
     * Parámetros: estructura de direcciones.
     *
     * Retorno: -
     *
     * Libera la memoria de addresses.
     * >THROW EXPLICITO DE EXCEPTION.
     */
    void _tryToConnectTo(addrinfo* addresses);

    /**
     * Descripción: si el fd es valido, lo cierra.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    void _closeFdIfValid();

   public:
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
    ProtectedSocket(const std::string& port, const int max_queued_clients);

    /** (SERVER-SIDE)
     * Diseñado para ser utilizado por el SERVIDOR para crear los
     * socket peer con los que se comunica con sus clientes.
     *
     * Descripción: constructor con fd.
     *
     * Parámetros: file descriptor valido correctamente inicializado.
     *
     * Se toma la responsabilidad del cierre del fd.
     */
    explicit ProtectedSocket(const int fd);

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
    ProtectedSocket(const std::string& hostname, const std::string& port);

    /* Deshabilitamos el constructor por copia. */
    ProtectedSocket(const ProtectedSocket&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ProtectedSocket& operator=(const ProtectedSocket&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ProtectedSocket(ProtectedSocket&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ProtectedSocket& operator=(ProtectedSocket&& other) = delete;

    /** (SERVER-SIDE)
     * Descripción: acepta la conexión de un cliente.
     *
     * Parámetros: -
     *
     * Retorno: file descriptor del socket aceptado.
     *
     * No toma la responsabilidad de cerrar el fd devuelto.
     * >THROW EXPLICITO DE EXCEPTION.
     */
    int accept();

    /**
     * Descripción: envia len bytes de source a través del socket.
     *
     * Parámetros: source const char*, cantidad de bytes.
     *
     * Retorno: N, si n>0: cantidad de bytes enviados.
     *             si n==0: el socket fue cerrado.
     *
     * >THROW EXPLICITO DE EXCEPTION.
     */
    ssize_t send(const char* source, const ssize_t len);

    /**
     * Descripción: recibe len bytes del socket en el buffer.
     *
     * Parámetros: buffer donde almacenar los len bytes recibidos.
     *
     * Retorno: N, si n>0: cantidad de bytes recibidos.
     *             si n==0: el socket fue cerrado.
     *
     * >THROW EXPLICITO DE EXCEPTION.
     */
    ssize_t recv(char* buffer, const ssize_t len);

    //---------------------------------------------------------------------
    // Sobrecarga de operadores >> << para envio y recepción de distintos
    // tipos de datos específicos. Aplica la misma documentación para
    // >> que para recv, y para << que para send.

    ssize_t operator<<(uint16_t n);
    ssize_t operator>>(uint16_t& n);

    ssize_t operator<<(const std::string& msg);
    // Para poder recibir un string necesitamos utilizar un protocolo

    ssize_t operator<<(char c);
    ssize_t operator>>(char& c);

    //---------------------------------------------------------------------

    /**
     * Descripción: desconecta uno o ambos canales del socket.
     *
     * Parámetros: canal/es a desconectar, si no se recibe, por defecto se
     * desconectan ambos canales (lectura y escritura).
     *
     * Retorno: -
     */
    void shutdown(const int& channel = SHUT_RDWR);

    /**
     * Descripción: cierra el socket si el mismo es válido.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    void close();

    /**
     * Descripción: destructor.
     *
     * Cierra el file descriptor asociado al socket.
     */
    ~ProtectedSocket();
};

//-----------------------------------------------------------------------------
#endif  // __PROTECTED_SOCKET_H__
