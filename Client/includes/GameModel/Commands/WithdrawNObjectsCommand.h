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
    uint32_t x;
    uint32_t y;
    uint32_t item_id;
    uint32_t amount;

   public:
    /* Constructor */
    WithdrawNObjectsCommand(uint32_t x, uint32_t y, uint32_t item_id,
                            uint32_t amount);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~WithdrawNObjectsCommand();
};

//-----------------------------------------------------------------------------

#endif  // __WITHDRAW_N_OBJECTS_COMMAND_H__
