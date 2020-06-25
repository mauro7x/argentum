#ifndef __SEND_PM_COMMAND_H__
#define __SEND_PM_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SendPMCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    SendPMCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~SendPMCommand();
};

//-----------------------------------------------------------------------------

#endif  // __SEND_PM_COMMAND_H__
