#ifndef __GRAB_ITEM_COMMAND_H__
#define __GRAB_ITEM_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class GrabItemCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    GrabItemCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~GrabItemCommand();
};

//-----------------------------------------------------------------------------

#endif  // __GRAB_ITEM_COMMAND_H__
