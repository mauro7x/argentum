#ifndef __NOTIFICATION_REPLY_H
#define __NOTIFICATION_REPLY_H

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

/* Notificación de reply en carga de responder las respuestas del servidor
 * respecto a los comandos recibidos*/

class NotificationReply: public Notification {
   public:
    /* Constructor */
    NotificationReply(char opcode, uint32_t length, std::string reply);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const SocketWrapper& peer);

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~NotificationReply() {}
};

#endif //__NOTIFICATION_REPLY_H
