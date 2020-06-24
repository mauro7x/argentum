#ifndef __NOTIFICATION_REPLY_H
#define __NOTIFICATION_REPLY_H

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Notificación de reply en carga de responder las respuestas del servidor
 * respecto a los comandos recibidos*/

class NotificationReply : public Notification {
   private:
    uint8_t message_type;
    std::string reply;

   public:
    /* Constructor por defult */
    NotificationReply();

    /* Constructor */
    NotificationReply(uint8_t message_type, std::string reply);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer);

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~NotificationReply() {}
};

#endif  //__NOTIFICATION_REPLY_H
