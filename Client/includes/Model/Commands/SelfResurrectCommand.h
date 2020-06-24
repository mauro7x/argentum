#ifndef __SELF_RESURRECT_COMMAND_H__
#define __SELF_RESURRECT_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SelfResurrectCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    SelfResurrectCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~SelfResurrectCommand();
};

//-----------------------------------------------------------------------------

#endif  // __SELF_RESURRECT_COMMAND_H__
