#ifndef __LOG_IN_H__
#define __LOG_IN_H__

//-----------------------------------------------------------------------------
#include <cstdint>
#include <vector>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/DataStructs.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/NonBlockingQueue.h"
#include "../../../Common/includes/Protocol.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/json_conversion.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Broadcasts/Broadcast.h"
#include "Broadcasts/NewPlayerBroadcast.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/** LogInProxy
 * Clase que se encarga de, una vez conectados con el servidor, intercambiar
 * mensajes con el mismo para seleccionar un personaje (existente o crear uno
 * nuevo) en el juego y recibir la data inicial para poder empezar a jugar.
 */

class LogInProxy {
   private:
    SocketWrapper& socket;
    NonBlockingQueue<Broadcast*>& broadcasts;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Recibe la data inicial del jugador y la pushea como un broadcast new. */
    void _receiveFirstPackage() const;

    //-----------------------------------------------------------------------------

   public:
    /** Constructor
     * Recibe el socket conectado con el servidor (lo utilizará para lectura
     * y escritura, por lo que no se permite su uso simultaneo con otros hilos
     * de ejecución), y una cola no bloqueante donde pushear el primer paquete.
     */
    LogInProxy(SocketWrapper& socket, NonBlockingQueue<Broadcast*>& broadcasts);

    /* Deshabilitamos el constructor por copia. */
    LogInProxy(const LogInProxy&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    LogInProxy& operator=(const LogInProxy&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    LogInProxy(LogInProxy&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    LogInProxy& operator=(LogInProxy&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública

    /* Coordina la conexión del cliente al juego, ya sea mediante un usuario
     * pre-existente o mediante uno nuevo. */
    void operator()() const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~LogInProxy();
};

//-----------------------------------------------------------------------------

#endif  // __LOG_IN_H__
