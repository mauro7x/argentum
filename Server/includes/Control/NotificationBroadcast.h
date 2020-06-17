#ifndef __NOTIFICATION_BROADCAST_H
#define __NOTIFICATION_BROADCAST_H

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

/* Notificación de Broadcast en carga de enviar el broadcast del juego 
al cliente*/

class NotificationBroadcast : public Notification {
   public:
    /* Constructor */
    NotificationBroadcast(uint32_t Broadcast_length, std::string Broadcast);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const SocketWrapper& peer);

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~NotificationBroadcast() {}
};

#endif  //__NOTIFICATION_BROADCAST_H