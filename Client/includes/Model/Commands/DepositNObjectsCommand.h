#ifndef __DEPOSIT_N_OBJECTS_COMMAND_H__
#define __DEPOSIT_N_OBJECTS_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class DepositNObjectsCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    DepositNObjectsCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~DepositNObjectsCommand();
};

//-----------------------------------------------------------------------------

#endif  // __DEPOSIT_N_OBJECTS_COMMAND_H__
