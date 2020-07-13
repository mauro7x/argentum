#ifndef __MESSAGE_H
#define __MESSAGE_H

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "../../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "Notification.h"
//-----------------------------------------------------------------------------

/* Notificación de Message en carga de renviar la mensaje que mande el sender al
receiver corresponde*/

class Message : public Notification {
   private:
    std::string from_nickname;
    std::string message;
    MessageType message_type;
    InstanceId from_id;

   public:
    /* Constructor */
    Message(const std::string& from_nickname, const std::string& message,
            const MessageType message_type, InstanceId from_id = 0);

    /* Habilitamos el constructor por copia. */
    Message(const Message& other);

    /* Habilitamos el operador= para copia.*/
    Message& operator=(const Message& other);

    //-----------------------------------------------------------------------------

    /* Envío polimórfico de notificacion. Devuelve si se pudo enviar. */
    virtual bool send(const InstanceId sender, const SocketWrapper& peer);

    //-----------------------------------------------------------------------------

    /* Destructor */
    virtual ~Message();
};

#endif  //__MESSAGE_H
