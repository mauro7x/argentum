#ifndef __ACCEPTER_H__
#define __ACCEPTER_H__

//-----------------------------------------------------------------------------
#include <atomic>

#include "ClosedSocketException.h"
#include "Exception.h"
#include "Socket.h"
#include "Thread.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Accepter : public Thread {
   private:
    // Atributos del objeto activo.
    Socket socket;
    std::atomic_bool keep_listening;
    std::atomic_bool is_running;

   public:
    /**
     * Descripción: constuctor.
     *
     * Parámetros: -
     */
    Accepter(const std::string& port, unsigned int max_clients_queued);

    /**
     * Descripción: código que se ejecutará al correr el hilo.
     *
     * Parámetros: -
     *
     * Retorno: -
     */
    virtual void run() override;

    /**
     * Descripción: chequea si el hilo se está ejecutando.
     *
     * Parámetros: -
     *
     * Retorno: TRUE si el hilo se está ejecutando, FALSE si no.
     */
    bool isRunning();

    /**
     * Descripcion: realiza el shutdown y el posterior close del socket
     * para dejar de recibir clientes.
     *
     * Parametros: -
     *
     * Retorno: -
     */
    void close();

    /**
     * Descripción: destructor.
     */
    virtual ~Accepter();
};

//-----------------------------------------------------------------------------
#endif  // __ACCEPTER_H__
