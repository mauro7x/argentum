#ifndef __SELL_N_OBJECTS_COMMAND_H__
#define __SELL_N_OBJECTS_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SellNObjectsCommand : public Command {
   private:
    uint32_t x;
    uint32_t y;
    uint8_t inventory_slot;
    uint32_t amount;

   public:
    /* Constructor */
    SellNObjectsCommand(uint32_t x, uint32_t y, uint8_t inventory_slot,
                        uint32_t amount);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~SellNObjectsCommand();
};

//-----------------------------------------------------------------------------

#endif  // __SELL_N_OBJECTS_COMMAND_H__
