#ifndef __SEND_GENERAL_MESSAGE_COMMAND_H__
#define __SEND_GENERAL_MESSAGE_COMMAND_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SendGeneralMessageCommand : public Command {
   private:
    std::string content;

   public:
    /* Constructor */
    SendGeneralMessageCommand(const std::string& content);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~SendGeneralMessageCommand();
};

//-----------------------------------------------------------------------------

#endif  // __SEND_GENERAL_MESSAGE_COMMAND_H__
