#ifndef __NOTIFICATION_MESSAGE_H
#define __NOTIFICATION_MESSAGE_H

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

/* Notificación de Message en carga de renviar la mensaje que mande el sender al
receiver corresponde*/

class NotificationMessage : public Notification {
   public:
    /* Constructor */
    NotificationMessage(uint32_t sender_length, std::string sender,
                        uint32_t message_length, std::string message);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const SocketWrapper& peer);

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~NotificationMessage() {}
};

#endif  //__NOTIFICATION_MESSAGE_H