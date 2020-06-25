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
    uint32_t x;
    uint32_t y;
    uint8_t inventory_slot;
    uint32_t amount;

   public:
    /* Constructor */
    DepositNObjectsCommand(uint32_t x, uint32_t y, uint8_t inventory_slot,
                           uint32_t amount);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~DepositNObjectsCommand();
};

//-----------------------------------------------------------------------------

#endif  // __DEPOSIT_N_OBJECTS_COMMAND_H__
