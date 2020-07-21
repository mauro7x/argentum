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
    /* Constructor */
    Reply(const uint8_t message_type, const std::string& reply);

    /* Habilitamos el constructor por copia. */
    Reply(const Reply& other);

    /* Habilitamos el operador= para copia.*/
    Reply& operator=(const Reply& other);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer);

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~Reply();
};

#endif  //__REPLY_H__
