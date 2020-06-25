#ifndef __WITHDRAW_N_OBJECTS_COMMAND_H__
#define __WITHDRAW_N_OBJECTS_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class WithdrawNObjectsCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    WithdrawNObjectsCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~WithdrawNObjectsCommand();
};

//-----------------------------------------------------------------------------

#endif  // __WITHDRAW_N_OBJECTS_COMMAND_H__
