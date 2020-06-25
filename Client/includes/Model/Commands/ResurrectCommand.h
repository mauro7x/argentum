#ifndef __RESURRECT_COMMAND_H__
#define __RESURRECT_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ResurrectCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    ResurrectCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ResurrectCommand();
};

//-----------------------------------------------------------------------------

#endif  // __RESURRECT_COMMAND_H__
