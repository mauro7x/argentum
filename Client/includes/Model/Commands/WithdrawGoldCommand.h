#ifndef __WITHDRAW_GOLD_COMMAND_H__
#define __WITHDRAW_GOLD_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class WithdrawGoldCommand : public Command {
   private:
    // Atributos

   public:
    /* Constructor */
    WithdrawGoldCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~WithdrawGoldCommand();
};

//-----------------------------------------------------------------------------

#endif  // __WITHDRAW_GOLD_COMMAND_H__
