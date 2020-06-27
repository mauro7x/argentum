#ifndef __UNEQUIP_OBJECT_COMMAND_H__
#define __UNEQUIP_OBJECT_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UnequipObjectCommand : public Command {
   private:
    int8_t equipment_slot;

   public:
    /* Constructor */
    UnequipObjectCommand(int8_t equipment_slot);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UnequipObjectCommand();
};

//-----------------------------------------------------------------------------

#endif  // __UNEQUIP_OBJECT_COMMAND_H__
