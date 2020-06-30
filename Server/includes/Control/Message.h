#ifndef __MESSAGE_H
#define __MESSAGE_H

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

/* Notificación de Message en carga de renviar la mensaje que mande el sender al
receiver corresponde*/

class Message : public Notification {
   private:
    const std::string from_nickname;
    const std::string message;

    MessageType message_type;

   public:
    /* Constructor */
    Message(const std::string from_nickname, const std::string message,
            const MessageType message_type);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer);

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~Message() {}
};

#endif  //__MESSAGE_H