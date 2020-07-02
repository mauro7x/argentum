#ifndef __REPLY_H__
#define __REPLY_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Notificación de reply en carga de responder las respuestas del servidor
 * respecto a los comandos recibidos*/

class Reply : public Notification {
   private:
    uint8_t message_type;
    std::string reply;

   public:
    /* Constructor por defult */
    Reply();

    /* Constructor */
    Reply(uint8_t message_type, std::string reply);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer);

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~Reply() {}
};

#endif  //__REPLY_H__
