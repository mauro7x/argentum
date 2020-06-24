#ifndef __RECEIVER_H__
#define __RECEIVER_H__

//-----------------------------------------------------------------------------
#include <atomic>
#include <cstdint>
#include <exception>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/NonBlockingQueue.h"
#include "../../../Common/includes/Protocol.h"
#include "../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../Common/includes/Thread.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Broadcasts/Broadcast.h"
#include "Broadcasts/BroadcastFactory.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Receiver : public Thread {
   private:
    const SocketWrapper& socket;              /* SÓLO LECTURA (RECV) */
    NonBlockingQueue<Broadcast*>& broadcasts; /* Broadcasts a recibir */
    // falta cola de info para mostrar x chat
    std::atomic_bool& exit; /* flag de ejecución compartido */

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Recibe una respuesta y la pushea en la cola */
    void _receiveReply() const;

    /* Recibe un mensaje privado y lo pushea en la cola */
    void _receivePrivateMessage() const;

    /* Recibe un broadcast y lo pushea en la cola */
    void _receiveBroadcast() const;

    /* Recibe un broadcast de tipo new */
    void _receiveNewBroadcast() const;

    /* Recibe un broadcast de tipo update */
    void _receiveUpdateBroadcast() const;

    /* Recibe un broadcast de tipo delete */
    void _receiveDeleteBroadcast() const;

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    Receiver(const SocketWrapper& socket,
             NonBlockingQueue<Broadcast*>& broadcasts, std::atomic_bool& exit);

    /* Deshabilitamos el constructor por copia. */
    Receiver(const Receiver&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Receiver& operator=(const Receiver&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Receiver(Receiver&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Receiver& operator=(Receiver&& other) = delete;

    //-----------------------------------------------------------------------------

    /* Hilo principal de ejecución */
    void run() override;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~Receiver();
};

//-----------------------------------------------------------------------------

#endif  // __RECEIVER_H__
