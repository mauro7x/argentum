#ifndef __SOCKET_H__
#define __SOCKET_H__

//-----------------------------------------------------------------------------
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <string>

#include "ClosedSocketException.h"
#include "Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Socket {
   protected:
    int fd;
    bool fd_valid;

    /**
     * Diseñado para crear las instancias de socket peer.
     *
     * Descripción: constructor con fd.
     *
     * Parámetros: file descriptor valido correctamente inicializado.
     *
     */
    explicit Socket(const int fd);

   private:
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
    void _setServerAddress(const std::string& port, addrinfo** address) const;

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
                             const std::string& port,
                             addrinfo** addresses) const;

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
    void _fixTimeWait(addrinfo* address) const;

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
    void _bind(addrinfo* address) const;

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
    void _listen(const int max_queued_clients) const;

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
    Socket(const std::string& port, const int max_queued_clients);

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
    Socket(const std::string& hostname, const std::string& port);

    /**
     * Deshabilitamos el constructor por copia y su operador.
     */
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    /**
     * Habilitamos el constructor por movimiento y su operador.
     */
    Socket(Socket&& other);
    Socket& operator=(Socket&& other);

    /** (SERVER-SIDE)
     * Descripción: acepta la conexión de un cliente.
     *
     * Parámetros: -
     *
     * Retorno: socket peer por movimiento.
     *
     * >THROW EXPLICITO DE EXCEPTION.
     */
    Socket accept() const;

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
    ssize_t send(const char* source, const ssize_t len) const;

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
    ssize_t recv(char* buffer, const ssize_t len) const;

    /**
     * Descripción: desconecta uno o ambos canales del socket.
     *
     * Parámetros: canal/es a desconectar, si no se recibe, por defecto se
     * desconectan ambos canales (lectura y escritura).
     *
     * Retorno: -
     */
    void shutdown(const int& channel = SHUT_RDWR) const;

    /**
     * Descripción: cierra el socket si el mismo es válido.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    void close();

    /**
     * Descripción: destructor. Si el socket es válido lo cierra.
     */
    virtual ~Socket();
};

//-----------------------------------------------------------------------------
#endif  // __SOCKET_H__
