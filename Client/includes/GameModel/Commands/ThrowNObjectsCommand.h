#ifndef __THROW_N_OBJECTS_COMMAND_H__
#define __THROW_N_OBJECTS_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ThrowNObjectsCommand : public Command {
   private:
    uint8_t inventory_slot;
    uint32_t amount;

   public:
    /* Constructor */
    ThrowNObjectsCommand(uint8_t inventory_slot, uint32_t amount);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ThrowNObjectsCommand();
};

//-----------------------------------------------------------------------------

#endif  // __THROW_N_OBJECTS_COMMAND_H__
