#ifndef __RECEIVER_H__
#define __RECEIVER_H__

//-----------------------------------------------------------------------------
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
#include "../GameView/GameView.h"
#include "Broadcasts/Broadcast.h"
#include "Broadcasts/BroadcastFactory.h"
#include "Messages/Message.h"
#include "Messages/MessageFactory.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Receiver : public Thread {
   private:
    const SocketWrapper& socket;              /* SÓLO LECTURA (RECV) */
    NonBlockingQueue<Broadcast*>& broadcasts; /* Broadcasts a recibir */
    NonBlockingQueue<Message*>& messages;     /* Mensajes a recibir */
    GameView& game_view;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Recibe un mensaje y lo pushea en la cola */
    void _receiveMessage() const;

    /* Recibe un broadcast y lo pushea en la cola */
    void _receiveBroadcast() const;

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    Receiver(const SocketWrapper& socket,
             NonBlockingQueue<Broadcast*>& broadcasts,
             NonBlockingQueue<Message*>& messages, GameView& game_view);

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
