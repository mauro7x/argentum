#ifndef __DEPOSIT_GOLD_COMMAND_H__
#define __DEPOSIT_GOLD_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class DepositGoldCommand : public Command {
   private:
    uint32_t x;
    uint32_t y;
    uint32_t amount;

   public:
    /* Constructor */
    DepositGoldCommand(uint32_t x, uint32_t y, uint32_t amount);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~DepositGoldCommand();
};

//-----------------------------------------------------------------------------

#endif  // __DEPOSIT_GOLD_COMMAND_H__
